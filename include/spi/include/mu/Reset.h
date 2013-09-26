/*                                                                  */
/* Licensed Materials - Property of IBM                             */
/*                                                                  */
/* Blue Gene/Q                                                      */
/*                                                                  */
/* (c) Copyright IBM Corp. 2011, 2012 All Rights Reserved           */
/*                                                                  */
/* US Government Users Restricted Rights - Use, duplication or      */
/* disclosure restricted by GSA ADP Schedule Contract with IBM      */
/* Corporation.                                                     */
/*                                                                  */
/* This software is available to you under either the GNU General   */
/* Public License (GPL) version 2 or the Eclipse Public License     */
/* (EPL) at your discretion.                                        */
/*                                                                  */

#ifndef	_MUSPI_RESET__H_ /* Prevent multiple inclusion */
#define	_MUSPI_RESET__H_

#include <hwi/include/common/compiler_support.h>
#include <hwi/include/bqc/dcr_support.h>
#include <hwi/include/bqc/MU_Macros.h>
#include <hwi/include/bqc/classroute.h>
#include <hwi/include/bqc/mu_dcr.h>
#include <hwi/include/bqc/nd_500_dcr.h>
#include <hwi/include/bqc/nd_rese_dcr.h>
#include <hwi/include/bqc/nd_x2_dcr.h>

__BEGIN_DECLS

typedef struct MUSPI_RESET
{
    uint64_t sequence;
    uint64_t old_classroutes[BGQ_COLL_CLASS_MAX_CLASSROUTES/2];
    uint64_t nd_linkmask;
    uint64_t nd_holdreset;
    uint64_t skip_collectives;
    uint64_t clear_user;
    uint64_t clear_system;
    uint64_t clear_allsubgroups;
    uint64_t clear_subgroup;
    uint64_t lastSequenceID;
} MUSPI_RESET_t;

__INLINE__
int32_t MUSPI_InitReset(MUSPI_RESET_t* reset)
{
    int x;
    
    memset(reset, 0, sizeof(MUSPI_RESET_t));
    reset->sequence = 1;
    reset->nd_linkmask        = (1<<ND_RESE_DCR_num)-1;
    reset->skip_collectives   = 0;
    reset->clear_user         = 1;
    reset->clear_system       = 0;
    reset->clear_allsubgroups = 1;
    reset->clear_subgroup     = 0;
    reset->nd_holdreset       = 0;
    reset->lastSequenceID     = 0xff;
    
    // save away all classroutes
    for(x=0; x<BGQ_COLL_CLASS_MAX_CLASSROUTES/2; x++)
    {
        reset->old_classroutes[x] = DCRReadPriv(ND_500_DCR(CTRL_COLL_CLASS_00_01) + x);    // Save away classroutes
    }
    return 0;
}

__INLINE__
int32_t MUSPI_InitResetHold(MUSPI_RESET_t* reset, uint64_t subgroup, uint64_t link)
{
    MUSPI_InitReset(reset);
    reset->nd_linkmask         = 1<<link;
    reset->skip_collectives    = 1;
    reset->clear_system        = 1;
    reset->clear_allsubgroups  = 0;
    reset->clear_subgroup      = subgroup;
    reset->lastSequenceID      = 7;
    return 0;
}

__INLINE__
int32_t MUSPI_InitResetRelease(MUSPI_RESET_t* reset, uint64_t subgroup, uint64_t link)
{
    MUSPI_InitReset(reset);
    reset->nd_linkmask         = 1<<link;
    reset->skip_collectives    = 1;
    reset->clear_system        = 1;
    reset->clear_allsubgroups  = 0;
    reset->clear_subgroup      = subgroup;
    reset->nd_holdreset        = 1;
    reset->sequence            = 8;
    return 0;
}


/**************************************************************
1.System waits until all system collective packets have been received and no new system collectives will be injected (could hang if bad user descriptor on DD1)
 **************************************************************/
__INLINE__
int32_t MUSPI_ResetIdle(MUSPI_RESET_t* reset)
{
    return 0;
}


/**************************************************************
1.7. Disable all user rMEs
1.8. Wait for all user rMEs to enter either idle, get tail (rget), get tail(rmFIFO), or commit tail (rmFIFO) state.
1.9. Reset all user rMEs that are in commit tail (rmFIFO) state.
 **************************************************************/
