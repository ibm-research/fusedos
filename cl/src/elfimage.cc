/* begin_generated_IBM_copyright_prolog                             */
/*                                                                  */
/* This is an automatically generated copyright prolog.             */
/* After initializing,  DO NOT MODIFY OR MOVE                       */
/* ================================================================ */
/*                                                                  */
/* Licensed Materials - Property of IBM                             */
/*                                                                  */
/* Blue Gene/Q                                                      */
/*                                                                  */
/* (C) Copyright IBM Corp.  2004, 2012                              */
/*                                                                  */
/* US Government Users Restricted Rights -                          */
/* Use, duplication or disclosure restricted                        */
/* by GSA ADP Schedule Contract with IBM Corp.                      */
/*                                                                  */
/* This software is available to you under the                      */
/* Eclipse Public License (EPL).                                    */
/*                                                                  */
/* ================================================================ */
/*                                                                  */
/* end_generated_IBM_copyright_prolog                               */

//! \file ElfImage.cc
//! \brief Methods for ElfImage class.

// Includes
#include "Kernel.h"
#include "elfimage.h"
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include "Personality_inlines.h"
#endif                                                                                                                 // FUSEDOS
#include "rank.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <unistd.h>
#include <elf.h>
#include <arpa/inet.h>
#include <new>

#if defined __FUSEDOS__                                                                                               // FUSEDOS
extern void *my_memory_top;                                                                                            // FUSEDOS
#endif                                                                                                                 // FUSEDOS

char elfstorage[sizeof(ElfImage)];

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
// MUDM interface for performing collective job loads
struct mudm_rdma_bcast rdma_load_bcast;
#endif                                                                                                                 // FUSEDOS

//! \brief Default constructor.

ElfImage::ElfImage()
{
    strncpy(_filename, "", sizeof(_filename));
    init();
}

//! \brief Constructor.
//! \param filename Path to executable file.

ElfImage::ElfImage(const char* filename)
{
    strncpy(_filename, filename, sizeof(_filename));
    init();
}

//! \brief  Initialize private data.
//! \return Nothing.

void
ElfImage::init(void)
{
    strncpy(_interpreter, "", sizeof(_interpreter));
    _fd = -1;
    memset(&_ehdr, '\0', sizeof(_ehdr));
    _errorReason = bgcios::Success;
    memset(&_fileInfo, '\0', sizeof(_fileInfo));

    return;
}

//! \brief Default destructor.

