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

#ifndef   _TESTINT_DCR__H_ // Prevent multiple inclusion
#define   _TESTINT_DCR__H_
  
__BEGIN_DECLS
  
#include <hwi/include/common/bgq_bitnumbers.h>
#include <hwi/include/bqc/dcr_support.h>
  
#define TESTINT_DCR_base    (0x0C8000)  // Base address for all instances
#define TESTINT_DCR_size    (0x2002)    // Size of instance (largest offset + 1 )
  
#define TESTINT_DCR(reg) _DCR_REG(TESTINT_DCR, reg)
  
// Register:
//   Name:        jtag_idcode
//   Type:        Read Only
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: JTAG_IDCODE register.
#define TESTINT_DCR__JTAG_IDCODE_offset  (0x0000) // Offset of register in instance 
#define TESTINT_DCR__JTAG_IDCODE_reset  UNSIGNED64(0x0000000000000000)

//   Field:       bits
//   Description: JTAG_IDCODE bits, according to IEEE 1149.1 specification (R/O).                            <p>Value for BQC DD1.0: x&lsquo;0000_0000_0900_3049&rsquo;</p>
#define TESTINT_DCR__JTAG_IDCODE__BITS_width        64
#define TESTINT_DCR__JTAG_IDCODE__BITS_position     63
#define TESTINT_DCR__JTAG_IDCODE__BITS_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__JTAG_IDCODE__BITS_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__JTAG_IDCODE__BITS_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        clocks
//   Type:        Read/Write External
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: Clock enables register.                            <p>Notes:   <ul>     <li>Only a read from DCR x0C8030 will give results. Reads from DCR x0C8031-3 will return 0's for all bits.</li>     <li>Write to DCR x0C8030 will write these bits</li>     <li>Write to DCR x0C8031 will be bit-wise OR-ed with the existing contents -- this is an &ldquo;atomic write-set&rdquo; operation</li>     <li>Write to DCR x0C8032 will be bit-wise NOT AND-ed with the existing contents -- this is an &ldquo;atomic write-clear&rdquo; operation</li>   </ul>   </p> 
#define TESTINT_DCR__CLOCKS_offset  (0x0030) // Offset of register in instance 
#define TESTINT_DCR__CLOCKS_range  (0x4) // Range of external address space
#define TESTINT_DCR__CLOCKS_reset  UNSIGNED64(0x0000000000000000)

//   Field:       c
//   Description: Unit level clock-enable bits
#define TESTINT_DCR__CLOCKS__C_width        64
#define TESTINT_DCR__CLOCKS__C_position     63
#define TESTINT_DCR__CLOCKS__C_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__CLOCKS__C_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__CLOCKS__C_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        config0
//   Type:        Read/Write External
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: Config0 register.                            <p>Notes:   <ul>     <li>Only a read from DCR x0C8040 will give results. Reads from DCR x0C8041-3 will only return 0's for all bits.</li>     <li>Write to DCR x0C8040 will write these bits</li>     <li>Write to DCR x0C8041 will be bit-wise OR-ed with the existing contents -- this is an &ldquo;atomic write-set&rdquo; operation</li>     <li>Write to DCR x0C8042 will be bit-wise NOT AND-ed with the existing contents -- this is an &ldquo;atomic write-clear&rdquo; operation</li>   </ul>   </p> 
#define TESTINT_DCR__CONFIG0_offset  (0x0040) // Offset of register in instance 
#define TESTINT_DCR__CONFIG0_range  (0x4) // Range of external address space
#define TESTINT_DCR__CONFIG0_reset  UNSIGNED64(0x0000000000000000)

//   Field:       C0
//   Description: R/W  Config0 bits
#define TESTINT_DCR__CONFIG0__C0_width        64
#define TESTINT_DCR__CONFIG0__C0_position     63
#define TESTINT_DCR__CONFIG0__C0_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__CONFIG0__C0_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__CONFIG0__C0_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        config1
//   Type:        Read/Write External
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: Config1 register.                            <p>Notes:   <ul>     <li>Only a read from DCR x0C8044 will give results. Reads from DCR x0C8045-7 will only return 0's for all bits.</li>     <li>Write to DCR x0C8044 will write these bits</li>     <li>Write to DCR x0C8045 will be bit-wise OR-ed with the existing contents -- this is an &ldquo;atomic write-set&rdquo; operation</li>     <li>Write to DCR x0C8046 will be bit-wise NOT AND-ed with the existing contents -- this is an &ldquo;atomic write-clear&rdquo; operation</li>   </ul>   </p> 
#define TESTINT_DCR__CONFIG1_offset  (0x0044) // Offset of register in instance 
#define TESTINT_DCR__CONFIG1_range  (0x4) // Range of external address space
#define TESTINT_DCR__CONFIG1_reset  UNSIGNED64(0x0000000000000000)

//   Field:       C1
//   Description: R/W  Config1 bits
#define TESTINT_DCR__CONFIG1__C1_width        64
#define TESTINT_DCR__CONFIG1__C1_position     63
#define TESTINT_DCR__CONFIG1__C1_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__CONFIG1__C1_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__CONFIG1__C1_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        Config0a
//   Type:        Read/Write External
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: Alternate Config0 register.
#define TESTINT_DCR__CONFIG0A_offset  (0x0048) // Offset of register in instance 
#define TESTINT_DCR__CONFIG0A_range  (0x1) // Range of external address space
#define TESTINT_DCR__CONFIG0A_reset  UNSIGNED64(0x0000000000000000)

//   Field:       bits
//   Description: Alternate configuration 0 to 63. Active when DCR x0C8240 counter (24-43) > 0.
#define TESTINT_DCR__CONFIG0A__BITS_width        64
#define TESTINT_DCR__CONFIG0A__BITS_position     63
#define TESTINT_DCR__CONFIG0A__BITS_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__CONFIG0A__BITS_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__CONFIG0A__BITS_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        Config1a
//   Type:        Read/Write External
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: Alternate Config1 register.
#define TESTINT_DCR__CONFIG1A_offset  (0x0049) // Offset of register in instance 
#define TESTINT_DCR__CONFIG1A_range  (0x1) // Range of external address space
#define TESTINT_DCR__CONFIG1A_reset  UNSIGNED64(0x0000000000000000)

//   Field:       bits
//   Description: Alternate configuration 64 to 127. Active when DCR x0C8240 counter (24-43) > 0.
#define TESTINT_DCR__CONFIG1A__BITS_width        64
#define TESTINT_DCR__CONFIG1A__BITS_position     63
#define TESTINT_DCR__CONFIG1A__BITS_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__CONFIG1A__BITS_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__CONFIG1A__BITS_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        thread_active0
//   Type:        Read/Write External
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: Config_Threads0 register.   <ul>     <li>Only a read from DCR x0C8050 will give results. Reads from DCR x0C8051-3 will only return 0's for all bits.</li>     <li>Write to DCR x0C8050 will write these bits</li>     <li>Write to DCR x0C8051 will be bit-wise OR-ed with the existing contents -- this is an &ldquo;atomic write-set&rdquo; operation</li>     <li>Write to DCR x0C8052 will be bit-wise NOT AND-ed with the existing contents -- this is an &ldquo;atomic write-clear&rdquo; operation</li>   </ul> 
#define TESTINT_DCR__THREAD_ACTIVE0_offset  (0x0050) // Offset of register in instance 
#define TESTINT_DCR__THREAD_ACTIVE0_range  (0x4) // Range of external address space
#define TESTINT_DCR__THREAD_ACTIVE0_reset  UNSIGNED64(0x0000000000000000)

//   Field:       C0
//   Description: <table border=0>                          <thead><tr><th>bit</th> <th>description</th></tr></thead>                          <tbody>                            <tr><td>0</td> <td>Core 0 thread 0 enable</td></tr>                            <tr><td>1</td> <td>Core 0 thread 1 enable</td></tr>                            <tr><td>2</td> <td>Core 0 thread 2 enable</td></tr>                            <tr><td>3</td> <td>Core 0 thread 3 enable</td></tr>                            <tr><td>4</td> <td>Core 1 thread 0 enable</td></tr>                            <tr><td>5</td> <td>Core 1 thread 1 enable</td></tr>                            <tr><td>...</td> <td>...</td></tr>                            <tr><td>60</td> <td>Core 15 thread 0 enable</td></tr>                            <tr><td>61</td> <td>Core 15 thread 1 enable</td></tr>                            <tr><td>62</td> <td>Core 15 thread 2 enable</td></tr>                            <tr><td>63</td> <td>Core 15 thread 3 enable</td></tr>                          </tbody>                        </table>
#define TESTINT_DCR__THREAD_ACTIVE0__C0_width        64
#define TESTINT_DCR__THREAD_ACTIVE0__C0_position     63
#define TESTINT_DCR__THREAD_ACTIVE0__C0_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__THREAD_ACTIVE0__C0_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__THREAD_ACTIVE0__C0_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        thread_active1
//   Type:        Read/Write External
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: Config_Threads1 register.
#define TESTINT_DCR__THREAD_ACTIVE1_offset  (0x0054) // Offset of register in instance 
#define TESTINT_DCR__THREAD_ACTIVE1_range  (0x4) // Range of external address space
#define TESTINT_DCR__THREAD_ACTIVE1_reset  UNSIGNED64(0x0000000000000000)

//   Field:       C1
//   Description: <table border=0>                           <thead><tr><th>bit</th> <th>description</th></tr></thead>                           <tbody>                             <tr><td>0</td> <td>Core 16 thread 0 enable</td></tr>                             <tr><td>1</td> <td>Core 16 thread 1 enable</td></tr>                             <tr><td>2</td> <td>Core 16 thread 2 enable</td></tr>                             <tr><td>3</td> <td>Core 16 thread 3 enable</td></tr>                           </tbody>                         </table> 
#define TESTINT_DCR__THREAD_ACTIVE1__C1_width        4
#define TESTINT_DCR__THREAD_ACTIVE1__C1_position     3
#define TESTINT_DCR__THREAD_ACTIVE1__C1_get(x)       _BGQ_GET(4,3,x)
#define TESTINT_DCR__THREAD_ACTIVE1__C1_set(v)       _BGQ_SET(4,3,v)
#define TESTINT_DCR__THREAD_ACTIVE1__C1_insert(x,v)  _BGQ_INSERT(4,3,x,v)

  
// Register:
//   Name:        thread_active0a
//   Type:        Read/Write External
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: Alternate Config_Threads0 register.
#define TESTINT_DCR__THREAD_ACTIVE0A_offset  (0x0058) // Offset of register in instance 
#define TESTINT_DCR__THREAD_ACTIVE0A_range  (0x1) // Range of external address space
#define TESTINT_DCR__THREAD_ACTIVE0A_reset  UNSIGNED64(0x0000000000000000)