__INLINE__
int32_t MUSPI_ResetCleanupStuckRmes(MUSPI_RESET_t* reset)
{
    uint64_t rMEState[2];
    int x, y;
    int rMEID;
    int wait_done;
    int state;
    uint64_t reset_rME_map = 0;
    uint64_t RME_ENABLE = 0xffff;
    uint64_t count;
    
    if(reset->clear_user)
        RME_ENABLE &= 0x0005;
    if(reset->clear_system)
        RME_ENABLE &= ~0x0005;
    
    // 1.7. Disable all user rMEs
    DCRWritePriv(MU_DCR(RME_ENABLE), MU_DCR__RME_ENABLE__VALUE_set(RME_ENABLE));
    
    // 1.8. Wait for all user rMEs to enter either idle, get tail (rget), get tail(rmFIFO), or commit tail (rmFIFO) state.
    
    do 
    { 
        wait_done = 1;
        rMEState[0]= DCRReadPriv(MU_DCR(RME_STATE0));
        rMEState[1]= DCRReadPriv(MU_DCR(RME_STATE1));
        reset_rME_map = 0;
        
        for(x=0; x<2; x++)
        {			
            for(y=0; y<8; y++)
            {			
                rMEID = x*8+y;
                state = _BGQ_GET(5, 24+4+5*y, rMEState[x]);
                
                if(!reset->clear_system && ((rMEID == 13) || (rMEID == 15)))
                {
                }
                else if(!reset->clear_user && ((rMEID != 13) && (rMEID != 15)))
                {
                }
                else
                {
                    if ((state != 0) && ((state < 0x10) || (state > 0x12))) // not entered idle/wait state
                    {
                        wait_done = 0;
                    }
                    else if (state == 0x12) // wait ctail state -> needs to be reset
                    {
                        reset_rME_map |= MU_DCR__RESET__RME_VEC_set(0x8000UL >> rMEID);
                    }
                }
            }
        }
    } while (wait_done == 0);
    
    // 1.9. Reset all user rMEs that are in commit tail (rmFIFO) state.
    DCRWritePriv(MU_DCR(RESET), reset_rME_map); // reset the selected rMEs
    count = 200;  // delay 100 MU cycles.  (this needs to work on Linux, which cannot include A2_inlines)
    while(count--)
    {
        asm volatile("nop;");
    }
    DCRWritePriv(MU_DCR(RESET), 0); // release the reset
    
    return 0;
}


/**************************************************************
2.Disable all user MU reception FIFOs (in DCRs) and injection FIFOs (in MMIO). This causes all reception or rget injection FIFO packets to be dropped,  however puts will continue through to the memory system. This also causes any user next descriptor not to be fetched. Each point-to-point FIFO will finish injecting the message that has already been fetched, but the collective FIFO may hang unless special steps below are taken for user collectives. Read back a DCR and an MMIO address to ensure all writes have completed and no race condition.  
 **************************************************************/
__INLINE__
int32_t MUSPI_ResetDisableUserReception(MUSPI_RESET_t* reset)
{
    uint64_t g;
    volatile uint64_t* ptr;
    volatile uint64_t* system_fifoptr;
    
    uint64_t value;
    for(g=0; g<BGQ_MU_NUM_FIFO_GROUPS; g++)
    {
        if((!reset->clear_allsubgroups) && (g != reset->clear_subgroup / BGQ_MU_NUM_FIFO_SUBGROUPS))
            continue;
        
        ptr = (uint64_t*)BGQ_MU_iDMA_DISABLE_OFFSET(g,0);
        
        value = 0xffffffff;
        if(!reset->clear_user)
        {
            system_fifoptr = (uint64_t *)(BGQ_MU_DCR_iDMA_SYSTEM_FIFO_OFFSET(g));
            value &= (system_fifoptr[0]);
        }
        if(!reset->clear_system)
        {
            system_fifoptr = (uint64_t *)(BGQ_MU_DCR_iDMA_SYSTEM_FIFO_OFFSET(g));
            value &= (~system_fifoptr[0]);
        }
        else if(!reset->clear_allsubgroups)
        {
            value &= _B8(32 + 7 + 8*(reset->clear_subgroup % BGQ_MU_NUM_FIFO_SUBGROUPS), -1);
        }
        *ptr = value;
        
        ptr = (uint64_t*)BGQ_MU_DCR_rDMA_ENABLED_OFFSET(g);
        value = 0xffff;
        if(!reset->clear_user)
        {
            system_fifoptr = (uint64_t *)(BGQ_MU_DCR_rDMA_SYSTEM_FIFO_OFFSET(g));
            value &= ~system_fifoptr[0];
        }
        if(!reset->clear_system)
        {
            system_fifoptr = (uint64_t *)(BGQ_MU_DCR_rDMA_SYSTEM_FIFO_OFFSET(g));
            value &= system_fifoptr[0];
        }
        else if(!reset->clear_allsubgroups)
        {
            value &= ~_B4(48 + 3 + 4*(reset->clear_subgroup % BGQ_MU_NUM_FIFO_SUBGROUPS), -1);
        }
        
        *ptr = (value & (*ptr));
    }
    
    // dummy read from MMIO space.  ptr is volatile so compiler shouldn't optimize out.
    ptr = (uint64_t*)BGQ_MU_iDMA_IS_ENABLED_OFFSET(0,0);
    value = *ptr;

    // dummy read from DCR space.  ptr is volatile so compiler shouldn't optimize out.
    ptr = (uint64_t*)BGQ_MU_DCR_rDMA_ENABLED_OFFSET(0);
    value = *ptr;
    
    // note:  this does not need to be re-enabled since the user task won't be resuming.
    return 0;
}

/**************************************************************
2.1. Enable all user rMEs
 **************************************************************/
__INLINE__
int32_t MUSPI_ResetEnableUserRmes(MUSPI_RESET_t* reset)
{
    DCRWritePriv(MU_DCR(RME_ENABLE), MU_DCR__RME_ENABLE__VALUE_set(0xffffUL));
    return 0;
}

/**************************************************************
2.2. Reset ND system IO tokens
 **************************************************************/
__INLINE__
int32_t MUSPI_ResetNDTokens(MUSPI_RESET_t* reset)
{
    int x;
    uint64_t nd_reset;
    if(reset->clear_system)
    {
        for(x=0; x<ND_RESE_DCR_num; x++)
        {
            if(ND_RESE_DCR__RESET__DCRS_OUT_get(DCRReadPriv(ND_RESE_DCR(x, RESET))) == 0)
            {
                if((reset->nd_linkmask & (1<<x)) == 0)
                    continue;
                
                nd_reset = DCRReadPriv(ND_RESE_DCR(x, RESET));
                ND_RESE_DCR__RESET__SE_TOK3_insert(nd_reset, 1);
                ND_RESE_DCR__RESET__SE_TOK4_insert(nd_reset, 1);
                DCRWritePriv(ND_RESE_DCR(x, RESET),  nd_reset);
            }
        }
    }
    return 0;
}