ElfImage::~ElfImage()
{
    // Close the file.
    if (_fd != -1) {
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
        (void) internal_close(_fd);
#else                                                                                                                  // FUSEDOS
        (void)close(_fd);                                                                                              // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    }
}

//! \brief  Open and load the sections of the ELF-format executable file.
//! \return 0 when successful, errno when unsuccessful.

int
ElfImage::loadProgram()
{
    int rc = 0;
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    int internalrc = 0;
    AppState_t* app = GetMyAppState();
    if(!App_IsCollectiveLoadActive())
    {
#endif                                                                                                                 // FUSEDOS
	rc = openExecutable();
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    }
    else
    {
        // Collective job load is active.

        // Determine the generation count value that we will be expecting with the return code.
        uint32_t iteration = app->jobLeaderData.collectiveLoadStatus.word.iteration + 1;
        if (App_IsLoadLeaderNode())
        {
	    rc = openExecutable();

            // Wait until all nodes reach this point
            internalrc = MUSPI_GIBarrierEnterAndWait( &systemLoadJobGIBarrier );
            assert(internalrc == 0);

            // Setup to perform the RDMA broadcast
            CollectiveLoadStatus_t newLoadStatus;
            newLoadStatus.word.iteration = iteration;
            newLoadStatus.word.returncode = rc;
            rdma_load_bcast.status_mem = (uint64_t)&(app->jobLeaderData.collectiveLoadStatus.dword);
            rdma_load_bcast.status_val = newLoadStatus.dword;
            // Replicate the this object's data on all nodes so that they can continue processing.
            rdma_load_bcast.source_payload_paddr = this;
            rdma_load_bcast.payload_length =  sizeof(ElfImage);
            rdma_load_bcast.class_route = 13; // job-wide system class route.
            rdma_load_bcast.dest_payload_paddr = this;
            rdma_load_bcast.num_in_class_route = app->LoadNodeCount;
            rdma_load_bcast.requestID = NULL;
            Kernel_WriteFlightLog(FLIGHTLOG, FL_APPBCASTO, (uint64_t)rdma_load_bcast.source_payload_paddr,rdma_load_bcast.payload_length,newLoadStatus.dword,0);
            //printf("Broadcast ElfImage object data. addr %016lx len %016lx, status %016lx\n", (uint64_t)this,sizeof(ElfImage),newLoadStatus.dword );
            do
            {
                internalrc = mudm_bcast_RDMA_write(NodeState.MUDM,  &rdma_load_bcast);
            } while (internalrc == -EBUSY);
            if (internalrc != -EINPROGRESS)
                printf("(E) mudm_bcast_RDMA_write of section headers returned %d\n", internalrc);
            assert(internalrc == -EINPROGRESS);

        }
        else
        {
            // Enter the barrier so that the job leader knows that we are ready.
            internalrc = MUSPI_GIBarrierEnterAndWait( &systemLoadJobGIBarrier );
            assert(internalrc == 0);

        }
        // SPIN until we see a response from the job leader with the with the expected
        // iteration count and corresponding return code from the job leader
        while(app->jobLeaderData.collectiveLoadStatus.word.iteration < iteration)
        {
            ppc_msync();
        }
        // obtain the return code from the broadcast operation
        rc = app->jobLeaderData.collectiveLoadStatus.word.returncode;
    }
#endif                                                                                                                 // FUSEDOS
    return rc;
}

//! \brief  Open the ELF-format executable file and validate the header.
//! \return 0 when successful, errno when unsuccessful.

int
ElfImage::openExecutable()
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    uint64_t cnk_rc;
#else                                                                                                                  // FUSEDOS
    int rc;
#endif                                                                                                                 // FUSEDOS

    // Make sure there is a valid file name.
    if (_filename[0] == '\0') {
        TRACE( TRACE_Jobctl, ("(E) ElfImage::openExecutable: file name string is empty\n") );
        _errorReason = bgcios::AppOpenError;
        return EINVAL;
    }

    // Open the executable file.
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    cnk_rc = internal_open(_filename, O_RDONLY, 0);
    if (CNK_RC_IS_FAILURE(cnk_rc)) {
        rc = CNK_RC_ERRNO(cnk_rc);
        TRACE( TRACE_Jobctl, ("(E) ElfImage::openExecutable: open(%s) failed, %d\n", _filename, rc) );
        _errorReason = bgcios::AppOpenError;
        return rc;
    }
    _fd = (int) CNK_RC_VALUE(cnk_rc);
#else                                                                                                                  // FUSEDOS
    rc = open(_filename, O_RDONLY, 0);                                                                                 // FUSEDOS
    if (rc == -1) {                                                                                                    // FUSEDOS
        int error = errno;                                                                                             // FUSEDOS
        TRACE( TRACE_Jobctl, ("(E) ElfImage::openExecutable: open(%s) failed, %d\n", _filename, error) );              // FUSEDOS
        return error;                                                                                                  // FUSEDOS
    }                                                                                                                  // FUSEDOS
    _fd = rc;                                                                                                          // FUSEDOS
#endif                                                                                                                 // FUSEDOS

    // Get info about the executable file.
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    cnk_rc = internal_fstat(_fd, &_fileInfo);
    if (CNK_RC_IS_FAILURE(cnk_rc)) {
        rc = CNK_RC_ERRNO(cnk_rc);
        TRACE( TRACE_Jobctl, ("(E) ElfImage::openExecutable: fstat(%d) failed, %d\n", _fd, rc) );
        _errorReason = bgcios::AppReadError;
        return rc;
    }
#else                                                                                                                  // FUSEDOS
    rc = fstat(_fd, &_fileInfo);                                                                                       // FUSEDOS
    if (rc == -1) {                                                                                                    // FUSEDOS
        int error = errno;                                                                                             // FUSEDOS
        TRACE( TRACE_Jobctl, ("(E) ElfImage::openExecutable: fstat(%d) failed, %d\n", _fd, error) );                   // FUSEDOS
        return error;                                                                                                  // FUSEDOS
    }                                                                                                                  // FUSEDOS
#endif                                                                                                                 // FUSEDOS

    // Make sure the user has execute authority to the executable file.
    if (!hasAuthority()) {
        TRACE( TRACE_Jobctl, ("(E) ElfImage::openExecutable: user does not have execute authority to \"%s\"\n", _filename) );
        _errorReason = bgcios::AppAuthorityError;
        return EACCES;
    }

    // Make sure the executable file is at least as big as an ELF header.
    if ((uint64_t)_fileInfo.st_size < sizeof(Elf64_Ehdr)) {
        TRACE( TRACE_Jobctl, ("(E) ElfImage::openExecutable: file size of %ld bytes is smaller than ELF header\n", _fileInfo.st_size) );
        _errorReason = bgcios::AppElfHeaderSize;
        return ENOEXEC;
    }

    // Read the ELF header which is always at the beginning of the file.
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    cnk_rc = internal_pread64(_fd, &_ehdr, sizeof(_ehdr), 0);
    if (CNK_RC_IS_FAILURE(cnk_rc)) {
        rc = CNK_RC_ERRNO(cnk_rc);
        TRACE( TRACE_Jobctl, ("(E) ElfImage::openExecutable: read of elf header failed with errno %d\n", rc) );
        _errorReason = bgcios::AppReadError;
        return rc;
    }
    int nbytes = (int) CNK_RC_VALUE(cnk_rc);
#else                                                                                                                  // FUSEDOS
    rc = pread64(_fd, &_ehdr, sizeof(_ehdr), 0);                                                                       // FUSEDOS
    if (rc == -1) {                                                                                                    // FUSEDOS
        int error = errno;                                                                                             // FUSEDOS
        TRACE( TRACE_Jobctl, ("(E) ElfImage::openExecutable: read of elf header failed with errno %d\n", error) );     // FUSEDOS
        return error;                                                                                                  // FUSEDOS
    }                                                                                                                  // FUSEDOS
    int nbytes = rc;                                                                                                   // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    if (nbytes != sizeof(_ehdr)) {
        TRACE( TRACE_Jobctl, ("(E) ElfImage::openExecutable: read of elf header truncated (got %d bytes, expected %ld)\n", nbytes, sizeof(_ehdr)) );
        _errorReason = bgcios::AppReadError;
        return ENODATA;
    }

    // Make sure the magic value is valid.
    if (memcmp(_ehdr.e_ident, ELFMAG, SELFMAG) != 0) {
        TRACE( TRACE_Jobctl, ("(E) ElfImage::openExecutable: magic value %4.4s is invalid\n", _ehdr.e_ident) );
        _errorReason = bgcios::AppElfHeaderError;
        return ENOEXEC;
    }

    // Make sure the file class is 64-bit.
    if (_ehdr.e_ident[EI_CLASS] != ELFCLASS64) {
        TRACE( TRACE_Jobctl, ("(E) ElfImage::openExecutable: class %d is not a 64-bit\n", _ehdr.e_ident[EI_CLASS]) );
        _errorReason = bgcios::AppElfHeaderError;
        return ENOEXEC;
    }

    // Make sure the data encoding is big endian.
    if (_ehdr.e_ident[EI_DATA] != ELFDATA2MSB) {
        TRACE( TRACE_Jobctl, ("(E) ElfImage::openExecutable: data encoding %d is not big endian\n", _ehdr.e_ident[EI_DATA]) );
        _errorReason = bgcios::AppElfHeaderError;
        return ENOEXEC;
    }

    // Make sure the OSABI is UNIX System V.
    if (_ehdr.e_ident[EI_OSABI] != ELFOSABI_SYSV) {
        TRACE( TRACE_Jobctl, ("(E) ElfImage::openExecutable: OSABI %d is not SYSV\n", _ehdr.e_ident[EI_OSABI]) );
        _errorReason = bgcios::AppElfHeaderError;
        return ENOEXEC;
    }

    // Make sure the object file type is executable.
    if ( (_ehdr.e_type != ET_EXEC) && (_ehdr.e_type != ET_DYN) ) {
        TRACE( TRACE_Jobctl, ("(E) ElfImage::openExecutable: type 0x%04x is not executable nor dynamic\n", _ehdr.e_type) );
        _errorReason = bgcios::AppElfHeaderError;
        return ENOEXEC;
    }

    // Make sure the architecture is PPC64.
    if (_ehdr.e_machine != EM_PPC64) {
        TRACE( TRACE_Jobctl, ("(E) ElfImage::openExecutable: machine %d is not PowerPC\n", _ehdr.e_machine) );
        _errorReason = bgcios::AppElfHeaderError;
        return ENOEXEC;
    }

    // Read the program header table from the ELF file.
    Elf64_Word segmentTableSize = _ehdr.e_phnum * _ehdr.e_phentsize;
    if (segmentTableSize > sizeof(_segmentTable)) {
        TRACE( TRACE_Jobctl, ("(E) ElfImage::openExecutable: program headers (%d bytes) larger than maximum (%ld)", segmentTableSize, sizeof(_segmentTable)) );
        _errorReason = bgcios::AppElfHeaderError;
        return ENOEXEC;
    }
    rc = readRegion(_segmentTable, _ehdr.e_phoff, segmentTableSize);
    if (rc != 0) {
        TRACE( TRACE_Jobctl, ("(E) ElfImage::openExecutable: reading program header table failed, %d\n", rc) );
        return rc;
    }

    // Read PT_NOTE and PT_INTERP segments, if present.
    int index;
    for (index = 0; index < numPhdrs(); index++)
    {
	Elf64_Phdr *phdr = getPhdr(index);
	if (phdr->p_type == PT_NOTE) {
	    rc = readSegment(phdr, _noteseg, sizeof(_noteseg), "PT_NOTE");
	    if (rc != 0) return rc;
	}
	if (phdr->p_type == PT_INTERP) {
	    rc = readSegment(phdr, _interpreter, sizeof(_interpreter), "PT_INTERP");
	    if (rc != 0) return rc;
	}
    }

    TRACE( TRACE_Jobctl, ("(I) ElfImage::openExecutable: opened and validated elf file \"%s\"\n", _filename) );
    return 0;
}