//   Field:       C0a
//   Description: Alternate Config_Threads0 (0 to 63). Active when DCR x0C8240 counter (44-63) > 0.
#define TESTINT_DCR__THREAD_ACTIVE0A__C0A_width        64
#define TESTINT_DCR__THREAD_ACTIVE0A__C0A_position     63
#define TESTINT_DCR__THREAD_ACTIVE0A__C0A_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__THREAD_ACTIVE0A__C0A_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__THREAD_ACTIVE0A__C0A_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        thread_active1a
//   Type:        Read/Write External
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: Alternate Config_Threads1 register. 
#define TESTINT_DCR__THREAD_ACTIVE1A_offset  (0x0059) // Offset of register in instance 
#define TESTINT_DCR__THREAD_ACTIVE1A_range  (0x1) // Range of external address space
#define TESTINT_DCR__THREAD_ACTIVE1A_reset  UNSIGNED64(0x0000000000000000)

//   Field:       C1a
//   Description: Alternate Config_Threads1 (0 to 3). Active when DCR x0C8240 counter (44-63) > 0.
#define TESTINT_DCR__THREAD_ACTIVE1A__C1A_width        4
#define TESTINT_DCR__THREAD_ACTIVE1A__C1A_position     3
#define TESTINT_DCR__THREAD_ACTIVE1A__C1A_get(x)       _BGQ_GET(4,3,x)
#define TESTINT_DCR__THREAD_ACTIVE1A__C1A_set(v)       _BGQ_SET(4,3,v)
#define TESTINT_DCR__THREAD_ACTIVE1A__C1A_insert(x,v)  _BGQ_INSERT(4,3,x,v)

  
// Register:
//   Name:        thread_active0_rb
//   Type:        Read Only
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: Config_Threads0_rb readback register.                              <p>Note on read-back:                              If bit CoreX threadY active is read as '1', the clocks for CoreX are active, resets for CoreX are released                              (i.e., its enable is high), and the tc_ac_pm_thread_stop(Y) signal for thread Y on core X is '0'.                              In all other cases, the CoreX threadY active bit reads as a '0'                              </p>                              
#define TESTINT_DCR__THREAD_ACTIVE0_RB_offset  (0x005A) // Offset of register in instance 
#define TESTINT_DCR__THREAD_ACTIVE0_RB_reset  UNSIGNED64(0x0000000000000000)

//   Field:       C0
//   Description: <table border=0>                          <thead><tr><th>bit</th> <th>description</th></tr></thead>                          <tbody>                            <tr><td>0</td> <td>Core 0 thread 0 is active</td></tr>                            <tr><td>1</td> <td>Core 0 thread 1 is active</td></tr>                            <tr><td>2</td> <td>Core 0 thread 2 is active</td></tr>                            <tr><td>3</td> <td>Core 0 thread 3 is active</td></tr>                            <tr><td>4</td> <td>Core 1 thread 0 is active</td></tr>                            <tr><td>5</td> <td>Core 1 thread 1 is active</td></tr>                            <tr><td>...</td> <td>...</td></tr>                            <tr><td>60</td> <td>Core 15 thread 0 is active</td></tr>                            <tr><td>61</td> <td>Core 15 thread 1 is active</td></tr>                            <tr><td>62</td> <td>Core 15 thread 2 is active</td></tr>                            <tr><td>63</td> <td>Core 15 thread 3 is active</td></tr>                          </tbody>                        </table>
#define TESTINT_DCR__THREAD_ACTIVE0_RB__C0_width        64
#define TESTINT_DCR__THREAD_ACTIVE0_RB__C0_position     63
#define TESTINT_DCR__THREAD_ACTIVE0_RB__C0_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__THREAD_ACTIVE0_RB__C0_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__THREAD_ACTIVE0_RB__C0_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        thread_active1_rb
//   Type:        Read Only
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: Config_Threads1_rb readback register.                              <p>Note: See Config_Threads0_rb register for explanation. </p>                              
#define TESTINT_DCR__THREAD_ACTIVE1_RB_offset  (0x005B) // Offset of register in instance 
#define TESTINT_DCR__THREAD_ACTIVE1_RB_reset  UNSIGNED64(0x0000000000000000)

//   Field:       C1
//   Description: <table border=0>                           <thead><tr><th>bit</th> <th>description</th></tr></thead>                           <tbody>                             <tr><td>0</td> <td>Core 16 thread 0 active</td></tr>                             <tr><td>1</td> <td>Core 16 thread 1 active</td></tr>                             <tr><td>2</td> <td>Core 16 thread 2 active</td></tr>                             <tr><td>3</td> <td>Core 16 thread 3 active</td></tr>                           </tbody>                         </table> 
#define TESTINT_DCR__THREAD_ACTIVE1_RB__C1_width        4
#define TESTINT_DCR__THREAD_ACTIVE1_RB__C1_position     3
#define TESTINT_DCR__THREAD_ACTIVE1_RB__C1_get(x)       _BGQ_GET(4,3,x)
#define TESTINT_DCR__THREAD_ACTIVE1_RB__C1_set(v)       _BGQ_SET(4,3,v)
#define TESTINT_DCR__THREAD_ACTIVE1_RB__C1_insert(x,v)  _BGQ_INSERT(4,3,x,v)

  
// Register:
//   Name:        glob_att
//   Type:        Read/Write External
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: Glob_att register.                            <p>Note: Only a read from DCR x0C80C0 will give results. Reads from DCR x0C80C1-3 will only return 0's for all bits.</p>                            
#define TESTINT_DCR__GLOB_ATT_offset  (0x00C0) // Offset of register in instance 
#define TESTINT_DCR__GLOB_ATT_range  (0x4) // Range of external address space
#define TESTINT_DCR__GLOB_ATT_reset  UNSIGNED64(0x0000000000000000)

//   Field:       lo_bits
//   Description: <table border=0>                          <thead><tr><th>bit</th> <th>description</th></tr></thead>                          <tbody>                            <tr><td>32-48</td> <td>Reserved</td></tr>                            <tr><td>49</td>    <td>Warning: Over-Temperature</td></tr>                            <tr><td>50</td>    <td>Critical Over Temperature</td></tr>                            <tr><td>51</td>    <td>R/O Error accessing the Devbus via JTAG</td></tr>                            <tr><td>52</td>    <td>R/O Error accessing the DCR via JTAG</td></tr>                            <tr><td>53</td>    <td>R/O Testint parity error bit</td></tr>                            <tr><td>54-59</td> <td>R/W 6 bits for user defined glob_att. For these bits only:                                                  <ul>                                                    <li>Write to DCR x0C80C0 will write these bits</li>                                                    <li>Write to DCR x0C80C1 will be bit-wise OR-ed with the existing contents -- this is an &ldquo;atomic write-set&rdquo; operation</li>                                                    <li>Write to DCR x0C80C2 will be bit-wise NOT AND-ed with the existing contents -- this is an &ldquo;atomic write-clear&rdquo; operation</li>                                                  </ul></td></tr>                            <tr><td>60-63</td> <td>R/O Input from Mailbox_Reg comparators</td></tr>                          </tbody>                        </table>
#define TESTINT_DCR__GLOB_ATT__LO_BITS_width        32
#define TESTINT_DCR__GLOB_ATT__LO_BITS_position     63
#define TESTINT_DCR__GLOB_ATT__LO_BITS_get(x)       _BGQ_GET(32,63,x)
#define TESTINT_DCR__GLOB_ATT__LO_BITS_set(v)       _BGQ_SET(32,63,v)
#define TESTINT_DCR__GLOB_ATT__LO_BITS_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        glob_att_mask
//   Type:        Read Only
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: Glob_att_mask register
#define TESTINT_DCR__GLOB_ATT_MASK_offset  (0x00D0) // Offset of register in instance 
#define TESTINT_DCR__GLOB_ATT_MASK_reset  UNSIGNED64(0x0000000000000000)

//   Field:       hi_bits
//   Description: R/O Glob_att_mask TDR bits(32 to 63)
#define TESTINT_DCR__GLOB_ATT_MASK__HI_BITS_width        32
#define TESTINT_DCR__GLOB_ATT_MASK__HI_BITS_position     31
#define TESTINT_DCR__GLOB_ATT_MASK__HI_BITS_get(x)       _BGQ_GET(32,31,x)
#define TESTINT_DCR__GLOB_ATT_MASK__HI_BITS_set(v)       _BGQ_SET(32,31,v)
#define TESTINT_DCR__GLOB_ATT_MASK__HI_BITS_insert(x,v)  _BGQ_INSERT(32,31,x,v)

//   Field:       lo_bits
//   Description: R/O masked (i.e., active) Glob_att bits(32 to 63)
#define TESTINT_DCR__GLOB_ATT_MASK__LO_BITS_width        32
#define TESTINT_DCR__GLOB_ATT_MASK__LO_BITS_position     63
#define TESTINT_DCR__GLOB_ATT_MASK__LO_BITS_get(x)       _BGQ_GET(32,63,x)
#define TESTINT_DCR__GLOB_ATT_MASK__LO_BITS_set(v)       _BGQ_SET(32,63,v)
#define TESTINT_DCR__GLOB_ATT_MASK__LO_BITS_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        gp1_reg
//   Type:        Read Only
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: GP1_REG register
#define TESTINT_DCR__GP1_REG_offset  (0x0100) // Offset of register in instance 
#define TESTINT_DCR__GP1_REG_reset  UNSIGNED64(0x0000000000000000)

//   Field:       r1
//   Description: RO GP1_REG
#define TESTINT_DCR__GP1_REG__R1_width        64
#define TESTINT_DCR__GP1_REG__R1_position     63
#define TESTINT_DCR__GP1_REG__R1_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__GP1_REG__R1_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__GP1_REG__R1_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        gp2_reg
//   Type:        Read/Write External
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: GP2_REG register.                            <p>Note: Only a read from DCR x0C8110 will give results. Reads from DCR x0C8111-3 will only return 0's for all bits.Verify with JTAG doc that this is correct.</p>                            
#define TESTINT_DCR__GP2_REG_offset  (0x0110) // Offset of register in instance 
#define TESTINT_DCR__GP2_REG_range  (0x4) // Range of external address space
#define TESTINT_DCR__GP2_REG_reset  UNSIGNED64(0x0000000000000000)

//   Field:       r2
//   Description: RWE GP2_REG register
#define TESTINT_DCR__GP2_REG__R2_width        64
#define TESTINT_DCR__GP2_REG__R2_position     63
#define TESTINT_DCR__GP2_REG__R2_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__GP2_REG__R2_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__GP2_REG__R2_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        gp3_reg
//   Type:        Read/Write External
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: GP3_REG register.                            <p>Notes:   <ul>     <li>A read from DCR x0C8120 will give results. </li>     <li>Reads from others will only return 0's for all bits.Verify with JTAG doc that this is correct.</li>   </ul>   </p> 
#define TESTINT_DCR__GP3_REG_offset  (0x0120) // Offset of register in instance 
#define TESTINT_DCR__GP3_REG_range  (0x1) // Range of external address space
#define TESTINT_DCR__GP3_REG_reset  UNSIGNED64(0x0000000000000000)

