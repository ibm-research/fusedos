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

#ifndef  _KERNEL_FIRMWARE_ENVMON_IMPL_H_
#define  _KERNEL_FIRMWARE_ENVMON_IMPL_H_

#include <hwi/include/bqc/A2_inlines.h>
#include <hwi/include/bqc/en_dcr.h>
#include <hwi/include/bqc/dr_arb_dcr.h>
#include <hwi/include/bqc/ddr.h>
#include <errno.h>

//********************************************************************************
// For old version of FPGA
//********************************************************************************

extern uint32_t vbuf_shift[EMON_DCA_N_DOMAINS];
#ifdef EMON_DEFINE_GLOBALS
uint32_t vbuf_shift[EMON_DCA_N_DOMAINS] = {
    1, // domain 1 = 0.8V
    0, // domain 2 = 1.4V
    1, // domain 3 = 2.5V
    0, // domain 4 = 3.3V
    0, // domain 6 = 1.5V
    1, // domain 8 = 1.0V
    0, // domain 7 = 0.9V (shift N/A)
};
#endif

#define EMON_DCA_CTRLREG ((fpga_version == EMON_DCA_FV_CTRLONRX0) ? EN_DCR(EN0_INPUT) : EN_DCR(EN5_INPUT))
#define EMON_DCA_DATAREG0 ((fpga_version == EMON_DCA_FV_CTRLONRX0) ? EN_DCR(EN1_INPUT) : EN_DCR(EN0_INPUT))
#define EMON_DCA_DATAREG1 ((fpga_version == EMON_DCA_FV_CTRLONRX0) ? EN_DCR(EN2_INPUT) : EN_DCR(EN1_INPUT))
#define EMON_DCA_DATAREG2 ((fpga_version == EMON_DCA_FV_CTRLONRX0) ? EN_DCR(EN3_INPUT) : EN_DCR(EN2_INPUT))

__INLINE__
uint64_t EMON_make_request_data(int set_disp, int magic_number, const int n_shift[], const int selector[])
{
  // build request data

#define ADD_ELEM(N) (_BGQ_SET(3, 27+(N)*10, n_shift[N]) | _BGQ_SET(5, 32+(N)*10, selector[N]))
    
  return
      _BGQ_SET(5, 17, set_disp) |
      _BGQ_SET(5, 22, magic_number) |
      ((fpga_version == EMON_DCA_FV_LONG) ? _BGQ_SET(10, 32, selector[0]) : (ADD_ELEM(0) | ADD_ELEM(1) | ADD_ELEM(2))) |
      _BGQ_SET(4, 62, (USE_VOINTBUS ? 0 : 0xA)); // select whether to use vbuf or not

#undef ADD_ELEM
}

__INLINE__
int EMON_try_sync_magic(uint64_t dcr_addr, int disp)
{
  static const int zeros[3] = {0, 0, 0};
  uint64_t magic = DCRReadPriv(dcr_addr) & 0x1f;
  int i;
  for(i = 0; i < 32; i++) { // sync 32 times
    magic = (magic+1) & 0x1f;
    uint64_t user_data = EMON_make_request_data(0, magic, zeros, zeros);
    DCRWritePriv(EN_DCR(USER), user_data);
    uint64_t en_input;
    uint64_t deadline = GetTimeBase() + 1600*100; // 100us timeout
    // printf("trying magic=%x, user_data = %016lx\n", magic, user_data);
    do {
      if (GetTimeBase() > deadline) {
	return -1;
      }
      en_input = DCRReadPriv(dcr_addr);
    } while(magic != ((en_input-disp) & 0x1f));
  }
  return 0;
}

// initialization : check FPGA version and so on
__INLINE__
int EMON_DiscoverFPGAVersion()
{ 
  int rc;

  // is this version 0?
  fpga_version = EMON_DCA_FV_CTRLONRX0; // version 0
  rc = EMON_try_sync_magic(EN_DCR(EN0_INPUT), 0);
  if (rc == 0) {
    // printf("EMON_DiscoverFPGAVersion: INFO: FPGA version is %d\n", fpga_version);
    return 0;
  }

  // is this version 1?
  fpga_version = EMON_DCA_FV_CTRLONRX5;
  rc = EMON_try_sync_magic(EN_DCR(EN5_INPUT), 0);
  if (rc == 0) {
      // printf("EMON_DiscoverFPGAVersion: INFO: FPGA version is %d\n", fpga_version);
      return 0;
  }

  // is this version 2?
  fpga_version = EMON_DCA_FV_LONG;
  rc = EMON_try_sync_magic(EN_DCR(EN5_INPUT), 2);
  if (rc == 0) {
      // printf("EMON_DiscoverFPGAVersion: INFO: FPGA version is %d\n", fpga_version); 
      return 0;
  }


  fpga_version = EMON_DCA_FV_INVALID;
  printf("EMON_DiscoverFPGAVersion: ERROR : synchronization with FPGA failed\n");
  return -1;
  
}