int
ElfImage::readSegment(Elf64_Phdr *phdr, void *buffer, size_t size, const char *name)
{
    int rc;
    if (phdr->p_filesz > size) {
	TRACE( TRACE_Jobctl, ("(E) ElfImage::readSegment: %s segment (%ld bytes) larger than maximum (%ld)", name, phdr->p_filesz, size) );
	_errorReason = bgcios::AppElfHeaderError;
	return ENOEXEC;
    }
    rc = readRegion(buffer, phdr->p_offset, phdr->p_filesz);
    if (rc != 0) {
	TRACE( TRACE_Jobctl, ("(E) ElfImage::readSegment: reading %s segment failed, rc %d\n", name, rc) );
	return rc;
    }
    return 0;
}

void
ElfImage::getInterpreter(char *interpreter, size_t interpreter_size)
{
    assert(interpreter_size >= sizeof(_interpreter));
    strncpy(interpreter, _interpreter, interpreter_size);
}

int
ElfImage::checkBGQNote(Elf64_Phdr *phdr)
{
    // The "name" field is at offset 12 in a note.  Look for "BGQ".
    char *p = _noteseg;
    while ((p + 12 + 4) < (_noteseg + phdr->p_filesz)) {
	// at least 4 characters of the name are in the buffer
	if (strncmp(p+12, "BGQ", 4) == 0) {
	    // found
	    return 0;
	}
	// move to the next note in the buffer
	p += (12 +                               // 3 fixed fields
	      ((((uint32_t *) p)[0] + 3) & ~3) + // length of name
	      ((((uint32_t *) p)[1] + 3) & ~3)); // length of data
    }

    TRACE( TRACE_Jobctl, ("(E) ElfImage::checkBGQNote: application not compiled for BGQ\n") );
    _errorReason = bgcios::AppElfHeaderError;
    return ENOEXEC;
}