//   Field:       r3
//   Description: RO GP3_REG
#define TESTINT_DCR__GP3_REG__R3_width        64
#define TESTINT_DCR__GP3_REG__R3_position     63
#define TESTINT_DCR__GP3_REG__R3_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__GP3_REG__R3_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__GP3_REG__R3_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        mailbox_reg0
//   Type:        Read/Write External
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: Mailbox0 register.                            <p>Notes:   <ul>     <li>Only a read from DCR x0C8160 will give results. Reads from DCR x0C8161-3 will only return 0's for all bits.</li>     <li>Write to DCR x0C8160 will write these bits</li>     <li>Write to DCR x0C8161 will be bit-wise OR-ed with the existing contents -- this is an &ldquo;atomic write-set&rdquo; operation</li>     <li>Write to DCR x0C8162 will be bit-wise NOT AND-ed with the existing contents -- this is an &ldquo;atomic write-clear&rdquo; operation</li>   </ul>   </p> 
#define TESTINT_DCR__MAILBOX_REG0_offset  (0x0160) // Offset of register in instance 
#define TESTINT_DCR__MAILBOX_REG0_range  (0x4) // Range of external address space
#define TESTINT_DCR__MAILBOX_REG0_reset  UNSIGNED64(0x0000000000000000)

//   Field:       U0
//   Description: R/W  mailbox0 bits
#define TESTINT_DCR__MAILBOX_REG0__U0_width        64
#define TESTINT_DCR__MAILBOX_REG0__U0_position     63
#define TESTINT_DCR__MAILBOX_REG0__U0_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__MAILBOX_REG0__U0_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__MAILBOX_REG0__U0_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        mailbox_reg1
//   Type:        Read/Write External
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: Mailbox1 register.                            <p>Notes:   <ul>     <li>Only a read from DCR x0C8170 will give results. Reads from DCR x0C8171-3 will only return 0's for all bits.</li>     <li>Write to DCR x0C8170 will write these bits</li>     <li>Write to DCR x0C8171 will be bit-wise OR-ed with the existing contents -- this is an &ldquo;atomic write-set&rdquo; operation</li>     <li>Write to DCR x0C8172 will be bit-wise NOT AND-ed with the existing contents -- this is an &ldquo;atomic write-clear&rdquo; operation</li>   </ul>   </p> 
#define TESTINT_DCR__MAILBOX_REG1_offset  (0x0170) // Offset of register in instance 
#define TESTINT_DCR__MAILBOX_REG1_range  (0x4) // Range of external address space
#define TESTINT_DCR__MAILBOX_REG1_reset  UNSIGNED64(0x0000000000000000)

//   Field:       U0
//   Description: R/W  mailbox1 bits
#define TESTINT_DCR__MAILBOX_REG1__U0_width        64
#define TESTINT_DCR__MAILBOX_REG1__U0_position     63
#define TESTINT_DCR__MAILBOX_REG1__U0_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__MAILBOX_REG1__U0_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__MAILBOX_REG1__U0_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        mailbox_reg2
//   Type:        Read/Write External
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: Mailbox2 register.                            <p>Notes:   <ul>     <li>Only a read from DCR x0C8180 will give results. Reads from DCR x0C8181-3 will only return 0's for all bits.</li>     <li>Write to DCR x0C8180 will write these bits</li>     <li>Write to DCR x0C8181 will be bit-wise OR-ed with the existing contents -- this is an &ldquo;atomic write-set&rdquo; operation</li>     <li>Write to DCR x0C8182 will be bit-wise NOT AND-ed with the existing contents -- this is an &ldquo;atomic write-clear&rdquo; operation</li>   </ul>   </p> 
#define TESTINT_DCR__MAILBOX_REG2_offset  (0x0180) // Offset of register in instance 
#define TESTINT_DCR__MAILBOX_REG2_range  (0x4) // Range of external address space
#define TESTINT_DCR__MAILBOX_REG2_reset  UNSIGNED64(0x0000000000000000)

//   Field:       U0
//   Description: R/W  mailbox2 bits
#define TESTINT_DCR__MAILBOX_REG2__U0_width        64
#define TESTINT_DCR__MAILBOX_REG2__U0_position     63
#define TESTINT_DCR__MAILBOX_REG2__U0_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__MAILBOX_REG2__U0_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__MAILBOX_REG2__U0_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        mailbox_reg3
//   Type:        Read/Write External
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: Mailbox3 register.                            <p>Notes:   <ul>     <li>Only a read from DCR x0C8190 will give results. Reads from DCR x0C8191-3 will only return 0's for all bits.</li>     <li>Write to DCR x0C8190 will write these bits</li>     <li>Write to DCR x0C8191 will be bit-wise OR-ed with the existing contents -- this is an &ldquo;atomic write-set&rdquo; operation</li>     <li>Write to DCR x0C8192 will be bit-wise NOT AND-ed with the existing contents -- this is an &ldquo;atomic write-clear&rdquo; operation</li>   </ul>   </p> 
#define TESTINT_DCR__MAILBOX_REG3_offset  (0x0190) // Offset of register in instance 
#define TESTINT_DCR__MAILBOX_REG3_range  (0x4) // Range of external address space
#define TESTINT_DCR__MAILBOX_REG3_reset  UNSIGNED64(0x0000000000000000)

//   Field:       U0
//   Description: R/W  mailbox3 bits
#define TESTINT_DCR__MAILBOX_REG3__U0_width        64
#define TESTINT_DCR__MAILBOX_REG3__U0_position     63
#define TESTINT_DCR__MAILBOX_REG3__U0_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__MAILBOX_REG3__U0_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__MAILBOX_REG3__U0_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        parity
//   Type:        Read/Write External
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: Parity register.
#define TESTINT_DCR__PARITY_offset  (0x01A0) // Offset of register in instance 
#define TESTINT_DCR__PARITY_range  (0x1) // Range of external address space
#define TESTINT_DCR__PARITY_reset  UNSIGNED64(0x0000000000000000)

//   Field:       bits
//   Description: Parity error bits from TI.  Elaborate table
#define TESTINT_DCR__PARITY__BITS_width        64
#define TESTINT_DCR__PARITY__BITS_position     63
#define TESTINT_DCR__PARITY__BITS_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__PARITY__BITS_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__PARITY__BITS_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        redund_ctrl
//   Type:        Read Only
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: Redundancy control
#define TESTINT_DCR__REDUND_CTRL_offset  (0x01C0) // Offset of register in instance 
#define TESTINT_DCR__REDUND_CTRL_reset  UNSIGNED64(0x0000000000000000)

//   Field:       bits
//   Description: Reads Redundancy control bits(0 to 63)
#define TESTINT_DCR__REDUND_CTRL__BITS_width        64
#define TESTINT_DCR__REDUND_CTRL__BITS_position     63
#define TESTINT_DCR__REDUND_CTRL__BITS_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__REDUND_CTRL__BITS_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__REDUND_CTRL__BITS_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        slice_sel_ctrl
//   Type:        Read Only
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: L2 Slice Select Control
#define TESTINT_DCR__SLICE_SEL_CTRL_offset  (0x01D0) // Offset of register in instance 
#define TESTINT_DCR__SLICE_SEL_CTRL_reset  UNSIGNED64(0x0000000000000000)

//   Field:       bits
//   Description: Reads slice_sel_ctrl bits(0 to 63)
#define TESTINT_DCR__SLICE_SEL_CTRL__BITS_width        64
#define TESTINT_DCR__SLICE_SEL_CTRL__BITS_position     63
#define TESTINT_DCR__SLICE_SEL_CTRL__BITS_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__SLICE_SEL_CTRL__BITS_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__SLICE_SEL_CTRL__BITS_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        usercode0
//   Type:        Read/Write External
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: Usercode0 register.                            <p>Notes:   <ul>     <li>Only a read from DCR x0C8200 will give results. Reads from DCR x0C8201-3 will only return 0's for all bits.</li>     <li>Write to DCR x0C8200 will write these bits</li>     <li>Write to DCR x0C8201 will be bit-wise OR-ed with the existing contents -- this is an &ldquo;atomic write-set&rdquo; operation</li>     <li>Write to DCR x0C8202 will be bit-wise NOT AND-ed with the existing contents -- this is an &ldquo;atomic write-clear&rdquo; operation</li>   </ul>   </p> 
#define TESTINT_DCR__USERCODE0_offset  (0x0200) // Offset of register in instance 
#define TESTINT_DCR__USERCODE0_range  (0x4) // Range of external address space
#define TESTINT_DCR__USERCODE0_reset  UNSIGNED64(0x0000000000000000)

//   Field:       U0
//   Description: R/W  usercode0 bits
#define TESTINT_DCR__USERCODE0__U0_width        64
#define TESTINT_DCR__USERCODE0__U0_position     63
#define TESTINT_DCR__USERCODE0__U0_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__USERCODE0__U0_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__USERCODE0__U0_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        usercode1
//   Type:        Read/Write External
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: Usercode1 register.                            <p>Notes:   <ul>     <li>Only a read from DCR x0C8210 will give results. Reads from DCR x0C8211-3 will only return 0's for all bits.</li>     <li>Write to DCR x0C8210 will write these bits</li>     <li>Write to DCR x0C8211 will be bit-wise OR-ed with the existing contents -- this is an &ldquo;atomic write-set&rdquo; operation</li>     <li>Write to DCR x0C8212 will be bit-wise NOT AND-ed with the existing contents -- this is an &ldquo;atomic write-clear&rdquo; operation</li>   </ul>   </p> 
#define TESTINT_DCR__USERCODE1_offset  (0x0210) // Offset of register in instance 
#define TESTINT_DCR__USERCODE1_range  (0x4) // Range of external address space
#define TESTINT_DCR__USERCODE1_reset  UNSIGNED64(0x0000000000000000)

//   Field:       U0
//   Description: R/W  usercode1 bits
#define TESTINT_DCR__USERCODE1__U0_width        64
#define TESTINT_DCR__USERCODE1__U0_position     63
#define TESTINT_DCR__USERCODE1__U0_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__USERCODE1__U0_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__USERCODE1__U0_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        usercode2
//   Type:        Read/Write External
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: Usercode2 register.                            <p>Notes:   <ul>     <li>Only a read from DCR x0C8220 will give results. Reads from DCR x0C8221-3 will only return 0's for all bits.</li>     <li>Write to DCR x0C8220 will write these bits</li>     <li>Write to DCR x0C8221 will be bit-wise OR-ed with the existing contents -- this is an &ldquo;atomic write-set&rdquo; operation</li>     <li>Write to DCR x0C8222 will be bit-wise NOT AND-ed with the existing contents -- this is an &ldquo;atomic write-clear&rdquo; operation</li>   </ul>   </p> 
#define TESTINT_DCR__USERCODE2_offset  (0x0220) // Offset of register in instance 
#define TESTINT_DCR__USERCODE2_range  (0x4) // Range of external address space
#define TESTINT_DCR__USERCODE2_reset  UNSIGNED64(0x0000000000000000)

