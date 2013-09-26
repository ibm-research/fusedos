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
/* (C) Copyright IBM Corp.  2008, 2012                              */
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

#ifndef _KERNEL_UPCI_H_
#define _KERNEL_UPCI_H_

//! \addtogroup upc_spi
//@{
/**
 * \file
 *
 * \brief UPC compile time definitions which vary
 * between firmware or user application compiles
 *
 */
//@}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include "signal.h"
#endif                                                                                                                 // FUSEDOS
#include "cnk/include/Config.h"
#if defined __FUSEDOS__                                                                                               // FUSEDOS
#include "spi/include/kernel/location.h"                                                                               // FUSEDOS
#endif                                                                                                                 // FUSEDOS
#include "spi/include/upci/upc_a2_pc.h"
#include "spi/include/upci/upc_c.h"
#include "spi/include/upci/punit_config.h"
#include "spi/include/upci/node_parms.h"

__BEGIN_DECLS


//! UPCI_INCL_DEBUG_DATA will include some extra rough counters
//! that can be used help track that certain activities are occurring.
//! The counters are not thread safe (they are incremented non-atomically,
//! so the value is not completely reliable.
//! The global variables are declared and used in the user state side of the
//! syscalls, but defined in spi/src/upci/us_debug.c
#define UPCI_INCL_DEBUG_DATA

#ifdef UPCI_INCL_DEBUG_DATA
typedef struct sUpci_Debug_Counts {
    volatile uint64_t syscallRetries;
    volatile uint64_t syscallInits;
    volatile uint64_t syscallFrees;
    volatile uint64_t syscallApplies;
    volatile uint64_t syscallXCoreApplies;
} Upci_Debug_Counts_t;

#define UPCI_DEBUG_COUNTS_INIT { 0, 0, 0, 0, 0 }
extern Upci_Debug_Counts_t upciDebugCounts;
void Upci_DumpDebugCounts(unsigned indent);
#define UPCI_INCR_DEBUG_COUNT(_ctr_) (upciDebugCounts._ctr_)++

#else
#define UPCI_INCR_DEBUG_COUNT(_ctr_)
#endif



//! UPCI uses one syscall, but with multiple operations.
//! The Upci_Kopts_t enums are used to select the operation.
typedef enum eUpci_Kops
{
    UPCI_KOP_UNDEF = 0,        //!< undefined ucpi operation
    UPCI_KOP_RESETINTS,        //!< reset int status and Enable PM Interrupts for the current hardware thread
    UCPI_KOP_GETPARMS,         //!< Get Node Parms
    UCPI_KOP_MODEINIT,         //!< Init UPC Hardware to mode (refct++)
    UPCI_KOP_A2PC_APPLY,       //!< apply a2 registers
    UPCI_KOP_A2PC_GET,         //!< Get a2 registers.
    UPCI_KOP_PUNIT_CFG_APPLY,  //!< apply punit config to the target unit (must be same as current core) (supports ints, swaps)
    UPCI_KOP_PUNIT_CFG_ATTACH, //!< attach punit config to target unit (can be diff core) (no ints, no swaps)
    UCPI_KOP_MODEFREE,         //!< Free UPC Hardware mode (refct--)
    UPCI_KOP_SETBGPMTHREAD,    //!< Set hwthread to current sw Bgpm Thread
    UPCI_KOP_CLEARBGPMTHREAD,  //!< Clear hwthread to sw bgpm thread
    UPCI_KOP_WAIT4MAILBOX,     //!< Wait for mailbox to empty before returning
    UCPI_KOP_GETKDEBUG,        //!< Get some debug info from kernel
    UCPI_KOP_SETPMSIG,         //!< Set signal value to be produced by PM interrupts
    UPCI_KOP_GETCNKCTRS,       //!< Sample CNK Counters

} Upci_Kops_t;




/**
 * \brief Kernel_Upci_ResetInts
 * Reset UPC_P Interrupt Status bits and Enable PM Interrupts
 * for the current hardware thread.
 * Needed at the end of the Signal Handler to atomically
 * re-enable interrupts for this thread to prevent conflicts
 * with context switches or other thread activity, since
 * the register requires read-modify-write.
 */