/**************************************************************
  3.throw away user packets on ND senders

  4.Do a system gi barrier on the compute nodes in the block/sub-block.
 **************************************************************/
__INLINE__
int32_t MUSPI_ResetDisableNDSenders(MUSPI_RESET_t* reset)
{
    int x;
    uint64_t nd_ctrl, nd_clear;
    for(x=0; x<ND_RESE_DCR_num; x++)
    {
        if(ND_RESE_DCR__RESET__DCRS_OUT_get(DCRReadPriv(ND_RESE_DCR(x, RESET))) == 0)
        {        
            if((reset->nd_linkmask & (1<<x)) == 0)
                continue;

	    nd_clear = DCRReadPriv(ND_RESE_DCR(x, CLEAR));
	    ND_RESE_DCR__CLEAR__SE_CLR_DROPPED_PKT_insert(nd_clear,1);
	    DCRWritePriv(ND_RESE_DCR(x, CLEAR), nd_clear);              // hold drop packet interrupt at 0
            
            nd_ctrl = DCRReadPriv(ND_RESE_DCR(x, CTRL));
            if(reset->clear_user)
            {
                ND_RESE_DCR__CTRL__SE_DROP_PKT_USR_insert(nd_ctrl, 1);
            }
            if(reset->clear_system)
            {
                ND_RESE_DCR__CTRL__SE_DROP_PKT_SYS_insert(nd_ctrl, 1);
            }
            DCRWritePriv(ND_RESE_DCR(x, CTRL),  nd_ctrl);               // drop user packets on ND senders
        }
    }
    
    return 0;
}

/**************************************************************
5.reset collective units
 **************************************************************/
__INLINE__
int32_t MUSPI_ResetCollectiveUnit(MUSPI_RESET_t* reset)
{
    uint64_t nd_ctrl;
    if(reset->skip_collectives)
        return 0;
    nd_ctrl = DCRReadPriv(ND_500_DCR(RESET));
    ND_500_DCR__RESET__COLL_insert(nd_ctrl, 1);
    DCRWritePriv(ND_500_DCR(RESET), nd_ctrl);                           // place collective unit in reset
    
    return 0;
}

/**************************************************************
6.reset user receiver VCs for collective packets
 **************************************************************/
__INLINE__
int32_t MUSPI_ResetCollectiveVC(MUSPI_RESET_t* reset)
{
    int x;
    uint64_t nd_reset;
    
    if(reset->skip_collectives)
        return 0;
    for(x=0; x<ND_RESE_DCR_num; x++)
    {
        if(ND_RESE_DCR__RESET__DCRS_OUT_get(DCRReadPriv(ND_RESE_DCR(x, RESET))) == 0)
        {
            if((reset->nd_linkmask & (1<<x)) == 0)
                continue;
            
            nd_reset = DCRReadPriv(ND_RESE_DCR(x, RESET));
            ND_RESE_DCR__RESET__RE_VC4_insert(nd_reset, 1);
            ND_RESE_DCR__RESET__RE_VC5_insert(nd_reset, 1);
            DCRWritePriv(ND_RESE_DCR(x, RESET),  nd_reset);                // reset user receiver VCs
        }
    }
    return 0;
}

/**************************************************************
7.reset user sender tokens for collective VCs
 **************************************************************/
__INLINE__
int32_t MUSPI_ResetCollectiveTokens(MUSPI_RESET_t* reset)
{
    int x;
    uint64_t nd_reset;
    if(reset->skip_collectives)
        return 0;
    for(x=0; x<ND_RESE_DCR_num; x++)
    {
        if(ND_RESE_DCR__RESET__DCRS_OUT_get(DCRReadPriv(ND_RESE_DCR(x, RESET))) == 0)
        {
            if((reset->nd_linkmask & (1<<x)) == 0)
                continue;
            
            nd_reset = DCRReadPriv(ND_RESE_DCR(x, RESET));
            ND_RESE_DCR__RESET__SE_TOK4_insert(nd_reset, 1); 
            ND_RESE_DCR__RESET__SE_TOK5_insert(nd_reset, 1);
            DCRWritePriv(ND_RESE_DCR(x, RESET),  nd_reset);               // reset all 12 user receiver VCs
        }
    }
    return 0;
}

/**************************************************************
8.program all user collective class maps to be loopback only
 **************************************************************/
__INLINE__
int32_t MUSPI_ResetCollectiveLoopback(MUSPI_RESET_t* reset)
{
    // program all classroutes to loopback mode
    int x;
    if(reset->skip_collectives)
        return 0;
    for(x=0; x<BGQ_COLL_CLASS_MAX_CLASSROUTES/2; x++)
    {
        DCRWritePriv(ND_500_DCR(CTRL_COLL_CLASS_00_01) + x,                                                                             
                     ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS00_TYPE_set(0) |
                     ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS00_UP_PORT_I_set(BGQ_CLASS_INPUT_LINK_LOCAL) |
                     ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS00_UP_PORT_O_set(0) |
                     ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS01_TYPE_set(0) |
                     ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS01_UP_PORT_I_set(BGQ_CLASS_INPUT_LINK_LOCAL) | 
                     ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS01_UP_PORT_O_set(0)
            );
    }
    
    return 0;
}

