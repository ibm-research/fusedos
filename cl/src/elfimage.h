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

//! \file  ElfImage.h
//! \brief Declaration of ElfImage classes.

#ifndef ELFIMAGE_H
#define ELFIMAGE_H

// Includes
#include <elf.h>
#include <sys/stat.h>
#include <ramdisk/include/services/MessageHeader.h>
#include <ramdisk/include/services/JobctlMessages.h>
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include "cioerror.h"
#endif                                                                                                                 // FUSEDOS
#include <stddef.h>

#ifndef NGROUPS_MAX
#define NGROUPS_MAX    65536
#endif


//! \brief Describe a loadable segment in an ELF-format executable file.


//! \brief Manage an ELF-format executable file.
//!
//! This class opens and validates an ELF executable file that is loaded on a
//! compute node.

class ElfImage
{

public:

    ElfImage();
    ElfImage(const char* filename);
    ~ElfImage();

    int loadProgram();
    int openExecutable();
    int readSegment(Elf64_Phdr *phdr, void *buffer, size_t size, const char *name);
    void getInterpreter(char *interpreter, size_t interpreter_size);
    int checkBGQNote(Elf64_Phdr *phdr);
    int readRegion(void *buffer, uint64_t offset, uint64_t size);
    int addSegment(AppState_t *appState, Elf64_Phdr *phdr, uint64_t poffset, uint32_t base_flags);
    int placeSegment(Elf64_Phdr *phdr, uint64_t poffset, uint64_t *next_avail_paddr);

    //! \brief  Return pathname string for the executable.
    //! \return String with pathname.

    inline const char* getFilename(void) const { return _filename; }

    //! \brief  Return program entry point.
    //! \return Program entry point address.

    inline Elf64_Addr getEntryPoint(void) const { return _ehdr.e_entry; }

    int numPhdrs();
    Elf64_Phdr *getPhdr(int index);

    //! \brief  Return the OS ABI value for the executable.
    //! \return OS ABI value.

    inline unsigned char getOSABI(void) const { return _ehdr.e_ident[EI_OSABI]; }

    //! \brief Return the object file type for the executable
    //! \return e_type value

    inline Elf64_Half getObjType(void) const { return _ehdr.e_type; }

    //! \brief  Return error reason value (which can be zero).
    //! \return Error reason that identifies the exact failure.

    inline bgcios::ReturnCode getErrorReason(void) const { return _errorReason; }

private:

    // Private methods.
    void init(void);
    bool hasAuthority(void);
    void setNoteData(Elf64_Nhdr *nhdr);

    //! Pathname of executable file.
    char _filename[bgcios::jobctl::MaxPathSize];

    //! Pathname of interpreter, empty string if none found.
    char _interpreter[bgcios::jobctl::MaxPathSize];

    //! Storage for the PT_NOTE segment, if any.
    //  Note sections are generally about 32 bytes each, and we don't expect
    //  more than two of them.  Leave room enough for eight.
    char _noteseg[256];

    //! ELF file header.
    Elf64_Ehdr _ehdr;

    //! Storage for program header table.
    //  We expect about 8 program headers at 56 bytes each.
    //  Allow twice that many.
    char _segmentTable[1024];

    //!< Status info for executable file.
    struct stat _fileInfo;

    //! Error reason code.
    bgcios::ReturnCode _errorReason;

    //!< File descriptor to executable file.
    int _fd;
};

#endif // ELFIMAGE_H
