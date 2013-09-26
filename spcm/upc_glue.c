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

#include <spi/include/kernel/upc_hw.h>
#include <spi/include/upci/upc_p.h>

void upc_init(void)
{
    UPC_P_Disable_Unit(UPC_LOCAL_UNIT);
    UPC_P_Clear_Unit(UPC_LOCAL_UNIT);
}

void upcp_init(int unit, int mode, unsigned ctrMode)
{
    UPC_P_Init_Unit_Mode(unit, mode, ctrMode);
}