/**************************************************************
9.release collective unit from reset
a.any collective fifo messages in progress (from injection descriptor in progress) will be thrown away as it tries to enter reception FIFOs,  collective puts will go through to the memory.  Thus all user MU collective messages will eventually clear.
 **************************************************************/
__INLINE__
int32_t MUSPI_ResetCollectiveUnitRelease(MUSPI_RESET_t* reset)
{
    uint64_t nd_ctrl;
    if(reset->skip_collectives)
        return 0;
    nd_ctrl = DCRReadPriv(ND_500_DCR(RESET));
    ND_500_DCR__RESET__COLL_insert(nd_ctrl, 0);
    DCRWritePriv(ND_500_DCR(RESET),  nd_ctrl);                      // release collective unit from reset
    return 0;
}

/**************************************************************
10. wait for user MU and ND to quiesce: wait for
a.the user descriptor fetching bit to clear, 
b.iMEs and rMEs are idle
c.clean ND user states: ND injection and reception fifos are empty, all user VC fifos are empty, all user sender tokens have been returned.
 **************************************************************/
__INLINE__
int32_t MUSPI_ResetWaitCollectiveClear(MUSPI_RESET_t* reset)
{
    uint64_t fetching_bit;
    uint64_t iMEState  = 0;
    uint64_t rMEState0 = 0;
    uint64_t rMEState1 = 0;
    int x;
    uint64_t headp[6];
    uint64_t tailp[6];
    uint64_t headtail_mismatch;
    uint64_t ndrese_not_clear;
    if(reset->skip_collectives)
        return 0;
    do
    {
        fetching_bit = DCRReadPriv(MU_DCR(MCSRAM_BD_FETCHING_DESC));
        
        iMEState = DCRReadPriv(MU_DCR(IME_STATE));
        rMEState0= DCRReadPriv(MU_DCR(RME_STATE0));
        rMEState1= DCRReadPriv(MU_DCR(RME_STATE1));
        
        for(x=0; x<2; x++)
        {
            headp[x] = DCRReadPriv(ND_500_DCR(INJ_FIFO_HEAD_P0) + x);
            tailp[x] = DCRReadPriv(ND_500_DCR(INJ_FIFO_TAIL_P0) + x);
        }
        for(x=0; x<4; x++)
        {
            headp[x+2] = DCRReadPriv(ND_500_DCR(RCP_FIFO_HEAD_P0) + x);
            tailp[x+2] = DCRReadPriv(ND_500_DCR(RCP_FIFO_TAIL_P0) + x);
        }
        
        for(x=0, headtail_mismatch=0; x<6; x++)
        {
            if(headp[x] != tailp[x]) headtail_mismatch = 1;
        }
        
        for(x=0, ndrese_not_clear = 0; x<ND_RESE_DCR_num; x++)
        {
            if(ND_RESE_DCR__RESET__DCRS_OUT_get(DCRReadPriv(ND_RESE_DCR(x, RESET))) == 0)
            {
                if((reset->nd_linkmask & (1<<x)) == 0)
                    continue;
                
                if(DCRReadPriv(ND_RESE_DCR(x, RE_VC0_PKT_CNT)) != 0)
                    ndrese_not_clear = 1;
                if(ND_RESE_DCR__RE_VC1_STATE__CTRL_SLOT_VALID_get( DCRReadPriv(ND_RESE_DCR(x, RE_VC1_STATE))))
                    ndrese_not_clear = 1;
                if(ND_RESE_DCR__RE_VC2_STATE__CTRL_SLOT_VALID_get( DCRReadPriv(ND_RESE_DCR(x, RE_VC2_STATE))))
                    ndrese_not_clear = 1;
                
                // number of SE tokens can be reduced via a DCR.  This is currently hardcoding the value.  Could be more flexible to 
                // read the expected number of tokens.  But this value is unlikely to change unless there is a hardware problem
                // that needs a workaround.
                if((DCRReadPriv(ND_RESE_DCR(x, SE_TOKENS)) & 0xfffff00ful) != 0x07e77007ul)
                    ndrese_not_clear = 1;
            }
        }
    }
    while((MU_DCR__MCSRAM_BD_FETCHING_DESC__USR_get(fetching_bit) != 0) ||
          ((iMEState & (~0x249249249249)) != 0) ||  // iME state is neither idle or wait-for-job
          (rMEState0 != 0) ||
          (rMEState1 != 0) ||
          headtail_mismatch ||
          ndrese_not_clear);
    return 0;
}

/**************************************************************
12.Because of inflight packets, a node may have entered previous barrier just before a user packet arrives.  So repeat step 10 and do another system gi barrier on these nodes.

13.release receiver VCs for collective packets
 **************************************************************/
__INLINE__
int32_t MUSPI_ResetReleaseVC(MUSPI_RESET_t* reset)
{
    int x;
    uint64_t nd_reset;
    if(reset->skip_collectives)
        return 0;
    for(x=0; x<ND_RESE_DCR_num; x++)
    {
        if(ND_RESE_DCR__RESET__DCRS_OUT_get(DCRReadPriv(ND_RESE_DCR(x, RESET))) == 0)
        {
            if((reset->nd_linkmask & (1<<x)) == 0)
                continue;
            
            nd_reset = DCRReadPriv(ND_RESE_DCR(x, RESET));
            ND_RESE_DCR__RESET__RE_VC4_insert(nd_reset, 0);
            ND_RESE_DCR__RESET__RE_VC5_insert(nd_reset, 0);
            DCRWritePriv(ND_RESE_DCR(x, RESET),  nd_reset);  // reset user receiver VCs
        }
    }
    return 0;
}