//! \brief  Read a region from the executable file.
//! \param  buffer Pointer to buffer for storing data.
//! \param  offset Offset to start of region in file.
//! \param  size Size of region.
//! \return 0 when successful, errno when unsuccessful.

int
ElfImage::readRegion(void *buffer, uint64_t offset, uint64_t size)
{
    // Read the region from the file.
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    uint64_t cnk_rc;
#else                                                                                                                  // FUSEDOS
    int rc;                                                                                                            // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    Elf64_Word bytesLeft = size;
    char *bufp = (char *)buffer;
    do {
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
       cnk_rc = internal_pread64(_fd, bufp, bytesLeft, offset);
       if (CNK_RC_IS_FAILURE(cnk_rc)) {
	   int errnum = CNK_RC_ERRNO(cnk_rc);
	   TRACE( TRACE_Jobctl, ("(E) ElfImage::readRegion: pread failed for %lu bytes at offset %lu, %d\n", size, offset, errnum) );
           _errorReason = bgcios::AppReadError;
	   return errnum;
       }
       int64_t nbytes = (int64_t) CNK_RC_VALUE(cnk_rc);
#else                                                                                                                  // FUSEDOS
       rc = pread64(_fd, bufp, bytesLeft, offset);                                                                     // FUSEDOS
       if (rc == -1) {                                                                                                 // FUSEDOS
	   int error = errno;                                                                                          // FUSEDOS
	   TRACE( TRACE_Jobctl, ("(E) ElfImage::readRegion: pread failed for %lu bytes at offset %lu, %d, bufp 0x%p\n", // FUSEDOS
                  size, offset, error, bufp) );                                                                        // FUSEDOS
	   // TODO MARIUS continue here -- why does it fail?!
	   return error;                                                                                               // FUSEDOS
       }                                                                                                               // FUSEDOS
       int nbytes = rc;                                                                                                // FUSEDOS
#endif                                                                                                                 // FUSEDOS
       if (nbytes <= 0) {
	   TRACE( TRACE_Jobctl, ("(E) ElfImage::readRegion: pread returned %d bytes when %lu bytes were requested at offset %lu\n",
                                 nbytes, size, offset) );
	   _errorReason = bgcios::AppReadError;
	   return ENODATA;
       }

       bytesLeft -= nbytes;
       bufp += nbytes;
       offset += nbytes;
    } while (bytesLeft > 0);

    return 0;
}

