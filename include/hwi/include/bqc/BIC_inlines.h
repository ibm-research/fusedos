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
/* (C) Copyright IBM Corp.  2010, 2012                              */
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
//
// BGQ Interrupt Controller device control functions
//
#ifndef BIC_INLINES_H
#define BIC_INLINES_H

#include <hwi/include/bqc/BIC.h>
#include <hwi/include/bqc/A2_inlines.h>
#include <hwi/include/common/compiler_support.h>
#include <hwi/include/bqc/PhysicalMap.h>
#include <hwi/include/bqc/gea_dcr.h>

__BEGIN_DECLS

__INLINE__ void BIC_WriteInterruptSend(BIC_REGISTER value)   
{ 
   ((BIC_Device *)BIC_DEVICE_BASE_ADDRESS)->_interrupt_send = value;
}

__INLINE__ void BIC_WriteWakeupSend(BIC_REGISTER value)
{ 
   ((BIC_Device *)BIC_DEVICE_BASE_ADDRESS)->_wakeup_send = value;
}

/* Writes to Clear External Register 0 */
__INLINE__ void BIC_WriteClearExternalRegister0(unsigned int thread_id, BIC_REGISTER value)   
{ 
    // hardware thread must be in the 0:3 range
   ((BIC_Device *)BIC_DEVICE_BASE_ADDRESS)->_clear_external_reg_0[(thread_id & 0x03)] = value;
}

/* Writes to Clear Critical Register 0 */
__INLINE__ void BIC_WriteClearCriticalRegister0(unsigned int thread_id, BIC_REGISTER value)   
{ 
   ((BIC_Device *)BIC_DEVICE_BASE_ADDRESS)->_clear_critical_reg_0[thread_id & 0x03] = value;
}

/* Writes to Clear Wakeup Register 0 */
__INLINE__ void BIC_WriteClearWakeupRegister0(unsigned int thread_id, BIC_REGISTER value)   
{ 
   ((BIC_Device *)BIC_DEVICE_BASE_ADDRESS)->_clear_wakeup_reg_0[thread_id & 0x03] = value;
}

/* Writes to Clear External Register 1 */
__INLINE__ void BIC_WriteClearExternalRegister1(unsigned int thread_id, BIC_REGISTER value)   
{ 
   ((BIC_Device *)BIC_DEVICE_BASE_ADDRESS)->_clear_external_reg_1[thread_id & 0x03] = value;
}

/* Writes to Clear Critical Register 1 */
__INLINE__ void BIC_WriteClearCriticalRegister1(unsigned int thread_id, BIC_REGISTER value)   
{ 
   ((BIC_Device *)BIC_DEVICE_BASE_ADDRESS)->_clear_critical_reg_1[thread_id & 0x03] = value;
}

/* Writes to Clear Wakeup Register 1 */
__INLINE__ void BIC_WriteClearWakeupRegister1(unsigned int thread_id, BIC_REGISTER value)   
{ 
   ((BIC_Device *)BIC_DEVICE_BASE_ADDRESS)->_clear_wakeup_reg_1[thread_id & 0x03] = value;
}

/* Writes to Set External Register 0 */
__INLINE__ void BIC_WriteSetExternalRegister0(unsigned int thread_id, BIC_REGISTER value)   
{ 
   ((BIC_Device *)BIC_DEVICE_BASE_ADDRESS)->_set_external_reg_0[thread_id & 0x03] = value;
}

/* Writes to Set Critical Register 0 */
__INLINE__ void BIC_WriteSetCriticalRegister0(unsigned int thread_id, BIC_REGISTER value)   
{ 
   ((BIC_Device *)BIC_DEVICE_BASE_ADDRESS)->_set_critical_reg_0[thread_id & 0x03] = value;
}

/* Writes to Set Wakeup Register 0 */
__INLINE__ void BIC_WriteSetWakeupRegister0(unsigned int thread_id, BIC_REGISTER value)   
{ 
   ((BIC_Device *)BIC_DEVICE_BASE_ADDRESS)->_set_wakeup_reg_0[thread_id & 0x03] = value;
}