/**************************************************************
14.stop throwing away user packets on ND senders
 **************************************************************/
__INLINE__
int32_t MUSPI_ResetPermitNDSenders(MUSPI_RESET_t* reset)
{
    int x;
    uint64_t nd_ctrl, nd_clear;
    for(x=0; x<ND_RESE_DCR_num; x++)
    {
        if(ND_RESE_DCR__RESET__DCRS_OUT_get(DCRReadPriv(ND_RESE_DCR(x, RESET))) == 0)
        {
            if((reset->nd_linkmask & (1<<x)) == 0)
                continue;
            
            nd_ctrl = DCRReadPriv(ND_RESE_DCR(x, CTRL));
            if(reset->clear_user)
            {
                ND_RESE_DCR__CTRL__SE_DROP_PKT_USR_insert(nd_ctrl, 0);
            }
            if(reset->clear_system)
            {
                ND_RESE_DCR__CTRL__SE_DROP_PKT_SYS_insert(nd_ctrl, 0);
            }
            DCRWritePriv(ND_RESE_DCR(x, CTRL), nd_ctrl);  // don't drop user packets

	    nd_clear = DCRReadPriv(ND_RESE_DCR(x, CLEAR));
	    ND_RESE_DCR__CLEAR__SE_CLR_DROPPED_PKT_insert(nd_clear,0);
	    DCRWritePriv(ND_RESE_DCR(x, CLEAR), nd_clear);              // holding drop packet interrupt at 0
        }
    }
    return 0;
}

// Helper functions to clear MU registers
__INLINE__
void MUSPI_ResetWriteMU ( uint64_t address, uint64_t value )
{
  volatile uint64_t *a = (uint64_t*)address;
  *a = value;
}

__INLINE__
uint64_t MUSPI_ResetReadMU( uint64_t address)
{
  volatile uint64_t *a = (uint64_t*)address;
  return *a;
}


__INLINE__
void MUSPI_ResetClearBitsMU( uint64_t address, uint64_t bitmask)
{
  volatile uint64_t *a = (uint64_t*)address;
  *a &= ~bitmask;
}

__INLINE__
void MUSPI_ResetClearMMIOIntMU(uint64_t g, uint64_t sg, uint64_t bitmask)
{
  uint64_t high_bits = MUSPI_ResetReadMU(BGQ_MU_READ_INT_STATUS_REGISTER_OFFSET(g,sg)) & bitmask;

  if (high_bits != 0)
  {
      MUSPI_ResetWriteMU( BGQ_MU_CLEAR_INT_STATUS_REGISTER_OFFSET(g, sg), high_bits);
  }
}

__INLINE__
void MUSPI_ResetClearDcrIntMU(uint64_t addr, uint64_t bitmask)
{
  uint64_t high_bits = DCRReadPriv(addr) & bitmask;

  if (high_bits != 0)
  {
      DCRWritePriv(addr, high_bits);
  }
}

// Initialize inj FIFOs in a given subgroup
__INLINE__
void MUSPI_ResetInitInjFifoSubgroupMU(MUSPI_RESET_t* reset, volatile uint64_t* system_ififoptr, uint64_t g, uint64_t sg)
{
    int i;
    uint64_t fifo_bitmask;
    uint64_t int_bitmask;

    for(i=0; i<BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP; i++) 
    {
        fifo_bitmask = _BN(64 - BGQ_MU_NUM_INJ_FIFOS_PER_GROUP + sg*BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP + i);
	int_bitmask  = fifo_bitmask << (BGQ_MU_NUM_REC_FIFOS_PER_GROUP*2);

	if((reset->clear_system && ((system_ififoptr[0] & fifo_bitmask) != 0)) ||
	   (reset->clear_user   && ((system_ififoptr[0] & fifo_bitmask) == 0)))
	{
	    MUSPI_ResetWriteMU(BGQ_MU_iDMA_START_OFFSET(g, sg, i), 0);         // start field
	    MUSPI_ResetWriteMU(BGQ_MU_iDMA_SIZE_OFFSET(g, sg, i), 0x3fUL);     // size  field
	    MUSPI_ResetWriteMU(BGQ_MU_iDMA_HEAD_OFFSET(g, sg, i), 0);          // head  field
	    MUSPI_ResetWriteMU(BGQ_MU_iDMA_TAIL_OFFSET(g, sg, i), 0);          // tail  field
	    MUSPI_ResetWriteMU(BGQ_MU_iDMA_DESC_COUNTER_OFFSET(g, sg, i),  0); // desc counter field

	    MUSPI_ResetClearBitsMU(BGQ_MU_iDMA_HIGH_PRIORITY_OFFSET(g,sg), fifo_bitmask); // high priority flag
	    MUSPI_ResetClearBitsMU(BGQ_MU_DCR_iDMA_THRESHOLD_INTERRUPT_ENABLE_OFFSET(g), fifo_bitmask); // clear thold int en
	    MUSPI_ResetClearBitsMU(BGQ_MU_DCR_iDMA_FIFO_AVAILABLE_REMOTE_GET_OFFSET(g), fifo_bitmask);  // clear rget flag
	    MUSPI_ResetClearMMIOIntMU(g, sg, int_bitmask); // clear imfifo threshold interrupt
	}
    }
}