int Kernel_Upci_ResetInts(uint64_t intStatusMask);



/**
 * \brief Kernel_Upci_GetNodeParms
 * Kernel syscall into to query various privileged metrics and parameters to be globally maintained.
 * Must be done prior to the Kernel_Upci_Mode_Init as it will change the upc configuration.
 * @param[out]  pNodeParms     structure to receive data.
 */
int Kernel_Upci_GetNodeParms(Upci_Node_Parms_t *pNodeParms);



//! Upci_KDebug_t - structure of various UPCI debug values to pass through syscall
typedef struct sUpci_KDebug {
    uint64_t xucr1;      // xucr1 register to check livelock settings
    uint64_t iullcr;     // iullcr register to check livelock settings
    uint64_t jobid;      // The current job id
    uint32_t coreInitMask; // indicates which cores performed UPC_Init at job load (should be all cores)
} Upci_KDebug_t;
#define UPCI_KDEBUG_INIT { 0, 0, 0, 0 }



/**
 * \brief Kernel_Upci_GetKDebug
 * Kernel syscall into to query some UPCI debug values from the kernel operation.
 * @param[out]  pKDebug     structure to receive data.
 */
int Kernel_Upci_GetKDebug(Upci_KDebug_t *pKDebug);
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#endif                                                                                                                 // FUSEDOS





//! Upci_ModeInitData_t - intermed data structure to pass thru syscall
typedef struct sModeInitData {
    unsigned          upcMode;
    UPC_Ctr_Mode_t    ctrMode;
    unsigned          unit;
} Upci_ModeInitData_t;




/**
 * \brief Kernel_Upci_Mode_Init
 * Kernel syscall into hardware mode initialization
 * Note: In fwext call there is no synchronization.
 *       From a cnk syscall only the 1st caller will initialize the
 *          upc hardware. Subsequent calls will only check that the
 *          mode is consistent and fail if not.
 *       The syscall does reference counting, so use
 *          Use Kernel_Upci_Mode_Free to free up use on the node to allow
 *          the mode to be changed if all using sw thread do the free operation.
 * @param[in]  upcMode     The UPC mode to initialize on UPC_C
 * @param[in]  ctrMode     Indicate how to initialize other units and whether to also initialize upc_p and L2 unit mode and rings - normally yes
 *                         In a firmware extension test this argument is ignored and and least one process per core must do a mode init instead.
 *                         (Originally created to allow one thread to initialize all hardware, but the cross-core DCR support is broken).
 * @param[in]  unit        Which UPC_P unit to attach to UPC_C for UPC modes 1 & 2
 *                          (-1) \ref UPC_LOCAL_UNIT indicates to use the UPC_P associated with the
 *                               the current hw thread.
 */
int Kernel_Upci_Mode_Init( unsigned upcMode, UPC_Ctr_Mode_t ctrMode, unsigned unit );



/**
 * \brief Kernel_Upci_Mode_Free
 * Kernel syscall to indicate that software thread is done using UPC hardware fwext call there is no synchronization.
 * \todo It only does reference counting, so if a thread on the node does not explicitly
 *       do the free, the mode is still locked.  Could we add hooks into thread completion
 *       to handle this is a better fashion?  Not really needed.
 */
int Kernel_Upci_Mode_Free();



/**
 * \brief Kernel_Upci_A2PC_ApplyRegs
 * Apply values for A2 Perf Counting Unit config registers (current core only)
 * @param[in]  pA2pc  structure with register values
 * @return Error indication
 * @retval 0 (\ref UPCI_EOK)
 * @retval \ref UPCI_EINV_PARM
 * @retval \ref UPCI_ESYSCALL_DATASIZE
 *
 */
int Kernel_Upci_A2PC_ApplyRegs( Upci_A2PC_t *pA2pc);