//   Field:       U0
//   Description: R/W  usercode2 bits
#define TESTINT_DCR__USERCODE2__U0_width        64
#define TESTINT_DCR__USERCODE2__U0_position     63
#define TESTINT_DCR__USERCODE2__U0_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__USERCODE2__U0_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__USERCODE2__U0_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        usercode3
//   Type:        Read/Write External
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: Usercode3 register.   <ul>     <li>Only a read from DCR x0C8230 will give results. Reads from DCR x0C8231-3 will only return 0's for all bits.</li>     <li>Write to DCR x0C8230 will write these bits</li>     <li>Write to DCR x0C8231 will be bit-wise OR-ed with the existing contents -- this is an &ldquo;atomic write-set&rdquo; operation</li>     <li>Write to DCR x0C8232 will be bit-wise NOT AND-ed with the existing contents -- this is an &ldquo;atomic write-clear&rdquo; operation</li>   </ul>   </p> 
#define TESTINT_DCR__USERCODE3_offset  (0x0230) // Offset of register in instance 
#define TESTINT_DCR__USERCODE3_range  (0x4) // Range of external address space
#define TESTINT_DCR__USERCODE3_reset  UNSIGNED64(0x0000000000000000)

//   Field:       U0
//   Description: R/W  usercode3 bits
#define TESTINT_DCR__USERCODE3__U0_width        64
#define TESTINT_DCR__USERCODE3__U0_position     63
#define TESTINT_DCR__USERCODE3__U0_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__USERCODE3__U0_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__USERCODE3__U0_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        Counter
//   Type:        Read/Write External
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: Wake-Up Counter                            <p>Notes:                            <ul>                            <li>Implements 3 counters, 20 bits each @100 MHz. Thus about 10ms max delay.</li>                            <li>A read from DCR x0C8240 will show the decrementing counter values.</li>                            </ul></p>
#define TESTINT_DCR__COUNTER_offset  (0x0240) // Offset of register in instance 
#define TESTINT_DCR__COUNTER_range  (0x1) // Range of external address space
#define TESTINT_DCR__COUNTER_reset  UNSIGNED64(0x0000000000000000)

//   Field:       bits
//   Description: <table border=0>                             <thead><tr><th>bit</th> <th>description</th></tr></thead>                             <tbody>                               <tr><td> 0</td>    <td>Hold off decrementing the 3 counters below until a glob_sync counter expires</td></tr>                               <tr><td> 1</td>    <td>Enable PUnit clock start counter</td></tr>                               <tr><td> 2</td>    <td>Enable config_reset counter</td></tr>                               <tr><td> 3</td>    <td>Enable threads_reset counter</td></tr>                               <tr><td> 4-23</td> <td>20 bit PUnit clock start counter (default off)</td></tr>                               <tr><td>24-43</td> <td>20 bit CONFIG reset counter </td></tr>                               <tr><td>44-63</td> <td>20 bit THREADS reset counter </td></tr>                             </tbody>                           </table>
#define TESTINT_DCR__COUNTER__BITS_width        64
#define TESTINT_DCR__COUNTER__BITS_position     63
#define TESTINT_DCR__COUNTER__BITS_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__COUNTER__BITS_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__COUNTER__BITS_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        gsync_ctr
//   Type:        Read/Write External
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: Global sync pulse counter.                            <p>Notes:   <ul>    <li>Writing a value N>0 to DCR x0C8250 will start the counter.  It decrements on each received global sync pulse.  When the counter expires it will reset bit 0 of DCR x0c8240.</li>    <li>A read from DCR x0C8250 will give the current (decrementing) counter value.</li>   </ul>   </p> 
#define TESTINT_DCR__GSYNC_CTR_offset  (0x0250) // Offset of register in instance 
#define TESTINT_DCR__GSYNC_CTR_range  (0x1) // Range of external address space
#define TESTINT_DCR__GSYNC_CTR_reset  UNSIGNED64(0x0000000000000000)

//   Field:       bits
//   Description: Glob Sync Counter Value
#define TESTINT_DCR__GSYNC_CTR__BITS_width        64
#define TESTINT_DCR__GSYNC_CTR__BITS_position     63
#define TESTINT_DCR__GSYNC_CTR__BITS_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__GSYNC_CTR__BITS_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__GSYNC_CTR__BITS_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        CLKTREE_CTRL0
//   Type:        Read Only
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: CLKTREE_CTRL0 register.
#define TESTINT_DCR__CLKTREE_CTRL0_offset  (0x0300) // Offset of register in instance 
#define TESTINT_DCR__CLKTREE_CTRL0_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ro
//   Description: RO CLKTREE_CTRL0 bits
#define TESTINT_DCR__CLKTREE_CTRL0__RO_width        64
#define TESTINT_DCR__CLKTREE_CTRL0__RO_position     63
#define TESTINT_DCR__CLKTREE_CTRL0__RO_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__CLKTREE_CTRL0__RO_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__CLKTREE_CTRL0__RO_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        CLKTREE_CTRL1
//   Type:        Read Only
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: CLKTREE_CTRL1 register.
#define TESTINT_DCR__CLKTREE_CTRL1_offset  (0x0310) // Offset of register in instance 
#define TESTINT_DCR__CLKTREE_CTRL1_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ro
//   Description: RO CLKTREE_CTRL1 bits
#define TESTINT_DCR__CLKTREE_CTRL1__RO_width        64
#define TESTINT_DCR__CLKTREE_CTRL1__RO_position     63
#define TESTINT_DCR__CLKTREE_CTRL1__RO_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__CLKTREE_CTRL1__RO_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__CLKTREE_CTRL1__RO_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        CLKTREE_CTRL2
//   Type:        Read Only
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: CLKTREE_CTRL2 register.
#define TESTINT_DCR__CLKTREE_CTRL2_offset  (0x0320) // Offset of register in instance 
#define TESTINT_DCR__CLKTREE_CTRL2_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ro
//   Description: RO CLKTREE_CTRL2 bits
#define TESTINT_DCR__CLKTREE_CTRL2__RO_width        64
#define TESTINT_DCR__CLKTREE_CTRL2__RO_position     63
#define TESTINT_DCR__CLKTREE_CTRL2__RO_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__CLKTREE_CTRL2__RO_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__CLKTREE_CTRL2__RO_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        CLKTREE_CTRL3
//   Type:        Read Only
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: CLKTREE_CTRL3 register.
#define TESTINT_DCR__CLKTREE_CTRL3_offset  (0x0330) // Offset of register in instance 
#define TESTINT_DCR__CLKTREE_CTRL3_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ro
//   Description: RO CLKTREE_CTRL3 bits
#define TESTINT_DCR__CLKTREE_CTRL3__RO_width        64
#define TESTINT_DCR__CLKTREE_CTRL3__RO_position     63
#define TESTINT_DCR__CLKTREE_CTRL3__RO_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__CLKTREE_CTRL3__RO_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__CLKTREE_CTRL3__RO_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        CLKTREE_CTRL4
//   Type:        Read Only
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: CLKTREE_CTRL4 register.
#define TESTINT_DCR__CLKTREE_CTRL4_offset  (0x0340) // Offset of register in instance 
#define TESTINT_DCR__CLKTREE_CTRL4_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ro
//   Description: RO CLKTREE_CTRL4 bits
#define TESTINT_DCR__CLKTREE_CTRL4__RO_width        64
#define TESTINT_DCR__CLKTREE_CTRL4__RO_position     63
#define TESTINT_DCR__CLKTREE_CTRL4__RO_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__CLKTREE_CTRL4__RO_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__CLKTREE_CTRL4__RO_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        CLKTREE_CTRL5
//   Type:        Read Only
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: CLKTREE_CTRL5 register.
#define TESTINT_DCR__CLKTREE_CTRL5_offset  (0x0350) // Offset of register in instance 
#define TESTINT_DCR__CLKTREE_CTRL5_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ro
//   Description: RO CLKTREE_CTRL5 bits
#define TESTINT_DCR__CLKTREE_CTRL5__RO_width        64
#define TESTINT_DCR__CLKTREE_CTRL5__RO_position     63
#define TESTINT_DCR__CLKTREE_CTRL5__RO_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__CLKTREE_CTRL5__RO_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__CLKTREE_CTRL5__RO_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        CLKTREE_CTRL6
//   Type:        Read Only
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: CLKTREE_CTRL6 register.
#define TESTINT_DCR__CLKTREE_CTRL6_offset  (0x0360) // Offset of register in instance 
#define TESTINT_DCR__CLKTREE_CTRL6_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ro
//   Description: RO CLKTREE_CTRL6 bits
#define TESTINT_DCR__CLKTREE_CTRL6__RO_width        64
#define TESTINT_DCR__CLKTREE_CTRL6__RO_position     63
#define TESTINT_DCR__CLKTREE_CTRL6__RO_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__CLKTREE_CTRL6__RO_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__CLKTREE_CTRL6__RO_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        CLKTREE_CTRL7
//   Type:        Read Only
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: CLKTREE_CTRL7 register.
#define TESTINT_DCR__CLKTREE_CTRL7_offset  (0x0370) // Offset of register in instance 
#define TESTINT_DCR__CLKTREE_CTRL7_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ro
//   Description: RO CLKTREE_CTRL7 bits
#define TESTINT_DCR__CLKTREE_CTRL7__RO_width        64
#define TESTINT_DCR__CLKTREE_CTRL7__RO_position     63
#define TESTINT_DCR__CLKTREE_CTRL7__RO_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__CLKTREE_CTRL7__RO_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__CLKTREE_CTRL7__RO_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        CLKTREE_CTRL8
//   Type:        Read Only
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: CLKTREE_CTRL8 register.
#define TESTINT_DCR__CLKTREE_CTRL8_offset  (0x0380) // Offset of register in instance 
#define TESTINT_DCR__CLKTREE_CTRL8_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ro
//   Description: RO CLKTREE_CTRL8 bits
#define TESTINT_DCR__CLKTREE_CTRL8__RO_width        64
#define TESTINT_DCR__CLKTREE_CTRL8__RO_position     63
#define TESTINT_DCR__CLKTREE_CTRL8__RO_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__CLKTREE_CTRL8__RO_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__CLKTREE_CTRL8__RO_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        CLKTREE_CTRL9
//   Type:        Read Only
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: CLKTREE_CTRL9 register.
#define TESTINT_DCR__CLKTREE_CTRL9_offset  (0x0390) // Offset of register in instance 
#define TESTINT_DCR__CLKTREE_CTRL9_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ro
//   Description: RO CLKTREE_CTRL9 bits
#define TESTINT_DCR__CLKTREE_CTRL9__RO_width        64
#define TESTINT_DCR__CLKTREE_CTRL9__RO_position     63
#define TESTINT_DCR__CLKTREE_CTRL9__RO_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__CLKTREE_CTRL9__RO_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__CLKTREE_CTRL9__RO_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        CLKTREE_CTRL10
//   Type:        Read Only
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: CLKTREE_CTRL10 register.
#define TESTINT_DCR__CLKTREE_CTRL10_offset  (0x03A0) // Offset of register in instance 
#define TESTINT_DCR__CLKTREE_CTRL10_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ro
//   Description: RO CLKTREE_CTRL10 bits
#define TESTINT_DCR__CLKTREE_CTRL10__RO_width        64
#define TESTINT_DCR__CLKTREE_CTRL10__RO_position     63
#define TESTINT_DCR__CLKTREE_CTRL10__RO_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__CLKTREE_CTRL10__RO_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__CLKTREE_CTRL10__RO_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        CLKTREE_CTRL11
//   Type:        Read Only
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: CLKTREE_CTRL11 register.
#define TESTINT_DCR__CLKTREE_CTRL11_offset  (0x03B0) // Offset of register in instance 
#define TESTINT_DCR__CLKTREE_CTRL11_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ro
//   Description: RO CLKTREE_CTRL11 bits
#define TESTINT_DCR__CLKTREE_CTRL11__RO_width        64
#define TESTINT_DCR__CLKTREE_CTRL11__RO_position     63
#define TESTINT_DCR__CLKTREE_CTRL11__RO_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__CLKTREE_CTRL11__RO_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__CLKTREE_CTRL11__RO_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        CLKTREE_CTRL12
//   Type:        Read Only
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: CLKTREE_CTRL12 register.
#define TESTINT_DCR__CLKTREE_CTRL12_offset  (0x03C0) // Offset of register in instance 
#define TESTINT_DCR__CLKTREE_CTRL12_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ro
//   Description: RO CLKTREE_CTRL12 bits
#define TESTINT_DCR__CLKTREE_CTRL12__RO_width        64
#define TESTINT_DCR__CLKTREE_CTRL12__RO_position     63
#define TESTINT_DCR__CLKTREE_CTRL12__RO_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__CLKTREE_CTRL12__RO_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__CLKTREE_CTRL12__RO_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        CLKTREE_CTRL13
//   Type:        Read Only
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: CLKTREE_CTRL13 register.
#define TESTINT_DCR__CLKTREE_CTRL13_offset  (0x03D0) // Offset of register in instance 
#define TESTINT_DCR__CLKTREE_CTRL13_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ro
//   Description: RO CLKTREE_CTRL13 bits
#define TESTINT_DCR__CLKTREE_CTRL13__RO_width        64
#define TESTINT_DCR__CLKTREE_CTRL13__RO_position     63
#define TESTINT_DCR__CLKTREE_CTRL13__RO_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__CLKTREE_CTRL13__RO_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__CLKTREE_CTRL13__RO_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        CLKTREE_CTRL14
//   Type:        Read Only
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: CLKTREE_CTRL14 register.
#define TESTINT_DCR__CLKTREE_CTRL14_offset  (0x03E0) // Offset of register in instance 
#define TESTINT_DCR__CLKTREE_CTRL14_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ro
//   Description: RO CLKTREE_CTRL14 bits
#define TESTINT_DCR__CLKTREE_CTRL14__RO_width        64
#define TESTINT_DCR__CLKTREE_CTRL14__RO_position     63
#define TESTINT_DCR__CLKTREE_CTRL14__RO_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__CLKTREE_CTRL14__RO_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__CLKTREE_CTRL14__RO_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        CLKTREE_CTRL15
//   Type:        Read Only
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: CLKTREE_CTRL15 register.
#define TESTINT_DCR__CLKTREE_CTRL15_offset  (0x03F0) // Offset of register in instance 
#define TESTINT_DCR__CLKTREE_CTRL15_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ro
//   Description: RO CLKTREE_CTRL15 bits
#define TESTINT_DCR__CLKTREE_CTRL15__RO_width        64
#define TESTINT_DCR__CLKTREE_CTRL15__RO_position     63
#define TESTINT_DCR__CLKTREE_CTRL15__RO_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__CLKTREE_CTRL15__RO_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__CLKTREE_CTRL15__RO_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        CLKTREE_CTRL16
//   Type:        Read Only
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: CLKTREE_CTRL16 register.
#define TESTINT_DCR__CLKTREE_CTRL16_offset  (0x0400) // Offset of register in instance 
#define TESTINT_DCR__CLKTREE_CTRL16_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ro
//   Description: RO CLKTREE_CTRL16 bits
#define TESTINT_DCR__CLKTREE_CTRL16__RO_width        64
#define TESTINT_DCR__CLKTREE_CTRL16__RO_position     63
#define TESTINT_DCR__CLKTREE_CTRL16__RO_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__CLKTREE_CTRL16__RO_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__CLKTREE_CTRL16__RO_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        CLKTREE_CTRL17
//   Type:        Read Only
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: CLKTREE_CTRL17 register.
#define TESTINT_DCR__CLKTREE_CTRL17_offset  (0x0410) // Offset of register in instance 
#define TESTINT_DCR__CLKTREE_CTRL17_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ro
//   Description: RO CLKTREE_CTRL17 bits
#define TESTINT_DCR__CLKTREE_CTRL17__RO_width        64
#define TESTINT_DCR__CLKTREE_CTRL17__RO_position     63
#define TESTINT_DCR__CLKTREE_CTRL17__RO_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__CLKTREE_CTRL17__RO_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__CLKTREE_CTRL17__RO_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        CLKTREE_CTRL18
//   Type:        Read Only
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: CLKTREE_CTRL18 register.
#define TESTINT_DCR__CLKTREE_CTRL18_offset  (0x0420) // Offset of register in instance 
#define TESTINT_DCR__CLKTREE_CTRL18_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ro
//   Description: RO CLKTREE_CTRL18 bits
#define TESTINT_DCR__CLKTREE_CTRL18__RO_width        64
#define TESTINT_DCR__CLKTREE_CTRL18__RO_position     63
#define TESTINT_DCR__CLKTREE_CTRL18__RO_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__CLKTREE_CTRL18__RO_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__CLKTREE_CTRL18__RO_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        tvs_control
//   Type:        Read/Write External
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: TVSENSE control register.
#define TESTINT_DCR__TVS_CONTROL_offset  (0x0500) // Offset of register in instance 
#define TESTINT_DCR__TVS_CONTROL_range  (0x1) // Range of external address space
#define TESTINT_DCR__TVS_CONTROL_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ctl
//   Description: TVSENSE control bits: reset/sense/sleep; alarm values; select which TVSENSE to send to Envmon
#define TESTINT_DCR__TVS_CONTROL__CTL_width        64
#define TESTINT_DCR__TVS_CONTROL__CTL_position     63
#define TESTINT_DCR__TVS_CONTROL__CTL_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__TVS_CONTROL__CTL_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__TVS_CONTROL__CTL_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        tvs_data
//   Type:        Read Only
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: Temperature register.
#define TESTINT_DCR__TVS_DATA_offset  (0x0510) // Offset of register in instance 
#define TESTINT_DCR__TVS_DATA_reset  UNSIGNED64(0x0000000000000000)