// Initialize rec FIFOs in a given subgroup
__INLINE__
void MUSPI_ResetInitRecFifoSubgroupMU(MUSPI_RESET_t* reset, volatile uint64_t* system_rfifoptr, uint64_t g, uint64_t sg)
{
    int i;
    uint64_t fifo_bitmask;
    uint64_t int_bitmask;

    for(i=0; i<BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP; i++)
    {
        fifo_bitmask = _BN(64 - BGQ_MU_NUM_REC_FIFOS_PER_GROUP + sg*BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP + i);
	int_bitmask = fifo_bitmask | (fifo_bitmask << BGQ_MU_NUM_REC_FIFOS_PER_GROUP);

	if((reset->clear_system && ((system_rfifoptr[0] & fifo_bitmask) != 0)) ||
	   (reset->clear_user   && ((system_rfifoptr[0] & fifo_bitmask) == 0)))
	{
	    MUSPI_ResetWriteMU(BGQ_MU_rDMA_START_OFFSET(g, sg, i), 0);     // start field
	    MUSPI_ResetWriteMU(BGQ_MU_rDMA_SIZE_OFFSET(g, sg, i), 0x1FUL); // size  field
	    MUSPI_ResetWriteMU(BGQ_MU_rDMA_HEAD_OFFSET(g, sg, i), 0);      // head  field
	    MUSPI_ResetWriteMU(BGQ_MU_rDMA_TAIL_OFFSET(g, sg, i), 0);      // tail  field

	    MUSPI_ResetClearBitsMU(BGQ_MU_DCR_rDMA_THRESHOLD_INTERRUPT_ENABLE_OFFSET(g), fifo_bitmask);       // clear thold int en
	    MUSPI_ResetClearBitsMU(BGQ_MU_DCR_rDMA_PACKET_RECEIVED_INTERRUPT_ENABLE_OFFSET(g), fifo_bitmask); // clear pkt int en
	    MUSPI_ResetClearMMIOIntMU(g, sg, int_bitmask); // clear rmfifo threshold / packet arrival interrupt
	    
	}
    }
}

// returns bit mask in which _BN(x), _BN(x+1), ..., _BN(y) are 1s
__INLINE__
uint64_t MUSPI_ResetBN_XtoY(uint64_t x, uint64_t y) { return (_BN(x) << 1) - _BN(y); }


// Clear possible software error interrupts
__INLINE__
void MUSPI_ResetClearAllDcrIntMU(void)
{
    MUSPI_ResetClearDcrIntMU(MU_DCR(MASTER_PORT0_INTERRUPTS__STATE), MUSPI_ResetBN_XtoY(24, 25));
    MUSPI_ResetClearDcrIntMU(MU_DCR(MASTER_PORT1_INTERRUPTS__STATE), MUSPI_ResetBN_XtoY(24, 25));
    MUSPI_ResetClearDcrIntMU(MU_DCR(MASTER_PORT2_INTERRUPTS__STATE), MUSPI_ResetBN_XtoY(24, 25));
    MUSPI_ResetClearDcrIntMU(MU_DCR(SLAVE_PORT_INTERRUPTS__STATE),   MUSPI_ResetBN_XtoY(21, 27));
    MUSPI_ResetClearDcrIntMU(MU_DCR(MMREGS_INTERRUPTS__STATE),       MUSPI_ResetBN_XtoY(9,  18));
    MUSPI_ResetClearDcrIntMU(MU_DCR(FIFO_INTERRUPTS__STATE),         _BN(63));
    MUSPI_ResetClearDcrIntMU(MU_DCR(MCSRAM_INTERRUPTS__STATE),       MUSPI_ResetBN_XtoY(15, 27) | MUSPI_ResetBN_XtoY(31, 32));
    MUSPI_ResetClearDcrIntMU(MU_DCR(RCSRAM_INTERRUPTS__STATE),       MUSPI_ResetBN_XtoY(12, 15));
    MUSPI_ResetClearDcrIntMU(MU_DCR(RPUTSRAM_INTERRUPTS__STATE),     MUSPI_ResetBN_XtoY(8, 13));
    MUSPI_ResetClearDcrIntMU(MU_DCR(RME_INTERRUPTS0__STATE),         _BN(5)|_BN(11)|_BN(17)|_BN(23)|_BN(29)|_BN(35)|_BN(41)|_BN(47));
    MUSPI_ResetClearDcrIntMU(MU_DCR(RME_INTERRUPTS1__STATE),         _BN(4)|_BN(10)|_BN(16)|_BN(22)|_BN(28)|_BN(34)|_BN(40)|_BN(46));
    MUSPI_ResetClearDcrIntMU(MU_DCR(ICSRAM_INTERRUPTS__STATE),       MUSPI_ResetBN_XtoY(20, 23));
    MUSPI_ResetClearDcrIntMU(MU_DCR(MISC_INTERRUPTS__STATE),         MUSPI_ResetBN_XtoY(3, 5));
}
    
    
    



/**************************************************************
15.Put MU user mmio and DCRs in clean state
a.Range registers, zero injection and reception fifos, BATs,  user/system DCR bits, user gi control registers etc
 **************************************************************/