__INLINE__
int EMON_decide_magic_number(int magic_number)
{
  // decide magic number to use
  if (magic_number < 0) { // pick up new one
    uint64_t v = DCRReadPriv(EMON_DCA_CTRLREG);
    int en_data = EN_DCR__EN0_INPUT__EN0_INPUT_get(v);
    magic_number = (en_data + 1);
  } else {
    magic_number ++;
  }
  magic_number &= 0x1f;
  return magic_number;
}

//================================================================================
// Gets specified DCA data via envmon
// input  : data_ids[3] => specifies which DCA data to get, 3 data can be retrieved
// in+out : p_magic_number => the caller specifies the current magic number
//                 this function will use (magic_number)+1 when it gets the data
//                 if it is -1, this function will pick up a fresh magic number and use it
//                 this function will update the variable with the new magic number
// output : ret_data[3] => the DCA data got from EnvMon, each 10 bits
  
// return value : 
//    SUCCESS -> 0
//    FAIL    -> -1

__INLINE__
int EMON_GetDCAData(int data_ids[], int n_shift[], int ret_data[], int* p_magic_number, int err_count[2])
{
  // decide magic number
  int magic_number = EMON_decide_magic_number(*p_magic_number);
  uint64_t magic_goal = (fpga_version == EMON_DCA_FV_LONG) ? ((magic_number + 2) & 0x1f) : magic_number;
  int set_disp = 10; // 3.6 seconds
  // set request data
  uint64_t user_data = EMON_make_request_data(set_disp, magic_number, n_shift, data_ids);
  //write request data to be sent to EnvMon
  DCRWritePriv(EN_DCR(USER), user_data);
  // printf("user_data = %lx\n", user_data);
  // printf("sending en_dcr(user) = %016lx\n", user_data);

  // wait for data return
  uint64_t timeout = 1600 * 100; // 100us
  uint64_t start_time = GetTimeBase();
  uint64_t en_input;
  do {
    en_input = DCRReadPriv(EMON_DCA_CTRLREG);

    // timeout error
    if (GetTimeBase() > (start_time + timeout)) { 
      //printf("ERR : magic number wait timeout\n");
      return -1;
    }
  } while(magic_goal != (en_input & 0x1f));


  // retrieve data
  ret_data[0] = (int)DCRReadPriv(EMON_DCA_DATAREG0);
  ret_data[1] = (int)DCRReadPriv(EMON_DCA_DATAREG1);
  ret_data[2] = (int)DCRReadPriv(EMON_DCA_DATAREG2);
  //static int x = 0;
  //if (x == 0) {
      //uint64_t en3 = DCRReadPriv(EN_DCR(EN3_INPUT));
      //uint64_t en4 = DCRReadPriv(EN_DCR(EN4_INPUT));
      //printf("en3, en4 = %lu (%lu mA) %lu (%lu mA)\n", en3, en3*256, en4, en4*64);
      //x = 1;
  //}
  // printf("got en0_input = %03lx, 1-3 = %u %u %u\n", en0_input, ret_data[0], ret_data[1], ret_data[2]);


  int i;
  if (fpga_version == EMON_DCA_FV_LONG) {
      if (en_input & 0xE0UL) { 
	  ret_data[0] = ret_data[1] = ret_data[2] = 0;
	  err_count[EMON_DCA_DATID_TO_DCAID(data_ids[0])] ++;
      }
  } else {
      // check error on each data
      for(i = 0; i < 3; i++) {
	  if (en_input & (0x20UL << i)) {
	      err_count[EMON_DCA_DATID_TO_DCAID(data_ids[i])] ++;
	      ret_data[i] = 0;
	  }
      }
  }

  return 0;
}


__INLINE__
int EMON_DCA_make_long_data(int dat[])
{
    int x = (dat[2] << 20) + (dat[1] << 10) + dat[0];
    // printf("long data = %x %x %x %x\n", x, dat[2], dat[1], dat[0]);
    return x;
}