//! \brief  Check for execute authority to an executable file.
//! \return True if the user has execute authority.

bool
ElfImage::hasAuthority(void)
{
    // See if the owner has authority.
    AppState_t *app = GetMyAppState();
    if (app->UserID == _fileInfo.st_uid) {
        if (_fileInfo.st_mode & S_IXUSR) {
            return true;
        }
        else {
            return false;
        }
    }

    // See if the group owner has authority.
    if (app->GroupID == _fileInfo.st_gid) {
        if (_fileInfo.st_mode & S_IXGRP) {
            return true;
        }
        else {
            return false;
        }
    }

#if 1
    // See if one of the secondary groups has authority.
    for (uint32_t index = 0; index < app->NumSecondaryGroups; ++index) {
        if (app->SecondaryGroups[index] == _fileInfo.st_gid) {
            if (_fileInfo.st_mode & S_IXGRP) {
                return true;
            }
            else {
                return false;
            }
        }
    }
#endif

    // See if other has authority.
    if (_fileInfo.st_mode & S_IXOTH) {
        return true;
    }

    // The user does not have authority.
    return false;
}


//! \brief  Set variables from a BGP-tag note section.
//! \param  nhdr Pointer to note header.
//! \return Nothing.

void
ElfImage::setNoteData(Elf64_Nhdr *nhdr)
{
#if 0
    if (nhdr->n_type == 16) {
        // Start at beginning of note descriptor.
        char *p = (char *)nhdr + sizeof(*nhdr) + nhdr->n_namesz;

        // The first string is the release.
        std::string releaseVariable = "BG_RELEASE=";
        releaseVariable += p;
        _variables.push_back(releaseVariable);
    }
#endif

    return;
}

int
ElfImage::addSegment(AppState_t *appState, Elf64_Phdr *phdr, uint64_t poffset, uint32_t base_flags)
{
    if (appState->App_NumSegs >= APP_MAX_SEGS)
    {
        TRACE( TRACE_Jobctl, ("(E) ElfImage::addSegment: too many segments (max %d)\n", APP_MAX_SEGS) );
	_errorReason = bgcios::AppElfHeaderError;
	return ENOEXEC;
    }
    AppSeg_t *seg = &(appState->App_Segments[appState->App_NumSegs]);
    seg->seg_va     = phdr->p_vaddr;
    seg->seg_pa     = phdr->p_paddr + poffset;
    seg->seg_filesz = phdr->p_filesz;
    seg->seg_memsz  = phdr->p_memsz;
    seg->seg_csum   = 0;
    if (phdr->p_type == PT_LOAD) base_flags |= APP_FLAGS_LOAD;
    if (phdr->p_type == PT_TLS)  base_flags |= APP_FLAGS_TLS;
    if (phdr->p_flags & PF_R)    base_flags |= APP_FLAGS_R;
    if (phdr->p_flags & PF_W)    base_flags |= APP_FLAGS_W;
    if (phdr->p_flags & PF_X)    base_flags |= APP_FLAGS_X;
    seg->seg_flags  = base_flags;
    TRACE( TRACE_Jobctl, ("(I) ElfImage::addSegment: added segment %03u: va=0x%08lx pa=0x%08lx filesz=0x%lx memsz=0x%lx flags=0x%x\n",
			  appState->App_NumSegs, seg->seg_va, seg->seg_pa, seg->seg_filesz, seg->seg_memsz, seg->seg_flags) );
    appState->App_NumSegs++;
    return 0;
}