//   Field:       bits
//   Description: Data represents temperature from TVSENSE macros
#define TESTINT_DCR__TVS_DATA__BITS_width        64
#define TESTINT_DCR__TVS_DATA__BITS_position     63
#define TESTINT_DCR__TVS_DATA__BITS_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__TVS_DATA__BITS_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__TVS_DATA__BITS_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        jmm_cntl
//   Type:        Read/Write External
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: JMM Control Register.
#define TESTINT_DCR__JMM_CNTL_offset  (0x0520) // Offset of register in instance 
#define TESTINT_DCR__JMM_CNTL_range  (0x1) // Range of external address space
#define TESTINT_DCR__JMM_CNTL_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ctl
//   Description: JMM control bits
#define TESTINT_DCR__JMM_CNTL__CTL_width        64
#define TESTINT_DCR__JMM_CNTL__CTL_position     63
#define TESTINT_DCR__JMM_CNTL__CTL_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__JMM_CNTL__CTL_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__JMM_CNTL__CTL_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        jmm_data
//   Type:        Read Only
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: JMM Data registers.
#define TESTINT_DCR__JMM_DATA_offset  (0x0530) // Offset of register in instance 
#define TESTINT_DCR__JMM_DATA_range  (0x2) // Range of external address space
#define TESTINT_DCR__JMM_DATA_reset  UNSIGNED64(0x0000000000000000)

//   Field:       bits
//   Description: Jitter data from JMM. DCR x0C8530 contains bits 0-63; Bit 0 is near launching end of delay line (Check!)
#define TESTINT_DCR__JMM_DATA__BITS_width        64
#define TESTINT_DCR__JMM_DATA__BITS_position     63
#define TESTINT_DCR__JMM_DATA__BITS_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__JMM_DATA__BITS_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__JMM_DATA__BITS_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        ecid
//   Type:        Read Only
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: ECID registers.
#define TESTINT_DCR__ECID_offset  (0x0600) // Offset of register in instance 
#define TESTINT_DCR__ECID_range  (0x32) // Range of external address space
#define TESTINT_DCR__ECID_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ro
//   Description: RO ECID bits 0-63
#define TESTINT_DCR__ECID__RO_width        64
#define TESTINT_DCR__ECID__RO_position     63
#define TESTINT_DCR__ECID__RO_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__ECID__RO_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__ECID__RO_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        dcr_pcb_cntl
//   Type:        Read/Write External
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: DCR_to_PCB Control register.
#define TESTINT_DCR__DCR_PCB_CNTL_offset  (0x2000) // Offset of register in instance 
#define TESTINT_DCR__DCR_PCB_CNTL_range  (0x1) // Range of external address space
#define TESTINT_DCR__DCR_PCB_CNTL_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ctl
//   Description: DCR_to_PCB control
#define TESTINT_DCR__DCR_PCB_CNTL__CTL_width        64
#define TESTINT_DCR__DCR_PCB_CNTL__CTL_position     63
#define TESTINT_DCR__DCR_PCB_CNTL__CTL_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__DCR_PCB_CNTL__CTL_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__DCR_PCB_CNTL__CTL_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        dcr_pcb_data
//   Type:        Read/Write External
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: DCR_to_PCB Data register.
#define TESTINT_DCR__DCR_PCB_DATA_offset  (0x2001) // Offset of register in instance 
#define TESTINT_DCR__DCR_PCB_DATA_range  (0x1) // Range of external address space
#define TESTINT_DCR__DCR_PCB_DATA_reset  UNSIGNED64(0x0000000000000000)

