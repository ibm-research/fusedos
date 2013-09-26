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

#ifndef  _KERNEL_CNK_ENVMON_IMPL_H_
#define  _KERNEL_CNK_ENVMON_IMPL_H_

#include "cnk/include/SPI_syscalls.h"

__INLINE__
uint64_t Kernel_GetRawEnvmonPowerData(int mode, uint8_t *fpga_version_p,
				      uint64_t raw_v_data[14],
				      uint64_t raw_a_data[14])
{
    return CNK_SPI_SYSCALL_4(GETRAWENVMONPOWERDATA,
			     mode, fpga_version_p, raw_v_data, raw_a_data);
}

__INLINE__
uint64_t Kernel_SetPowerConsumptionParam(uint64_t param, uint64_t value)
{
    return CNK_SPI_SYSCALL_2(SETPOWERCONSUMPTIONPARAM, param, value);
}

__INLINE__
uint64_t Kernel_ExecEnvmon2Command(uint64_t command)
{
    return CNK_SPI_SYSCALL_1(EXECENVMON2COMMAND,
                             command);
}

#endif /* _KERNEL_CNK_ENVMON_IMPL_H_ */