int
ElfImage::placeSegment(Elf64_Phdr *phdr, uint64_t poffset, uint64_t *poffset_adjust)
{
    int rc = 0;

    // Map a TLB for the segment.
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    uint64_t paddr = ROUND_DN_1G(phdr->p_paddr + poffset);
#endif                                                                                                                 // FUSEDOS
    uint64_t vaddr = ROUND_DN_1G(phdr->p_vaddr + VMM_APP_LOAD_VADDR);
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    uint64_t last_tlb_vaddr  = ROUND_DN_1G(phdr->p_vaddr + VMM_APP_LOAD_VADDR + phdr->p_filesz);
    while(vaddr <= last_tlb_vaddr) {
        rc = vmm_MapUserSpace( APP_FLAGS_R | APP_FLAGS_W | APP_FLAGS_ESEL(3) | APP_FLAGS_NONSPECULATIVE,
                               (void*)paddr,
                               (void*)vaddr,
                               VMM_PAGE_SIZE_1G,
                               0,
                               0); // needs to be kernel size
        vaddr += VMM_PAGE_SIZE_1G;
        paddr += VMM_PAGE_SIZE_1G;
    }
    if (poffset_adjust)
    {
        if ((paddr - VMM_PAGE_SIZE_1G) > *poffset_adjust)
        {
            *poffset_adjust = paddr - VMM_PAGE_SIZE_1G;
        }
    }
#endif                                                                                                                 // FUSEDOS
    // Temporarily set text address range so address validation is successful.
    AppProcess_t* p = GetMyProcess();
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    vaddr = phdr->p_vaddr + VMM_APP_LOAD_VADDR;
#else                                                                                                                  // FUSEDOS
	vaddr = phdr->p_vaddr;
#endif                                                                                                                 // FUSEDOS
    p->Text_VStart = vaddr;
    p->Text_VEnd   = vaddr + phdr->p_filesz - 1;
    p->Text_PStart = phdr->p_paddr + poffset;
    p->Text_PEnd   = p->Text_PStart + phdr->p_filesz - 1;
    TRACE( TRACE_Jobctl, ("(I) elf_placesegment: reading segment into vaddr=0x%lx paddr=0x%lx size=0x%08lx offset=0x%08lx\n",
			  p->Text_VStart, p->Text_PStart, phdr->p_filesz, phdr->p_offset) );

    // Read the segment from the elf file.
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    if(!App_IsCollectiveLoadActive())
    {
	rc = readRegion((void*)vaddr, phdr->p_offset, phdr->p_filesz);
#else                                                                                                                  // FUSEDOS
	rc = readRegion((void*)(p->Text_PStart), phdr->p_offset, phdr->p_filesz);
#endif                                                                                                                 // FUSEDOS
	// TODO MARIUS: we must read to the appropriate position
	// (check with former version)
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    }
    else
    {
	AppState_t *app = GetMyAppState();
	int internalrc = 0;
	// Determine the iteration count we will be sending in the job leader and expecting in the other nodes.
	// This must be obtained prior to the GA barrier.
	uint32_t iteration = app->jobLeaderData.collectiveLoadStatus.word.iteration + 1;

	// Collective job load is active.
	if (App_IsLoadLeaderNode())
	{
	    rc = readRegion((void*)vaddr, phdr->p_offset, phdr->p_filesz);

	    // Wait until all nodes reach this point
	    internalrc = MUSPI_GIBarrierEnterAndWait( &systemLoadJobGIBarrier );
	    assert(internalrc == 0);

	    // Setup status information for the operation
	    CollectiveLoadStatus_t newLoadStatus;
	    newLoadStatus.word.iteration = iteration;
	    newLoadStatus.word.returncode = rc;
	    rdma_load_bcast.status_mem = (uint64_t)(&(app->jobLeaderData.collectiveLoadStatus.dword));
	    rdma_load_bcast.status_val = newLoadStatus.dword;
	    // Setup to perform the rdma broadcast
	    rdma_load_bcast.class_route = 13; // job-wide system class route.
	    rdma_load_bcast.num_in_class_route = app->LoadNodeCount;
	    rdma_load_bcast.requestID = NULL;
	    // Broadcast the region data to all the nodes.
	    rdma_load_bcast.source_payload_paddr = (rc == 0) ? (void*)p->Text_PStart : (void*)&_errorReason;
	    rdma_load_bcast.payload_length = ((rc == 0) ? p->Text_PEnd + 1 - p->Text_PStart : sizeof(_errorReason));
	    rdma_load_bcast.dest_payload_paddr = (rc == 0) ? (void*)p->Text_PStart : (void*)&_errorReason;
	    Kernel_WriteFlightLog(FLIGHTLOG, FL_APPBCASTS, (uint64_t)rdma_load_bcast.source_payload_paddr,rdma_load_bcast.payload_length,newLoadStatus.dword,0);
	    //printf("Broadcast section data. addr %016lx len %016lx status %016lx\n", (uint64_t)rdma_load_bcast.source_payload_paddr,rdma_load_bcast.payload_length, newLoadStatus.dword );
	    // Issue the broadcast operation. Loop until we get through since we may be still busy with a previous request.
	    do
	    {
		internalrc = mudm_bcast_RDMA_write(NodeState.MUDM,  &rdma_load_bcast);
	    } while(internalrc == -EBUSY);
	    if (internalrc != -EINPROGRESS)
	    {
		printf("(E) mudm_bcast_RDMA_write in elf_placesegment returned status %d\n", internalrc);
	    }
	    assert(internalrc == -EINPROGRESS);
	}
	else
	{
	    // Enter the barrier so that the job leader knows that we are ready.
	    internalrc = MUSPI_GIBarrierEnterAndWait( &systemLoadJobGIBarrier );
	    assert(internalrc == 0);
	}
	// SPIN until we see a matching iteration count. Once we have a match,
	// we know that the return code is stored.
	while(app->jobLeaderData.collectiveLoadStatus.word.iteration < iteration)
	{
	    ppc_msync();
	}
	// obtain the return code from the broadcast operation
	rc = app->jobLeaderData.collectiveLoadStatus.word.returncode;
    }
#endif                                                                                                                 // FUSEDOS

    // Reset text address range.
    p->Text_VStart = p->Text_VEnd = 0;
    p->Text_PStart = p->Text_PEnd = 0;
    vaddr = ROUND_DN_1G(phdr->p_vaddr + VMM_APP_LOAD_VADDR);

    // Unmap TLB.
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    while(vaddr <= last_tlb_vaddr) {
        vmm_UnmapUserSpace((void*)vaddr, VMM_PAGE_SIZE_1G, 0);
        vaddr += VMM_PAGE_SIZE_1G;
    }
#endif                                                                                                                 // FUSEDOS

    return rc;
}