__INLINE__
int32_t MUSPI_ResetRestoreMU(MUSPI_RESET_t* reset)
{
    uint64_t g;
    uint64_t sg;
    int i;
    volatile uint64_t* system_ififoptr;
    volatile uint64_t* system_rfifoptr;
    volatile uint64_t* BAT;
 
    // initialize user range settings
    if(reset->clear_user)
    {
        for(i=0; i<16; i++)
        {
            DCRWritePriv(MU_DCR(MIN_USR_ADDR_RANGE) + i, 0xffffffffffffffffull);
            DCRWritePriv(MU_DCR(MAX_USR_ADDR_RANGE) + i, 0x0);
        }
	DCRWritePriv(MU_DCR(USR_INJ_RANGE), 0);
    }

   
    for(g=0; g<BGQ_MU_NUM_FIFO_GROUPS; g++)
    {        
        if((!reset->clear_allsubgroups) && (g != reset->clear_subgroup / BGQ_MU_NUM_FIFO_SUBGROUPS))
            continue;
        
        system_ififoptr = (uint64_t *)(BGQ_MU_DCR_iDMA_SYSTEM_FIFO_OFFSET(g));
        system_rfifoptr = (uint64_t *)(BGQ_MU_DCR_rDMA_SYSTEM_FIFO_OFFSET(g));

        for(sg=0; sg<BGQ_MU_NUM_FIFO_SUBGROUPS; sg++)
        {
            if((!reset->clear_allsubgroups) && (sg != reset->clear_subgroup % BGQ_MU_NUM_FIFO_SUBGROUPS))
                continue;
            
	    // inj FIFOs
	    MUSPI_ResetInitInjFifoSubgroupMU(reset, system_ififoptr, g, sg);

	    // rec FIFOs
	    MUSPI_ResetInitRecFifoSubgroupMU(reset, system_rfifoptr, g, sg);
            
	    // base address table
            BAT = (uint64_t*)BGQ_MU_DCR_BAT_MODE0_OFFSET(g);
            if((BAT[g/16] & (0x8000000000000000ull >> ((g%16)*BGQ_MU_NUM_FIFO_SUBGROUPS + sg))) == 0) // user-mode BAT
            {
                // All counters within subgroup use the same system bit.
                for(i=0; i<BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP; i++)
                {
                    uint64_t* addr = (uint64_t*)BGQ_MU_DATA_COUNTER_BASE_ADDRESS_OFFSET(g, sg, i);
                    *addr = 0x1fffffffffUL;
                }
            }
        }
        if(reset->clear_user)
        {
            // Deallocate all fifos in group from user-space
            *system_ififoptr = ( 1ULL << BGQ_MU_NUM_INJ_FIFOS_PER_GROUP ) - 1;
            *system_rfifoptr = ( 1ULL << BGQ_MU_NUM_REC_FIFOS_PER_GROUP ) - 1;
        }
    }
    if(reset->clear_user)
    {
        BAT[0] = ~0UL; // make all base address table system
	BAT[1] = ~0UL;
	// DCRReadPriv(MU_DCR(SYS_BARRIER))
	MUSPI_ResetClearAllDcrIntMU(); // system should not cause software error interrupt
    }

    return 0;
}

// Zero out GI user classroutes, disable interrupt, and remove access to any user GI barriers
__INLINE__
void MUSPI_ResetDeallocUserGI(uint64_t sysgi, uint64_t class_id)
{
    uint64_t giroute;
    uint64_t giinten;

    if((_BN(48+class_id) & sysgi) == 0)
    {
        // clear class route
        giroute = DCRReadPriv(ND_500_DCR(CTRL_GI_CLASS_00_01) + class_id/2);
	giroute &= (0x00000000ffffffffull << (32 * (class_id % 2)));
	DCRWritePriv(ND_500_DCR(CTRL_GI_CLASS_00_01) + class_id/2, giroute);

	//clear GI control bits
	MUSPI_ResetWriteMU(BGQ_MU_GI_CONTROL_OFFSET(0, class_id), 0);

	//clear interrupt enable
	giinten = DCRReadPriv(MU_DCR(BARRIER_INT_EN));
	giinten &= ~(_BGQ_SET(3, 3, 0x7UL) >> (class_id * 4));
	DCRWritePriv(MU_DCR(BARRIER_INT_EN), giinten);

    }
}

