/*
 * Microcode for RoQ device driver and library
 *
 * Copyright (c) 2008-2012, IBM Corporation
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 */

#define __KERNEL__
#include <hwi/include/common/bgq_alignment.h>
#include <hwi/include/bqc/A2_inlines.h>
#include <hwi/include/bqc/A2_core.h>
#include <hwi/include/bqc/testint_dcr.h>
#include <hwi/include/bqc/BIC.h>
#include <hwi/include/bqc/BIC_inlines.h>
#include <hwi/include/bqc/pe_dcr.h>

#include <firmware/include/mailbox.h>
#include <firmware/include/personality.h>
#include <firmware/include/Firmware_Interface.h>
#include <firmware/include/BeDram_inlines.h>
#undef __KERNEL__

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include <spi/include/mu/Addressing.h>
#ifndef MUSPI_assert
 #define MUSPI_assert
#endif
#include <spi/include/mu/Addressing_inlines.h>
#include <spi/include/kernel/MU.h>
#include <spi/include/mu/Descriptor.h>
#include <spi/include/mu/Descriptor_inlines.h>
#include <spi/include/mu/InjFifo.h>
#include <spi/include/mu/RecFifo.h>
#include <transcon.h>

#include <roq_kernel.h>
#include <roq_debug.h>

//#include "Kernel.h"
volatile Firmware_Interface_t *_firmware_domain = 0 ;
//Firmware_DomainDescriptor_t _descriptor_domain;
//extern NodeState_t NodeState ;

//#define DD()        _descriptor_domain
//#define DD() NodeState.DomainDesc
//
//__C_LINKAGE uint64_t
//domain_physddr_start(void)
//{
//  return DD().ddrOrigin;
//}