int
ElfImage::numPhdrs()
{
    return _ehdr.e_phnum;
}

Elf64_Phdr *
ElfImage::getPhdr(int index)
{
    assert(index < _ehdr.e_phnum);
    return (Elf64_Phdr *) (_segmentTable + (index * _ehdr.e_phentsize));
}

// TODO MARIUS this function cleary needs rework // FUSEDOS
int elf_loadimage(AppState_t *appState, uint64_t physical_offset, uint64_t *poffset_adjust)
{
    int rc;
    bgcios::ReturnCode error = bgcios::Success;
    int index;
    Elf64_Phdr *phdr;
    int dynamic = false;
    int code_seg_found = false;
    uint64_t next_avail_pa = 0;
    uint64_t local_poffset_adjust = 0;
    char interpreter[bgcios::jobctl::MaxPathSize] = "";
    if (poffset_adjust)
    {
        local_poffset_adjust = *poffset_adjust;
    }

    const char* filename = (char*)&appState->App_Args[0];
    TRACE( TRACE_Jobctl, ("(I) elf_loadimage: loading elf file '%s'\n", filename) );

    memset(elfstorage, 0x00, sizeof(ElfImage));
    ElfImage* elf = new(elfstorage) ElfImage(filename);
    assert (elf != NULL);

    rc = elf->loadProgram();
    if (rc != 0) goto done;

    for (index = 0; index < elf->numPhdrs(); index++)
    {
	phdr = elf->getPhdr(index);
	switch (phdr->p_type)
	{
	    case PT_LOAD:
	    case PT_TLS:
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
		rc = elf->addSegment(appState, phdr, local_poffset_adjust, 0);
#else                                                                                                                  // FUSEDOS
		// for FusedOS, we pass physical_offset here, instead of the
		// adjust value
		rc = elf->addSegment(appState, phdr, physical_offset, 0);
#endif                                                                                                                 // FUSEDOS
		if (rc != 0) goto done;
		break;
	    case PT_NOTE:
		rc = elf->checkBGQNote(phdr);
		if (rc != 0) goto done;
		break;
	    case PT_INTERP:
		dynamic = true;
		elf->getInterpreter(interpreter, sizeof(interpreter));
		break;
	    default:
		break;
	}
    }

    for (index = 0; index < elf->numPhdrs(); index++)
    {
	phdr = elf->getPhdr(index);
	if (phdr->p_type == PT_LOAD)
	{
	    if ((phdr->p_paddr % (1024*1024)) != 0) {
		TRACE( TRACE_Jobctl, ("(E) elf_loadimage: PT_LOAD segment at address 0x%08lx is not 1MB aligned\n", phdr->p_paddr) );
		error = bgcios::AppSegmentAlignment;
		rc = ENOEXEC;
		goto done;
	    }
	    if ((phdr->p_flags & PF_X) && (phdr->p_filesz > 0)) {
		code_seg_found = true;
	    }
	    if (dynamic)
	    {
		// find pa high-water mark, where we'll place the interpreter
		if (next_avail_pa < (phdr->p_paddr + phdr->p_memsz))
		{
		    next_avail_pa = phdr->p_paddr + phdr->p_memsz;
		}
	    }
	    else
	    {
		rc = elf->placeSegment(phdr, physical_offset, poffset_adjust);
		if (rc != 0) goto done;
	    }
	}
    }

    // Make sure there is code in the executable.
    if (!code_seg_found) {
        TRACE( TRACE_Jobctl, ("(E) elf_loadimage: no code segment in program image\n") );
        error = bgcios::AppNoCodeSection;
        rc = ENOEXEC;
	goto done;
    }

    if (dynamic)
    {
	appState->invokeWithInterpreter = 1;
	const char *interpreter_ptr = NULL;
	App_GetEnvString("BG_INTERPRETER", &interpreter_ptr);
	if (interpreter_ptr != NULL)
	{
	    strncpy(interpreter, interpreter_ptr, sizeof(interpreter));
	}
	TRACE( TRACE_Jobctl, ("(I) elf_loadimage: loading interpreter '%s' for dynamically linked executable\n", interpreter) );

        // Save full path to the interpreter into the AppState object
        strncpy(appState->InterpreterPathAndName, interpreter, MIN(sizeof(interpreter), sizeof(appState->InterpreterPathAndName)));

        // We're done with the original image.  Now switch to the interpreter.
	elf->~ElfImage();
	memset(elfstorage, 0x00, sizeof(ElfImage));
	elf = new(elfstorage) ElfImage(interpreter);
	rc = elf->loadProgram();
	if (rc != 0) goto done;

	next_avail_pa = ROUND_UP_1M(next_avail_pa);
	for (index = 0; index < elf->numPhdrs(); index++)
	{
	    Elf64_Phdr *phdr = elf->getPhdr(index);
	    if (phdr->p_type == PT_LOAD)
	    {
		rc = elf->addSegment(appState, phdr, next_avail_pa, APP_FLAGS_DYN);
		if (rc != 0) goto done;
		rc = elf->placeSegment(phdr, next_avail_pa + physical_offset, NULL);
		if (rc != 0) goto done;
	    }
	}
    }

    appState->App_EntryPoint = elf->getEntryPoint();
    TRACE( TRACE_Jobctl, ("(I) elf_loadimage: entry point 0x%016lx\n", appState->App_EntryPoint) );

done:
    if (rc != 0)
    {
	if (error == bgcios::Success)
	{
	    error = elf->getErrorReason();
	}
	App_SetLoadState(AppState_LoadFailed, error, rc);
    }
    elf->~ElfImage();
    return rc;
}