/**************************************************************
16.Put ND user DCRs in clean state
a.Zero user gi and collective class routes
b.clear throw away packets interrupts 

c.clear the reset user sender tokens for collective VCs (set reset bit 0)

todo:
  d.clear upc counts
  e.set upc configuration registers to default values
  f.clear injection fifo checksums (does not work on dd1, does work on dd2)
  g.enable sending of gi packets (may be disabled for termination checks)
  h.Report link error ras events and all recoverable error counts such as link error counts and retransmissions – write 1 to clear, then 0 to enable counts again.
  i.Clear any collective overflow Nan interrupts
  j.There may be others that will have to be worked out as problems arise


17.system gi barrier on these nodes
**************************************************************/
__INLINE__
int32_t MUSPI_ResetRestoreND(MUSPI_RESET_t* reset)
{
    uint64_t g;
    uint64_t x;
    uint64_t sysgi;
    uint64_t nd_reset;

    // List of interrupt registers to clear after SRAM initialization
    static const uint64_t int_to_clear[] = {
        MU_DCR(FIFO_INTERRUPTS__STATE),
        MU_DCR(IMU_ECC_INTERRUPTS__STATE),
        MU_DCR(RMU_ECC_INTERRUPTS__STATE),
        MU_DCR(ICSRAM_INTERRUPTS__STATE),
        MU_DCR(RMU_ECC_CORR_INTERRUPTS__STATE),
        MU_DCR(RCSRAM_INTERRUPTS__STATE),
    };
    
    if(reset->skip_collectives == 0)
    {
        for(x=0; x<BGQ_COLL_CLASS_MAX_CLASSROUTES/2; x++)
        {
            // strip any non-system classroutes
            if(ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS00_TYPE_get(reset->old_classroutes[x]) != 1)  reset->old_classroutes[x] &= 0x00000000ffffffffull;
            if(ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS01_TYPE_get(reset->old_classroutes[x]) != 1)  reset->old_classroutes[x] &= 0xffffffff00000000ull;
            
            DCRWritePriv(ND_500_DCR(CTRL_COLL_CLASS_00_01) + x, reset->old_classroutes[x]);
        }
    
        
        sysgi = DCRReadPriv(MU_DCR(SYS_BARRIER));
        for(x=0; x<16; x++)
        {
  	    MUSPI_ResetDeallocUserGI(sysgi, x);
	}
        DCRWritePriv(MU_DCR(SYS_BARRIER), 0xffff);
    }    
    
    // Clear MU packet arrival interrupts
    if(reset->clear_allsubgroups)
    {
        for(g=0; g<BGQ_MU_NUM_FIFO_GROUPS; g++)
        {
            uint64_t* ptr = (uint64_t*)BGQ_MU_CLEAR_INT_STATUS_REGISTER_OFFSET(g,0);
            *ptr = 0xffffffffffffffffull;  // clear all interrupts
        }
    }    
    else
    {
        // ???
    }
    
    for(x = 0; x < sizeof(int_to_clear)/sizeof(int_to_clear[0]); x++)
    {
        DCRWritePriv(int_to_clear[x], ~0UL);
    }
    
    // Clear ND interrupts
    DCRWritePriv(ND_500_DCR(CTRL_CLEAR0), 
                 ND_500_DCR__CTRL_CLEAR0__CLR_HDRCHK_ERR_set(0xfff) | 
                 ND_500_DCR__CTRL_CLEAR0__CLR_COLL_HDRCHK_ERR_set(3) | 
                 ND_500_DCR__CTRL_CLEAR0__INT_RCP_500_CLR_set(0xf0) | 
                 ND_500_DCR__CTRL_CLEAR0__COLL_NONFATAL_CLR_set(0xf0000000));
    DCRWritePriv(ND_500_DCR(CTRL_CLEAR0), 0);
    
    if(reset->nd_holdreset == 0)
    {
        for(x=0; x<ND_RESE_DCR_num; x++)
        {
            if(ND_RESE_DCR__RESET__DCRS_OUT_get(DCRReadPriv(ND_RESE_DCR(x, RESET))) == 0)
            {
                if((reset->nd_linkmask & (1<<x)) == 0)
                    continue;
                
                nd_reset = DCRReadPriv(ND_RESE_DCR(x, RESET));
                ND_RESE_DCR__RESET__SE_TOK4_insert(nd_reset, 0);
                ND_RESE_DCR__RESET__SE_TOK5_insert(nd_reset, 0);
                DCRWritePriv(ND_RESE_DCR(x, RESET),  nd_reset);
            }
        }
    }
    else
    {
        // Mask fatal interrupts.  Control system will unmask when next block boots.
        for(x=0; x<ND_RESE_DCR_num; x++)
        {
            if((reset->nd_linkmask & (1<<x)) == 0)
                continue;
            DCRWritePriv(ND_RESE_DCR(x, FATAL_ERR_ENABLE), 0);
        }
    }
    return 0;
}


__INLINE__
int32_t MUSPI_Reset(MUSPI_RESET_t* reset) 
{
    int32_t rc = 0;
    
//#define CALL(seq, func) if((reset->sequence == seq)&&(rc == 0)) { printf("MUSPI_Reset.  Sequence=%d Step=%s\n", seq, #func); rc = func(reset); }
#define CALL(seq, func) if((reset->sequence == seq)&&(rc == 0)) { rc = func(reset); }
#define LAST(seq)       if(reset->sequence == seq) return rc;
    CALL(1, MUSPI_ResetIdle);
    CALL(2, MUSPI_ResetCleanupStuckRmes);
    CALL(2, MUSPI_ResetDisableUserReception);
    CALL(2, MUSPI_ResetEnableUserRmes);
    CALL(2, MUSPI_ResetNDTokens);
    CALL(3, MUSPI_ResetDisableNDSenders);
    CALL(4, MUSPI_ResetCollectiveUnit);
    CALL(4, MUSPI_ResetCollectiveVC);
    CALL(4, MUSPI_ResetCollectiveTokens);
    CALL(4, MUSPI_ResetCollectiveLoopback);
    CALL(4, MUSPI_ResetCollectiveUnitRelease);
    CALL(5, MUSPI_ResetWaitCollectiveClear);
    CALL(6, MUSPI_ResetWaitCollectiveClear);
    CALL(7, MUSPI_ResetReleaseVC);
    CALL(8, MUSPI_ResetPermitNDSenders);
    CALL(8, MUSPI_ResetRestoreMU);
    CALL(8, MUSPI_ResetRestoreND);
    CALL(9, MUSPI_ResetIdle);
    LAST(10);
    LAST(reset->lastSequenceID);
#undef CALL
#undef LAST
    
    if(rc == 0)
    {
        reset->sequence++;
        return EAGAIN;
    }
    return rc;
}

__END_DECLS

#endif