/* Writes to Set External Register 1 */
__INLINE__ void BIC_WriteSetExternalRegister1(unsigned int thread_id, BIC_REGISTER value)   
{ 
   ((BIC_Device *)BIC_DEVICE_BASE_ADDRESS)->_set_external_reg_1[thread_id & 0x03] = value;
}

/* Writes to Set Critical Register 1 */
__INLINE__ void BIC_WriteSetCriticalRegister1(unsigned int thread_id, BIC_REGISTER value)   
{ 
   ((BIC_Device *)BIC_DEVICE_BASE_ADDRESS)->_set_critical_reg_1[thread_id & 0x03] = value;
}

/* Writes to Set Wakeup Register 1 */
__INLINE__ void BIC_WriteSetWakeupRegister1(unsigned int thread_id, BIC_REGISTER value)   
{ 
   ((BIC_Device *)BIC_DEVICE_BASE_ADDRESS)->_set_wakeup_reg_1[thread_id & 0x03] = value;
}

/* Reads the contents of Status External Register 0 */
__INLINE__ BIC_REGISTER BIC_ReadStatusExternalRegister0(unsigned int thread)
{ 
       unsigned long long int value;
       value = ((BIC_Device *)BIC_DEVICE_BASE_ADDRESS)->_clear_external_reg_0[thread & 0x03];
       return (value);
}

__INLINE__ BIC_REGISTER BIC_ReadStatusCriticalRegister0(unsigned int thread)
{ 
       unsigned long long int value;
       value = ((BIC_Device *)BIC_DEVICE_BASE_ADDRESS)->_clear_critical_reg_0[thread & 0x03];
       return (value);
}

__INLINE__ BIC_REGISTER BIC_ReadStatusWakeupRegister0(unsigned int thread)
{ 
       unsigned long long int value;
       value = ((BIC_Device *)BIC_DEVICE_BASE_ADDRESS)->_clear_wakeup_reg_0[thread & 0x03];
       return (value);
}

/* Reads the contents of Status External Register 1 */
__INLINE__ BIC_REGISTER BIC_ReadStatusExternalRegister1(unsigned int thread)
{ 
       unsigned long long int value;
       value = ((BIC_Device *)BIC_DEVICE_BASE_ADDRESS)->_clear_external_reg_1[thread & 0x03];
       return (value);
}


__INLINE__ BIC_REGISTER BIC_ReadStatusCriticalRegister1(unsigned int thread)
{ 
       unsigned long long int value;
       value = ((BIC_Device *)BIC_DEVICE_BASE_ADDRESS)->_clear_critical_reg_1[thread & 0x03];
       return (value);
}

__INLINE__ BIC_REGISTER BIC_ReadStatusWakeupRegister1(unsigned int thread)
{ 
       unsigned long long int value;
       value = ((BIC_Device *)BIC_DEVICE_BASE_ADDRESS)->_clear_wakeup_reg_1[thread & 0x03];
       return (value);
}

__INLINE__ void BIC_WriteInterruptMap(unsigned thread_id, uint64_t value)
{
    ((BIC_Device *)BIC_DEVICE_BASE_ADDRESS)->_map_interrupt[thread_id & 0x03] = value;
}

__INLINE__ BIC_REGISTER BIC_ReadInterruptMap(unsigned thread)
{
       unsigned long long int value;
       value = ((BIC_Device *)BIC_DEVICE_BASE_ADDRESS)->_map_interrupt[thread & 0x03];
       return (value);
}


__INLINE__ void BIC_InsertInterruptMap(unsigned thread, unsigned lane, unsigned interruptType)
{

  // Example: BIC_InsertInterruptMap( 0, BIC_MAP_GEA_LANE(1), BIC_CRITICAL_INTERRUPT );

  unsigned long long int value = ((BIC_Device *)BIC_DEVICE_BASE_ADDRESS)->_map_interrupt[thread & 0x03];
  value = ( value & ~_B2(lane*2 + 1, -1) ) | _B2(lane*2 + 1, interruptType );
  ((BIC_Device *)BIC_DEVICE_BASE_ADDRESS)->_map_interrupt[thread & 0x03] = value;
}