//   Field:       bits
//   Description: Data from/to PCB interface
#define TESTINT_DCR__DCR_PCB_DATA__BITS_width        64
#define TESTINT_DCR__DCR_PCB_DATA__BITS_position     63
#define TESTINT_DCR__DCR_PCB_DATA__BITS_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__DCR_PCB_DATA__BITS_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__DCR_PCB_DATA__BITS_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        interrupt_internal_error_sw_info
//   Type:        Interrupt Information
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: Contains status for software type internal errors
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO_offset  (0x0027) // Offset of register in instance 
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       read
//   Description: set if the transaction was a read
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_width        1
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_position     0
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_get(x)       _BGQ_GET(1,0,x)
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_set(v)       _BGQ_SET(1,0,v)
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       write
//   Description: set if the transaction was a write
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_width        1
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_position     1
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_get(x)       _BGQ_GET(1,1,x)
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_set(v)       _BGQ_SET(1,1,v)
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       privileged
//   Description: set if the transaction had privileged permission
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_width        1
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_position     2
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_get(x)       _BGQ_GET(1,2,x)
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_set(v)       _BGQ_SET(1,2,v)
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       test_int
//   Description: set if the transaction had test_int permission
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_width        1
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_position     3
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_get(x)       _BGQ_GET(1,3,x)
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_set(v)       _BGQ_SET(1,3,v)
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       address
//   Description: reflects the address at the time of error
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_width        24
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_position     63
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_get(x)       _BGQ_GET(24,63,x)
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_set(v)       _BGQ_SET(24,63,v)
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_insert(x,v)  _BGQ_INSERT(24,63,x,v)

  
// Register:
//   Name:        interrupt_internal_error_hw_info
//   Type:        Interrupt Information
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: Contains status for hardware type internal errors
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO_offset  (0x0028) // Offset of register in instance 
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       address_parity_good
//   Description: reflects each individual parity status for address
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_width        3
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_position     6
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_get(x)       _BGQ_GET(3,6,x)
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_set(v)       _BGQ_SET(3,6,v)
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_insert(x,v)  _BGQ_INSERT(3,6,x,v)

//   Field:       data_parity_good
//   Description: reflects each individual parity status for data
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_width        8
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_position     14
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_get(x)       _BGQ_GET(8,14,x)
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_set(v)       _BGQ_SET(8,14,v)
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_insert(x,v)  _BGQ_INSERT(8,14,x,v)

//   Field:       address
//   Description: reflects the address at the time of error
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_width        24
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_position     63
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_get(x)       _BGQ_GET(24,63,x)
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_set(v)       _BGQ_SET(24,63,v)
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_insert(x,v)  _BGQ_INSERT(24,63,x,v)

  
// Register:
//   Name:        interrupt_internal_error_data_info
//   Type:        Interrupt Information
//   Permission:  Privileged Only
//   Reset:       0000000000000000
//   Description: Contains status for hardware type internal errors
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO_offset  (0x0029) // Offset of register in instance 
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       data
//   Description: reflects the local data at the time of error
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_width        64
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_position     63
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_get(x)       _BGQ_GET(64,63,x)
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_set(v)       _BGQ_SET(64,63,v)
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Interrupt State Register: ti_interrupt_state
//   State Permission: Privileged Only
//   First Permission: Privileged Only
//   Force Permission: Privileged Only
#define TESTINT_DCR__TI_INTERRUPT_STATE__STATE_offset         (0x0010) // Offset of State register in instance
#define TESTINT_DCR__TI_INTERRUPT_STATE__FIRST_offset         (0x0014) // Offset of First register in instance
#define TESTINT_DCR__TI_INTERRUPT_STATE__FORCE_offset         (0x0013) // Offset of Force register in instance
#define TESTINT_DCR__TI_INTERRUPT_STATE__MACHINE_CHECK_offset (0x0015) // Offset of Machine Check register in instance
#define TESTINT_DCR__TI_INTERRUPT_STATE__CRITICAL_offset      (0x0016) // Offset of Critical register in instance
#define TESTINT_DCR__TI_INTERRUPT_STATE__NONCRITICAL_offset   (0x0017) // Offset of Noncritical register in instance