__INLINE__
uint64_t Kernel_GetRawEnvmonPowerData(int mode, uint8_t *fpga_version_p,
				      uint64_t raw_v_data[14],
				      uint64_t raw_a_data[14])
{
  int magic_number = -1;
  int i, j;
  int rc;
  int err_count[2]; // for each DCA

  emon_mode = mode;

  if (fpga_version == EMON_DCA_FV_INVALID) {
    rc = EMON_DiscoverFPGAVersion();
    if (rc != 0) {
      return ENODEV;
    }
  }

  if (fpga_version_p != NULL) {
    (*fpga_version_p) = fpga_version;
  }

  if ((raw_v_data == NULL) || (raw_a_data == NULL)) {
    return 0;
  }

  for(i = 0; i < 2; i++) 
    err_count[i] = 0;

  int v_data[3]; 
  int i_data[3];
  int v_data_ids[3];
  int i_data_ids[3];
  
  int domain_id;

  if (fpga_version == EMON_DCA_FV_LONG) {
      for(i = 0; i < 14; i++) { // 7 domains * 2 DCAs
	  v_data_ids[0] = EMON_DCA_DATID_VO_INTBUSPX(i);
	  i_data_ids[0] = EMON_DCA_DATID_IMONX(i);
	  v_data_ids[1] = 0;
	  i_data_ids[1] = 0;
	  v_data_ids[2] = 0;
	  i_data_ids[2] = 0;

	  rc = EMON_GetDCAData(v_data_ids, NULL, v_data, &magic_number, err_count);
	  if (rc) return EIO;
	  rc = EMON_GetDCAData(i_data_ids, NULL, i_data, &magic_number, err_count);
	  if (rc) return EIO;

	  raw_v_data[i] = EMON_DCA_make_long_data(v_data);
	  raw_a_data[i] = EMON_DCA_make_long_data(i_data);
      }
  } else {
      for(i = 0; i < 12; i+=3) { // 6 domains * 2 DCAs,  get 3 per each envmon access
	  int v_shift[3];
	  int i_shift[3];
	  
	  for(j = 0; j < 3; j++) {
	      domain_id = (i+j) >> 1;
	      v_data_ids[j] = EMON_DCA_DATID_VO_INTBUSPX(i+j);
	      i_data_ids[j] = EMON_DCA_DATID_IMONX(i+j);
	      v_shift[j] = USE_VOINTBUS ? 0 : vbuf_shift[domain_id];
	      i_shift[j] = 0;
	  }
	  
	  rc = EMON_GetDCAData(v_data_ids, v_shift, v_data, &magic_number, err_count);
	  if (rc) return EIO;
	  rc = EMON_GetDCAData(i_data_ids, i_shift, i_data, &magic_number, err_count);
	  if (rc) return EIO;
	  
	  for(j = 0; j < 3; j++) {
	      raw_v_data[i+j] = v_data[j];
	      raw_a_data[i+j] = i_data[j];
	  }
      }
      for(i = 12; i < 14; i++) {
	  raw_v_data[i+j] = raw_a_data[i+j] = 0;
      }
  }

  for(i = 0; i < 2; i++) {
    if ((err_count[i] != 0) && (err_count[i] != 12)) { // must be all pass or all fail
      return EIO;
    }
  }

  return 0;
}

__INLINE__
uint64_t Kernel_SetPowerConsumptionParam(uint64_t param, uint64_t value)
{
    uint64_t rc = EINVAL;

    switch (param)
    {
    case DDR_IDLES_PER_ACCESS:
	if (value <= 126)
	{
	    uint64_t mcamisc0, mcamisc1;
	    mcamisc0 = DCRReadPriv(_DDR_MC_MCAMISC(0));
	    mcamisc1 = DCRReadPriv(_DDR_MC_MCAMISC(1));
	    _BGQ_INSERT(7, 43, mcamisc0, value);
	    _BGQ_INSERT(7, 43, mcamisc1, value);
            DCRWritePriv(_DDR_MC_MCAMISC(0), mcamisc0);
            DCRWritePriv(_DDR_MC_MCAMISC(1), mcamisc1);
	    rc = 0;
	}
	break;
    default:
	break;
    }
    return rc;
}

//================================================================================
// Executes a command for Envmon2 FPGA&DCA interface for power measurement

__INLINE__
uint64_t Kernel_ExecEnvmon2Command(uint64_t command)
{
    DCRWritePriv(EN_DCR(USER), command & 0x0007FFFFFFFFFFFFUL); // mask unused bits for safety
    return 0;
}


#endif /* _KERNEL_FIRMWARE_ENVMON_IMPL_H_ */