__INLINE__ BIC_REGISTER BIC_ReadExternalIntSummary(unsigned thread)
{
       unsigned long long int value;
       value = ((BIC_Device *)BIC_DEVICE_BASE_ADDRESS)->_ext_int_summary[thread & 0x03];
       return (value);
}

__INLINE__ BIC_REGISTER BIC_ReadCriticalIntSummary(unsigned thread)
{
       unsigned long long int value;
       value = ((BIC_Device *)BIC_DEVICE_BASE_ADDRESS)->_crit_int_summary[thread & 0x03];
       return (value);
}

__INLINE__ BIC_REGISTER BIC_ReadMachineCheckIntSummary(unsigned thread)
{
       unsigned long long int value;
       value = ((BIC_Device *)BIC_DEVICE_BASE_ADDRESS)->_mach_int_summary[thread & 0x03];
       return (value);
}

__INLINE__ uint64_t BIC_ReadGeaInterruptMap(uint64_t reg)
{
    uint64_t value;
    value = (&(GEA_DCR_PRIV_PTR->gea_interrupt_map0))[reg];
    return (value);
}

__INLINE__ void BIC_WriteGeaInterruptMap(uint64_t reg, uint64_t value)
{
    (&(GEA_DCR_PRIV_PTR->gea_interrupt_map0))[reg] = value;
}

__INLINE__ uint64_t BIC_ReadGeaInterruptStatus(uint64_t reg)
{
    uint64_t value;
    value = (&(GEA_DCR_PRIV_PTR->gea_interrupt_status0))[reg];
    return (value);
}

__INLINE__ uint64_t BIC_ReadGeaMappedInterruptStatus(uint64_t interruptBit, uint64_t reg)
{
    uint64_t value;
    value = (&(GEA_DCR_PRIV_PTR->gea_mapped_interrupt_status0_0))[(3*interruptBit) + reg];
    return (value);
}

__INLINE__ uint64_t BIC_ReadGeaDebugControlRegister( void )
{
    uint64_t value;
    value = GEA_DCR_PRIV_PTR->gea_debug_control;
    return (value);
}

__INLINE__ void BIC_WriteGeaDebugControlRegister(uint64_t value)
{
    GEA_DCR_PRIV_PTR->gea_debug_control = value;
}

__INLINE__ uint64_t BIC_ReadGeaInterruptState( void )
{
    uint64_t value;
    value = GEA_DCR_PRIV_PTR->gea_interrupt_state__state;
    return (value);
}

__INLINE__ void BIC_WriteGeaInterruptState(uint64_t value)
{
    GEA_DCR_PRIV_PTR->gea_interrupt_state__state = value;
}


__INLINE__ uint64_t BIC_ReadGeaInterruptControlRegisterLow( void )
{
    uint64_t value;
    value = GEA_DCR_PRIV_PTR->gea_interrupt_state_control_low;
    return (value);
}

__INLINE__ void BIC_WriteGeaInterruptControlRegisterLow(uint64_t value)
{
    GEA_DCR_PRIV_PTR->gea_interrupt_state_control_low = value;
}

__INLINE__ uint64_t BIC_ReadGeaInterruptControlRegisterHigh( void )
{
    uint64_t value;
    value = GEA_DCR_PRIV_PTR->gea_interrupt_state_control_high;
    return (value);
}

__INLINE__ void BIC_WriteGeaInterruptControlRegisterHigh(uint64_t value)
{
    GEA_DCR_PRIV_PTR->gea_interrupt_state_control_high = value;
}

__INLINE__ BIC_REGISTER BIC_ReadInputStatus(void)
{
       unsigned long long int value;
       value = ((BIC_Device *)BIC_DEVICE_BASE_ADDRESS)->_input_status;
       return (value);
}

__END_DECLS

#endif