#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_0_width        1
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_0_position     0
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_0_get(x)       _BGQ_GET(1,0,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_0_set(v)       _BGQ_SET(1,0,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_0_insert(x,v)  _BGQ_INSERT(1,0,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_1_width        1
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_1_position     1
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_1_get(x)       _BGQ_GET(1,1,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_1_set(v)       _BGQ_SET(1,1,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_1_insert(x,v)  _BGQ_INSERT(1,1,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_2_width        1
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_2_position     2
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_2_get(x)       _BGQ_GET(1,2,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_2_set(v)       _BGQ_SET(1,2,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_2_insert(x,v)  _BGQ_INSERT(1,2,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_3_width        1
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_3_position     3
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_3_get(x)       _BGQ_GET(1,3,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_3_set(v)       _BGQ_SET(1,3,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_3_insert(x,v)  _BGQ_INSERT(1,3,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_4_width        1
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_4_position     4
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_4_get(x)       _BGQ_GET(1,4,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_4_set(v)       _BGQ_SET(1,4,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_4_insert(x,v)  _BGQ_INSERT(1,4,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_5_width        1
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_5_position     5
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_5_get(x)       _BGQ_GET(1,5,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_5_set(v)       _BGQ_SET(1,5,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_5_insert(x,v)  _BGQ_INSERT(1,5,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_6_width        1
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_6_position     6
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_6_get(x)       _BGQ_GET(1,6,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_6_set(v)       _BGQ_SET(1,6,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_6_insert(x,v)  _BGQ_INSERT(1,6,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_7_width        1
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_7_position     7
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_7_get(x)       _BGQ_GET(1,7,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_7_set(v)       _BGQ_SET(1,7,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_7_insert(x,v)  _BGQ_INSERT(1,7,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_8_width        1
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_8_position     8
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_8_get(x)       _BGQ_GET(1,8,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_8_set(v)       _BGQ_SET(1,8,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_8_insert(x,v)  _BGQ_INSERT(1,8,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_9_width        1
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_9_position     9
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_9_get(x)       _BGQ_GET(1,9,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_9_set(v)       _BGQ_SET(1,9,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_9_insert(x,v)  _BGQ_INSERT(1,9,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_10_width        1
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_10_position     10
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_10_get(x)       _BGQ_GET(1,10,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_10_set(v)       _BGQ_SET(1,10,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_10_insert(x,v)  _BGQ_INSERT(1,10,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_11_width        1
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_11_position     11
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_11_get(x)       _BGQ_GET(1,11,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_11_set(v)       _BGQ_SET(1,11,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_11_insert(x,v)  _BGQ_INSERT(1,11,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_12_width        1
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_12_position     12
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_12_get(x)       _BGQ_GET(1,12,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_12_set(v)       _BGQ_SET(1,12,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_12_insert(x,v)  _BGQ_INSERT(1,12,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_13_width        1
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_13_position     13
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_13_get(x)       _BGQ_GET(1,13,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_13_set(v)       _BGQ_SET(1,13,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_13_insert(x,v)  _BGQ_INSERT(1,13,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_14_width        1
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_14_position     14
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_14_get(x)       _BGQ_GET(1,14,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_14_set(v)       _BGQ_SET(1,14,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_14_insert(x,v)  _BGQ_INSERT(1,14,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_15_width        1
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_15_position     15
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_15_get(x)       _BGQ_GET(1,15,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_15_set(v)       _BGQ_SET(1,15,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_15_insert(x,v)  _BGQ_INSERT(1,15,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_16_width        1
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_16_position     16
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_16_get(x)       _BGQ_GET(1,16,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_16_set(v)       _BGQ_SET(1,16,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_16_insert(x,v)  _BGQ_INSERT(1,16,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_17_width        1
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_17_position     17
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_17_get(x)       _BGQ_GET(1,17,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_17_set(v)       _BGQ_SET(1,17,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_17_insert(x,v)  _BGQ_INSERT(1,17,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_18_width        1
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_18_position     18
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_18_get(x)       _BGQ_GET(1,18,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_18_set(v)       _BGQ_SET(1,18,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_18_insert(x,v)  _BGQ_INSERT(1,18,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_19_width        1
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_19_position     19
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_19_get(x)       _BGQ_GET(1,19,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_19_set(v)       _BGQ_SET(1,19,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_19_insert(x,v)  _BGQ_INSERT(1,19,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_20_width        1
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_20_position     20
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_20_get(x)       _BGQ_GET(1,20,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_20_set(v)       _BGQ_SET(1,20,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_20_insert(x,v)  _BGQ_INSERT(1,20,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_21_width        1
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_21_position     21
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_21_get(x)       _BGQ_GET(1,21,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_21_set(v)       _BGQ_SET(1,21,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_21_insert(x,v)  _BGQ_INSERT(1,21,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_22_width        1
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_22_position     22
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_22_get(x)       _BGQ_GET(1,22,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_22_set(v)       _BGQ_SET(1,22,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_22_insert(x,v)  _BGQ_INSERT(1,22,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_23_width        1
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_23_position     23
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_23_get(x)       _BGQ_GET(1,23,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_23_set(v)       _BGQ_SET(1,23,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE__BIC_TDR_23_insert(x,v)  _BGQ_INSERT(1,23,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE__GSYNC_width        1
#define TESTINT_DCR__TI_INTERRUPT_STATE__GSYNC_position     24
#define TESTINT_DCR__TI_INTERRUPT_STATE__GSYNC_get(x)       _BGQ_GET(1,24,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE__GSYNC_set(v)       _BGQ_SET(1,24,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE__GSYNC_insert(x,v)  _BGQ_INSERT(1,24,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE__DCR_TO_PCB_ERR_width        1
#define TESTINT_DCR__TI_INTERRUPT_STATE__DCR_TO_PCB_ERR_position     25
#define TESTINT_DCR__TI_INTERRUPT_STATE__DCR_TO_PCB_ERR_get(x)       _BGQ_GET(1,25,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE__DCR_TO_PCB_ERR_set(v)       _BGQ_SET(1,25,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE__DCR_TO_PCB_ERR_insert(x,v)  _BGQ_INSERT(1,25,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE__LOCAL_RING_width        1
#define TESTINT_DCR__TI_INTERRUPT_STATE__LOCAL_RING_position     26
#define TESTINT_DCR__TI_INTERRUPT_STATE__LOCAL_RING_get(x)       _BGQ_GET(1,26,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE__LOCAL_RING_set(v)       _BGQ_SET(1,26,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE__LOCAL_RING_insert(x,v)  _BGQ_INSERT(1,26,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE__INT_PARITY_ERROR_width        1
#define TESTINT_DCR__TI_INTERRUPT_STATE__INT_PARITY_ERROR_position     27
#define TESTINT_DCR__TI_INTERRUPT_STATE__INT_PARITY_ERROR_get(x)       _BGQ_GET(1,27,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE__INT_PARITY_ERROR_set(v)       _BGQ_SET(1,27,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE__INT_PARITY_ERROR_insert(x,v)  _BGQ_INSERT(1,27,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE__INT_MBOX_01_EQ_width        1
#define TESTINT_DCR__TI_INTERRUPT_STATE__INT_MBOX_01_EQ_position     28
#define TESTINT_DCR__TI_INTERRUPT_STATE__INT_MBOX_01_EQ_get(x)       _BGQ_GET(1,28,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE__INT_MBOX_01_EQ_set(v)       _BGQ_SET(1,28,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE__INT_MBOX_01_EQ_insert(x,v)  _BGQ_INSERT(1,28,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE__INT_MBOX_01_NE_width        1
#define TESTINT_DCR__TI_INTERRUPT_STATE__INT_MBOX_01_NE_position     29
#define TESTINT_DCR__TI_INTERRUPT_STATE__INT_MBOX_01_NE_get(x)       _BGQ_GET(1,29,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE__INT_MBOX_01_NE_set(v)       _BGQ_SET(1,29,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE__INT_MBOX_01_NE_insert(x,v)  _BGQ_INSERT(1,29,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE__INT_MBOX_23_EQ_width        1
#define TESTINT_DCR__TI_INTERRUPT_STATE__INT_MBOX_23_EQ_position     30
#define TESTINT_DCR__TI_INTERRUPT_STATE__INT_MBOX_23_EQ_get(x)       _BGQ_GET(1,30,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE__INT_MBOX_23_EQ_set(v)       _BGQ_SET(1,30,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE__INT_MBOX_23_EQ_insert(x,v)  _BGQ_INSERT(1,30,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE__INT_MBOX_23_NE_width        1
#define TESTINT_DCR__TI_INTERRUPT_STATE__INT_MBOX_23_NE_position     31
#define TESTINT_DCR__TI_INTERRUPT_STATE__INT_MBOX_23_NE_get(x)       _BGQ_GET(1,31,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE__INT_MBOX_23_NE_set(v)       _BGQ_SET(1,31,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE__INT_MBOX_23_NE_insert(x,v)  _BGQ_INSERT(1,31,x,v)

  
// Interrupt State Register: interrupt_internal_error
//   State Permission: Privileged Only
//   First Permission: Privileged Only
//   Force Permission: Privileged Only
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR__STATE_offset         (0x0020) // Offset of State register in instance
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR__FIRST_offset         (0x0023) // Offset of First register in instance
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR__FORCE_offset         (0x0022) // Offset of Force register in instance
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR__MACHINE_CHECK_offset (0x0024) // Offset of Machine Check register in instance
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR__CRITICAL_offset      (0x0025) // Offset of Critical register in instance
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR__NONCRITICAL_offset   (0x0026) // Offset of Noncritical register in instance

#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_width        1
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_position     0
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_get(x)       _BGQ_GET(1,0,x)
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_set(v)       _BGQ_SET(1,0,v)
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_insert(x,v)  _BGQ_INSERT(1,0,x,v)


#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_width        1
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_position     1
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_get(x)       _BGQ_GET(1,1,x)
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_set(v)       _BGQ_SET(1,1,v)
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_insert(x,v)  _BGQ_INSERT(1,1,x,v)


#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_width        1
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_position     2
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_get(x)       _BGQ_GET(1,2,x)
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_set(v)       _BGQ_SET(1,2,v)
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_insert(x,v)  _BGQ_INSERT(1,2,x,v)


#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_width        1
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_position     3
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_get(x)       _BGQ_GET(1,3,x)
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_set(v)       _BGQ_SET(1,3,v)
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_insert(x,v)  _BGQ_INSERT(1,3,x,v)

  
// Interrupt Control Register: ti_interrupt_state_control_low
//   Permission: Privileged Only
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_LOW_offset         (0x0011) // Offset of Control register in instance
  
// Interrupt Control Register: ti_interrupt_state_control_high
//   Permission: Privileged Only
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH_offset         (0x0012) // Offset of Control register in instance

#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_0_width        2
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_0_position     1
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_0_get(x)       _BGQ_GET(2,1,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_0_set(v)       _BGQ_SET(2,1,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_0_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_1_width        2
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_1_position     3
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_1_get(x)       _BGQ_GET(2,3,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_1_set(v)       _BGQ_SET(2,3,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_1_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_2_width        2
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_2_position     5
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_2_get(x)       _BGQ_GET(2,5,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_2_set(v)       _BGQ_SET(2,5,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_2_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_3_width        2
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_3_position     7
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_3_get(x)       _BGQ_GET(2,7,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_3_set(v)       _BGQ_SET(2,7,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_3_insert(x,v)  _BGQ_INSERT(2,7,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_4_width        2
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_4_position     9
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_4_get(x)       _BGQ_GET(2,9,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_4_set(v)       _BGQ_SET(2,9,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_4_insert(x,v)  _BGQ_INSERT(2,9,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_5_width        2
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_5_position     11
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_5_get(x)       _BGQ_GET(2,11,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_5_set(v)       _BGQ_SET(2,11,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_5_insert(x,v)  _BGQ_INSERT(2,11,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_6_width        2
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_6_position     13
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_6_get(x)       _BGQ_GET(2,13,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_6_set(v)       _BGQ_SET(2,13,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_6_insert(x,v)  _BGQ_INSERT(2,13,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_7_width        2
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_7_position     15
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_7_get(x)       _BGQ_GET(2,15,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_7_set(v)       _BGQ_SET(2,15,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_7_insert(x,v)  _BGQ_INSERT(2,15,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_8_width        2
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_8_position     17
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_8_get(x)       _BGQ_GET(2,17,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_8_set(v)       _BGQ_SET(2,17,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_8_insert(x,v)  _BGQ_INSERT(2,17,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_9_width        2
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_9_position     19
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_9_get(x)       _BGQ_GET(2,19,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_9_set(v)       _BGQ_SET(2,19,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_9_insert(x,v)  _BGQ_INSERT(2,19,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_10_width        2
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_10_position     21
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_10_get(x)       _BGQ_GET(2,21,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_10_set(v)       _BGQ_SET(2,21,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_10_insert(x,v)  _BGQ_INSERT(2,21,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_11_width        2
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_11_position     23
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_11_get(x)       _BGQ_GET(2,23,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_11_set(v)       _BGQ_SET(2,23,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_11_insert(x,v)  _BGQ_INSERT(2,23,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_12_width        2
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_12_position     25
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_12_get(x)       _BGQ_GET(2,25,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_12_set(v)       _BGQ_SET(2,25,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_12_insert(x,v)  _BGQ_INSERT(2,25,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_13_width        2
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_13_position     27
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_13_get(x)       _BGQ_GET(2,27,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_13_set(v)       _BGQ_SET(2,27,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_13_insert(x,v)  _BGQ_INSERT(2,27,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_14_width        2
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_14_position     29
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_14_get(x)       _BGQ_GET(2,29,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_14_set(v)       _BGQ_SET(2,29,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_14_insert(x,v)  _BGQ_INSERT(2,29,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_15_width        2
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_15_position     31
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_15_get(x)       _BGQ_GET(2,31,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_15_set(v)       _BGQ_SET(2,31,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_15_insert(x,v)  _BGQ_INSERT(2,31,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_16_width        2
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_16_position     33
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_16_get(x)       _BGQ_GET(2,33,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_16_set(v)       _BGQ_SET(2,33,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_16_insert(x,v)  _BGQ_INSERT(2,33,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_17_width        2
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_17_position     35
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_17_get(x)       _BGQ_GET(2,35,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_17_set(v)       _BGQ_SET(2,35,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_17_insert(x,v)  _BGQ_INSERT(2,35,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_18_width        2
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_18_position     37
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_18_get(x)       _BGQ_GET(2,37,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_18_set(v)       _BGQ_SET(2,37,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_18_insert(x,v)  _BGQ_INSERT(2,37,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_19_width        2
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_19_position     39
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_19_get(x)       _BGQ_GET(2,39,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_19_set(v)       _BGQ_SET(2,39,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_19_insert(x,v)  _BGQ_INSERT(2,39,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_20_width        2
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_20_position     41
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_20_get(x)       _BGQ_GET(2,41,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_20_set(v)       _BGQ_SET(2,41,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_20_insert(x,v)  _BGQ_INSERT(2,41,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_21_width        2
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_21_position     43
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_21_get(x)       _BGQ_GET(2,43,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_21_set(v)       _BGQ_SET(2,43,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_21_insert(x,v)  _BGQ_INSERT(2,43,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_22_width        2
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_22_position     45
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_22_get(x)       _BGQ_GET(2,45,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_22_set(v)       _BGQ_SET(2,45,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_22_insert(x,v)  _BGQ_INSERT(2,45,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_23_width        2
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_23_position     47
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_23_get(x)       _BGQ_GET(2,47,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_23_set(v)       _BGQ_SET(2,47,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__BIC_TDR_23_insert(x,v)  _BGQ_INSERT(2,47,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__GSYNC_width        2
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__GSYNC_position     49
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__GSYNC_get(x)       _BGQ_GET(2,49,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__GSYNC_set(v)       _BGQ_SET(2,49,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__GSYNC_insert(x,v)  _BGQ_INSERT(2,49,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__DCR_TO_PCB_ERR_width        2
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__DCR_TO_PCB_ERR_position     51
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__DCR_TO_PCB_ERR_get(x)       _BGQ_GET(2,51,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__DCR_TO_PCB_ERR_set(v)       _BGQ_SET(2,51,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__DCR_TO_PCB_ERR_insert(x,v)  _BGQ_INSERT(2,51,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__LOCAL_RING_width        2
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__LOCAL_RING_position     53
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__LOCAL_RING_get(x)       _BGQ_GET(2,53,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__LOCAL_RING_set(v)       _BGQ_SET(2,53,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__LOCAL_RING_insert(x,v)  _BGQ_INSERT(2,53,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__INT_PARITY_ERROR_width        2
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__INT_PARITY_ERROR_position     55
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__INT_PARITY_ERROR_get(x)       _BGQ_GET(2,55,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__INT_PARITY_ERROR_set(v)       _BGQ_SET(2,55,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__INT_PARITY_ERROR_insert(x,v)  _BGQ_INSERT(2,55,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__INT_MBOX_01_EQ_width        2
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__INT_MBOX_01_EQ_position     57
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__INT_MBOX_01_EQ_get(x)       _BGQ_GET(2,57,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__INT_MBOX_01_EQ_set(v)       _BGQ_SET(2,57,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__INT_MBOX_01_EQ_insert(x,v)  _BGQ_INSERT(2,57,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__INT_MBOX_01_NE_width        2
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__INT_MBOX_01_NE_position     59
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__INT_MBOX_01_NE_get(x)       _BGQ_GET(2,59,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__INT_MBOX_01_NE_set(v)       _BGQ_SET(2,59,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__INT_MBOX_01_NE_insert(x,v)  _BGQ_INSERT(2,59,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__INT_MBOX_23_EQ_width        2
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__INT_MBOX_23_EQ_position     61
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__INT_MBOX_23_EQ_get(x)       _BGQ_GET(2,61,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__INT_MBOX_23_EQ_set(v)       _BGQ_SET(2,61,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__INT_MBOX_23_EQ_insert(x,v)  _BGQ_INSERT(2,61,x,v)


#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__INT_MBOX_23_NE_width        2
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__INT_MBOX_23_NE_position     63
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__INT_MBOX_23_NE_get(x)       _BGQ_GET(2,63,x)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__INT_MBOX_23_NE_set(v)       _BGQ_SET(2,63,v)
#define TESTINT_DCR__TI_INTERRUPT_STATE_CONTROL_HIGH__INT_MBOX_23_NE_insert(x,v)  _BGQ_INSERT(2,63,x,v)

  
// Interrupt Control Register: interrupt_internal_error_control_high
//   Permission: Privileged Only
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH_offset         (0x0021) // Offset of Control register in instance

#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_width        2
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_position     1
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_get(x)       _BGQ_GET(2,1,x)
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_set(v)       _BGQ_SET(2,1,v)
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_width        2
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_position     3
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_get(x)       _BGQ_GET(2,3,x)
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_set(v)       _BGQ_SET(2,3,v)
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_width        2
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_position     5
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_get(x)       _BGQ_GET(2,5,x)
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_set(v)       _BGQ_SET(2,5,v)
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_width        2
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_position     7
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_get(x)       _BGQ_GET(2,7,x)
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_set(v)       _BGQ_SET(2,7,v)
#define TESTINT_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_insert(x,v)  _BGQ_INSERT(2,7,x,v)

#ifndef __ASSEMBLY__

typedef struct testint_dcr { 
    /* 0x0000 */ uint64_t jtag_idcode;
    /* 0x0001 */ uint64_t reserve_0001[0x000F];
    /* 0x0010 */ uint64_t ti_interrupt_state__state;
    /* 0x0011 */ uint64_t ti_interrupt_state_control_low;
    /* 0x0012 */ uint64_t ti_interrupt_state_control_high;
    /* 0x0013 */ uint64_t ti_interrupt_state__force;
    /* 0x0014 */ uint64_t ti_interrupt_state__first;
    /* 0x0015 */ uint64_t ti_interrupt_state__machine_check;
    /* 0x0016 */ uint64_t ti_interrupt_state__critical;
    /* 0x0017 */ uint64_t ti_interrupt_state__noncritical;
    /* 0x0018 */ uint64_t reserve_0018[0x0008];
    /* 0x0020 */ uint64_t interrupt_internal_error__state;
    /* 0x0021 */ uint64_t interrupt_internal_error_control_high;
    /* 0x0022 */ uint64_t interrupt_internal_error__force;
    /* 0x0023 */ uint64_t interrupt_internal_error__first;
    /* 0x0024 */ uint64_t interrupt_internal_error__machine_check;
    /* 0x0025 */ uint64_t interrupt_internal_error__critical;
    /* 0x0026 */ uint64_t interrupt_internal_error__noncritical;
    /* 0x0027 */ uint64_t interrupt_internal_error_sw_info;
    /* 0x0028 */ uint64_t interrupt_internal_error_hw_info;
    /* 0x0029 */ uint64_t interrupt_internal_error_data_info;
    /* 0x002A */ uint64_t reserve_002A[0x0006];
    /* 0x0030 */ uint64_t clocks[TESTINT_DCR__CLOCKS_range];
    /* 0x0034 */ uint64_t reserve_0034[0x000C];
    /* 0x0040 */ uint64_t config0[TESTINT_DCR__CONFIG0_range];
    /* 0x0044 */ uint64_t config1[TESTINT_DCR__CONFIG1_range];
    /* 0x0048 */ uint64_t Config0a;
    /* 0x0049 */ uint64_t Config1a;
    /* 0x004A */ uint64_t reserve_004A[0x0006];
    /* 0x0050 */ uint64_t thread_active0[TESTINT_DCR__THREAD_ACTIVE0_range];
    /* 0x0054 */ uint64_t thread_active1[TESTINT_DCR__THREAD_ACTIVE1_range];
    /* 0x0058 */ uint64_t thread_active0a;
    /* 0x0059 */ uint64_t thread_active1a;
    /* 0x005A */ uint64_t thread_active0_rb;
    /* 0x005B */ uint64_t thread_active1_rb;
    /* 0x005C */ uint64_t reserve_005C[0x0064];
    /* 0x00C0 */ uint64_t glob_att[TESTINT_DCR__GLOB_ATT_range];
    /* 0x00C4 */ uint64_t reserve_00C4[0x000C];
    /* 0x00D0 */ uint64_t glob_att_mask;
    /* 0x00D1 */ uint64_t reserve_00D1[0x002F];
    /* 0x0100 */ uint64_t gp1_reg;
    /* 0x0101 */ uint64_t reserve_0101[0x000F];
    /* 0x0110 */ uint64_t gp2_reg[TESTINT_DCR__GP2_REG_range];
    /* 0x0114 */ uint64_t reserve_0114[0x000C];
    /* 0x0120 */ uint64_t gp3_reg;
    /* 0x0121 */ uint64_t reserve_0121[0x003F];
    /* 0x0160 */ uint64_t mailbox_reg0[TESTINT_DCR__MAILBOX_REG0_range];
    /* 0x0164 */ uint64_t reserve_0164[0x000C];
    /* 0x0170 */ uint64_t mailbox_reg1[TESTINT_DCR__MAILBOX_REG1_range];
    /* 0x0174 */ uint64_t reserve_0174[0x000C];
    /* 0x0180 */ uint64_t mailbox_reg2[TESTINT_DCR__MAILBOX_REG2_range];
    /* 0x0184 */ uint64_t reserve_0184[0x000C];
    /* 0x0190 */ uint64_t mailbox_reg3[TESTINT_DCR__MAILBOX_REG3_range];
    /* 0x0194 */ uint64_t reserve_0194[0x000C];
    /* 0x01A0 */ uint64_t parity;
    /* 0x01A1 */ uint64_t reserve_01A1[0x001F];
    /* 0x01C0 */ uint64_t redund_ctrl;
    /* 0x01C1 */ uint64_t reserve_01C1[0x000F];
    /* 0x01D0 */ uint64_t slice_sel_ctrl;
    /* 0x01D1 */ uint64_t reserve_01D1[0x002F];
    /* 0x0200 */ uint64_t usercode0[TESTINT_DCR__USERCODE0_range];
    /* 0x0204 */ uint64_t reserve_0204[0x000C];
    /* 0x0210 */ uint64_t usercode1[TESTINT_DCR__USERCODE1_range];
    /* 0x0214 */ uint64_t reserve_0214[0x000C];
    /* 0x0220 */ uint64_t usercode2[TESTINT_DCR__USERCODE2_range];
    /* 0x0224 */ uint64_t reserve_0224[0x000C];
    /* 0x0230 */ uint64_t usercode3[TESTINT_DCR__USERCODE3_range];
    /* 0x0234 */ uint64_t reserve_0234[0x000C];
    /* 0x0240 */ uint64_t Counter;
    /* 0x0241 */ uint64_t reserve_0241[0x000F];
    /* 0x0250 */ uint64_t gsync_ctr;
    /* 0x0251 */ uint64_t reserve_0251[0x00AF];
    /* 0x0300 */ uint64_t CLKTREE_CTRL0;
    /* 0x0301 */ uint64_t reserve_0301[0x000F];
    /* 0x0310 */ uint64_t CLKTREE_CTRL1;
    /* 0x0311 */ uint64_t reserve_0311[0x000F];
    /* 0x0320 */ uint64_t CLKTREE_CTRL2;
    /* 0x0321 */ uint64_t reserve_0321[0x000F];
    /* 0x0330 */ uint64_t CLKTREE_CTRL3;
    /* 0x0331 */ uint64_t reserve_0331[0x000F];
    /* 0x0340 */ uint64_t CLKTREE_CTRL4;
    /* 0x0341 */ uint64_t reserve_0341[0x000F];
    /* 0x0350 */ uint64_t CLKTREE_CTRL5;
    /* 0x0351 */ uint64_t reserve_0351[0x000F];
    /* 0x0360 */ uint64_t CLKTREE_CTRL6;
    /* 0x0361 */ uint64_t reserve_0361[0x000F];
    /* 0x0370 */ uint64_t CLKTREE_CTRL7;
    /* 0x0371 */ uint64_t reserve_0371[0x000F];
    /* 0x0380 */ uint64_t CLKTREE_CTRL8;
    /* 0x0381 */ uint64_t reserve_0381[0x000F];
    /* 0x0390 */ uint64_t CLKTREE_CTRL9;
    /* 0x0391 */ uint64_t reserve_0391[0x000F];
    /* 0x03A0 */ uint64_t CLKTREE_CTRL10;
    /* 0x03A1 */ uint64_t reserve_03A1[0x000F];
    /* 0x03B0 */ uint64_t CLKTREE_CTRL11;
    /* 0x03B1 */ uint64_t reserve_03B1[0x000F];
    /* 0x03C0 */ uint64_t CLKTREE_CTRL12;
    /* 0x03C1 */ uint64_t reserve_03C1[0x000F];
    /* 0x03D0 */ uint64_t CLKTREE_CTRL13;
    /* 0x03D1 */ uint64_t reserve_03D1[0x000F];
    /* 0x03E0 */ uint64_t CLKTREE_CTRL14;
    /* 0x03E1 */ uint64_t reserve_03E1[0x000F];
    /* 0x03F0 */ uint64_t CLKTREE_CTRL15;
    /* 0x03F1 */ uint64_t reserve_03F1[0x000F];
    /* 0x0400 */ uint64_t CLKTREE_CTRL16;
    /* 0x0401 */ uint64_t reserve_0401[0x000F];
    /* 0x0410 */ uint64_t CLKTREE_CTRL17;
    /* 0x0411 */ uint64_t reserve_0411[0x000F];
    /* 0x0420 */ uint64_t CLKTREE_CTRL18;
    /* 0x0421 */ uint64_t reserve_0421[0x00DF];
    /* 0x0500 */ uint64_t tvs_control;
    /* 0x0501 */ uint64_t reserve_0501[0x000F];
    /* 0x0510 */ uint64_t tvs_data;
    /* 0x0511 */ uint64_t reserve_0511[0x000F];
    /* 0x0520 */ uint64_t jmm_cntl;
    /* 0x0521 */ uint64_t reserve_0521[0x000F];
    /* 0x0530 */ uint64_t jmm_data[TESTINT_DCR__JMM_DATA_range];
    /* 0x0532 */ uint64_t reserve_0532[0x00CE];
    /* 0x0600 */ uint64_t ecid[TESTINT_DCR__ECID_range];
    /* 0x0632 */ uint64_t reserve_0632[0x19CE];
    /* 0x2000 */ uint64_t dcr_pcb_cntl;
    /* 0x2001 */ uint64_t dcr_pcb_data;
} testint_dcr_t;
  
#define TESTINT_DCR_USER_PTR _DCR_USER_PTR(testint_dcr_t, TESTINT_DCR)
#define TESTINT_DCR_PRIV_PTR _DCR_PRIV_PTR(testint_dcr_t, TESTINT_DCR)
  
#endif // !__ASSEMBLY__
  
__END_DECLS
  
#endif // add nothing below this line.