/**
 * \brief Kernel_Upci_A2PC_GetRegs
 * Get values in A2 Perf Counting Unit config registers (current core only)
 * @param[in,out]  pA2pcVal  structure to hold results
 * @return Error indication
 * @retval 0 (\ref UPCI_EOK)
 * @retval \ref UPCI_EINV_PARM
 * @retval \ref UPCI_ESYSCALL_DATASIZE
 *
 */
int Kernel_Upci_A2PC_GetRegs( Upci_A2PC_Val_t *pA2pcVal);

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS


/**
 * \brief Kernel_Upci_Punit_Cfg_Apply
 * Apply Punit Cfg to the current core unit (unitId must be equal to current core)
 * Supports overflow interrupts and sw context swaps.
 * @param[in]  pCfg    config structure
 * @param[in]  unitId
 *
 * @return Error indication
 * @retval \ref UPCI_EOK
 * @retval \ref UPCI_EINV_PARM
 * @retval \ref UPCI_ESYSCALL_DATASIZE
 *
 * \todo examine all the return values closely
 *
 */
int Kernel_Upci_Punit_Cfg_Apply( Upci_Punit_Cfg_t *pCfg, unsigned unitId);



/**
 * \brief Kernel_Upci_Punit_Cfg_Attach
 * Attach Punit Cfg to the given core unit
 * unitId can different than current core.
 * Not ovf int support or context swap support.
 * @param[in]  pCfg    config structure
 * @param[in]  unitId
 *
 * @return Error indication
 * @retval \ref UPCI_EOK
 * @retval \ref UPCI_EINV_PARM
 * @retval \ref UPCI_ESYSCALL_DATASIZE
 *
 * \todo examine all the return values closely
 *
 */
int Kernel_Upci_Punit_Cfg_Attach( Upci_Punit_Cfg_t *pCfg, unsigned unitId);


//! Kernel_Upci_SetBgpmThread()
//! Associate or clear current sw thread with current hw thread for subsequent pm signal delivery.
int Kernel_Upci_SetBgpmThread();
int Kernel_Upci_ClearBgpmThread();







//! Wait4MailboxEmpty()
//! Because on BGQ, printf operations buffer the data to bedram mailbox, and then
//! the control system asynchronously snarfs the data for the actual print, the
//! printing and A2 run can happen simultaneously.
//! This simultaneous operation can effect the event counts - especially on DD1 which
//! has a workaround which halts the core threads while the printing is occurring.
//! However, the clocks continue to run.
//! Thus, use this routine mitigate the likely-hood of prints during critical sections.
//! It does not prevent another thread from later starting a print which then freezes all
//! threads again.
//!
//! @return # of tests which occur till mailbox empty (for a sanity check)
int Kernel_Upci_Wait4MailboxEmpty();



//! SetPmSig()
//! Set signal value that PM interrupts should register for the sw thread which currently
//! owns the hardware thread.
int Kernel_Upci_SetPmSig(int sig);
#define UPCI_DEFAULT_PM_SIGNAL (34) // (SIGRTMAX-3)  // SIGRTMAX may be a function call



/**
 * CNK Counter Types.  Can only collect counters from one type at a time.
 */
typedef enum {
    CNKCtrNode = 0,
    CNKCtrProc,
    CNKCtrHWT,
} Upci_CNKCtrType_t;

/**
 * \brief Kernel_Upci_GetCNKCounts
 * Retrieve the current value of CNK counters
 * @param[in]      ctrType  type of the counters
 * @param[in,out]  ctrBuff  buffer of ids and filled in values
 * @param[in]      buffLen  number of entries in buffer
 *
 * @return Error indication
 * @retval \ref UPCI_EOK
 * @retval \ref UPCI_EINV_PARM
 * @retval \ref UPCI_ESYSCALL_DATASIZE
 *
 */
int Kernel_Upci_GetCNKCounts( Upci_CNKCtrType_t ctrType, PerfCountItem_t *ctrBuff, int buffLen);



//! Get cached LockID value.
uint64_t Upci_GetLockID();




#include "upci_impl.h"
#endif                                                                                                                 // FUSEDOS



__END_DECLS

#endif


