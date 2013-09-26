/*
 * Microcode for RoQ device driver and library
 *
 * Copyright (c) 2012, IBM Corporation
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 */

#include <stdlib.h>
#include <bgq_hw_abstraction.h>
#include <transcon.h>
#include <roq_fw.h>
void invoke_RoQ_TX(void)
  {
    struct RoQ_TX_Arguments a = {
        .mTxThreadNumber = 0 ,
        .mPortId = 0 ,
        .recfifo_ptr = NULL ,
        .rem_recfifo_ptr = NULL
    };
    RoQ_TX((void *)&a) ;
  }
