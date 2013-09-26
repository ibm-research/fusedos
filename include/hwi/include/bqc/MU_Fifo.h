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

#ifndef	_MUHWI_FIFO_H_ /* Prevent multiple inclusion */
#define	_MUHWI_FIFO_H_


/**
 * \file MU_Fifo.h
 *
 * \brief C Header File containing Message Unit hardware Fifo Definitions
 */

#include <hwi/include/bqc/MU_Macros.h>


/**
 * \brief Descriptor Size
 *
 * The number of bytes in a descriptor.  Injection fifos contain
 * zero or more descriptors.
 */
#define BGQ_MU_DESCRIPTOR_SIZE_IN_BYTES       64


/**
 * \brief Descriptor Size in Power-of-2 Units
 *
 * The power-of-2 number of bytes in a descriptor.  Injection fifos contain
 * zero or more descriptors.
 */
#define BGQ_MU_DESCRIPTOR_SIZE_IN_POWER_OF_2 6


/** 
 * \brief Minimum (non-remote-get) Injection Fifo Size
 *
 * All (non-remote-get) injection fifos must be at least this large.
 * It must be large enough to hold 1 descriptor and have 1 additional
 * descriptor slot unused.  That slot can be anywhere in the fifo, and can
 * change depending on where the current head and tail are located.
 *
 * For example, in a 2-slot fifo (1 used, 1 unused), the first descriptor
 * will be injected into slot 0, and the fifo will be full.  Slot 1 will
 * be unused.  When the first descriptor is complete, the next descriptor
 * can be injected, and will go into slot 1.  Again, the fifo will be full,
 * but this time, slot 0 will be unused.
 */
#define BGQ_MU_MIN_INJFIFO_SIZE_IN_BYTES   (2*BGQ_MU_DESCRIPTOR_SIZE_IN_BYTES)   


/**
 * \brief Minimum Number of (non-remote-get) Injection Fifo Slots
 *
 * All (non-remote-get) injection fifos must have at least this many descriptor
 * slots.  This is determined based on the minimum injection fifo size.
 */
#define BGQ_MU_MIN_INJFIFO_SLOTS              (BGQ_MU_MIN_INJFIFO_SIZE_IN_BYTES / BGQ_MU_DESCRIPTOR_SIZE_IN_BYTES)


/** 
 * \brief Minimum Remote Get Injection Fifo Size
 *
 * All Remote Get injection fifos must be at least this large.
 * It must be large enough to hold 1 packet of descriptors (512 bytes) and have
 * 1 additional descriptor slot unused.  That slot can be anywhere in the fifo,
 * and can change depending on where the current head and tail are located.
 *
 */
#define BGQ_MU_MIN_RGETFIFO_SIZE_IN_BYTES   (512 + BGQ_MU_DESCRIPTOR_SIZE_IN_BYTES)   


/**
 * \brief Minimum Reception Fifo Size
 *
 * All reception fifos must be at least this large.
 * It must be large enough to hold 1 packet (544 bytes) and have 1 32-byte chunk
 * unused.  That unused chunk can be anywhere in the fifo, and can change
 * depending on where the current head and tail are located.
 */
#define BGQ_MU_MIN_RECFIFO_SIZE_IN_BYTES      (544 + 32)


/**
 * \brief Hardware Fifo Structure
 *
 * Each fifo contains these fields in the MU SRAM.
 */
typedef struct MUHWI_Fifo
{
  volatile uint64_t    pa_start; /**< Physical address of the start of the fifo */
  volatile uint64_t    size_1;   /**< The size of the fifo - 1, in bytes        */
  volatile uint64_t    pa_head;  /**< Physical address of the head of the fifo.
                                      This points to the descriptor that the MU
                                      is currently processing or will process
				      next.  The MU moves this pointer when it 
				      has completed processing a descriptor.    */
  volatile uint64_t    pa_tail;  /**< Physical address of the tail of the fifo.
				      This points to the next available slot 
				      for a descriptor to be injected.          */
} MUHWI_Fifo_t;


/**
 * \brief Hardware Extension for Injection Fifo
 *
 * The injection fifo is extended with additional state in the SRAM to show the
 * free space and number of descriptors completed.  This additional state saves
 * software overheads to compute them.
 */
typedef struct MUHWI_InjFifo
{
  volatile uint64_t descCount;  /**< Number of descriptors completed in injfifo SRAM */
  volatile uint64_t freeSpace;  /**< Fifo free space in injfifo SRAM */
} MUHWI_InjFifo_t;


/**
 * \brief Hardware Injection Fifo Status
 *
 * This structure maps an area of the MU SRAM that provides status about
 * the injection fifos
 */
typedef struct MUHWI_InjFifoStatus {
  volatile uint64_t     isEnabled;/**< The fifos that have been enabled.
                                       This shows status for an entire group
				       (4 subgroups).  There are 8 bits per
				       subgroup, one bit for each injection fifo
				       in that subgroup.  The first subgroup bits
				       are 32-39, the second subgroup bits are
				       40-47, the third subgroup bits are 48-55,
				       and the fourth subgroup bits are 56-63. */
  volatile uint64_t     enable;   /**< Enable injection fifos.  This has enable
				       bits for an entire group (4 subgroups).
				       Writing a 1 to a bit enables that fifo.
				       There are 8 bits per subgroup, one bit 
				       for each injection fifo in that subgroup.
				       The first subgroup bits are 32-39, the 
				       second subgroup bits are 40-47, the 
				       third subgroup bits are 48-55,
				       and the fourth subgroup bits are 56-63.*/
  volatile uint64_t     disable;  /**< Disable injection fifos. This has disable
				       bits for an entire group (4 subgroups).
				       Writing a 1 to a bit disables that fifo.
				       There are 8 bits per subgroup, one bit 
				       for each injection fifo in that subgroup.
				       The first subgroup bits are 32-39, the 
				       second subgroup bits are 40-47, the 
				       third subgroup bits are 48-55,
				       and the fourth subgroup bits are 56-63. */
  volatile uint64_t     priority; /**< Sets the priority of injection fifos.
				       This has priority bits for the entire
				       group (4 subgroups).  Setting a bit to 1
				       means that injection fifo has high 
				       priority.  Note that system fifos already
				       have high priority and ignore these bits.
				       There are 8 bits per subgroup, one bit 
				       for each injection fifo in that subgroup.
				       The first subgroup bits are 32-39, the 
				       second subgroup bits are 40-47, the 
				       third subgroup bits are 48-55,
				       and the fourth subgroup bits are 56-63. */
  volatile uint64_t     interruptStatus; /**< Fifo interrupt status.
				       This has interrupt bits for the entire 
				       group (4 subgroups).  Bits 0-31 are for
				       each of the injection fifos.  A 1
				       indicates the injection fifo got a 
				       free-space threshold up-crossing (the
				       free-space became larger than the free
				       space specified in the injection
				       fifo threshold DCR register).
				       Bits 32-63 are for reception fifos.
				       See MUHWI_RecFifoStatus_t.
				       There are 8 bits per subgroup, one bit 
				       for each injection fifo in that subgroup.
				       The first subgroup bits are 0-7, the 
				       second subgroup bits are 8-15, the 
				       third subgroup bits are 16-23,
				       and the fourth subgroup bits are 24-31. */
  volatile uint64_t     clearInterruptStatus; /**< Fifo clear interrupt status.
				       This has clear interrupt bits for the entire 
				       group (4 subgroups).  Bits 0-31 are for
				       each of the injection fifos.  Setting a
				       bit to 1 clears the interrupt (clears the
				       corresponding bit in the interruptStatus).
				       Bits 32-63 are for reception fifos.
				       See MUHWI_RecFifoStatus_t.
				       There are 8 bits per subgroup, one bit 
				       for each injection fifo in that subgroup.
				       The first subgroup bits are 0-7, the 
				       second subgroup bits are 8-15, the 
				       third subgroup bits are 16-23,
				       and the fourth subgroup bits are 24-31. */
} MUHWI_InjFifoStatus_t;


/**
 * \brief Hardware Reception Fifo Status
 *
 * This structure maps an area of the MU SRAM that provides status about
 * the reception fifos
 */
typedef struct MUHWI_RecFifoStatus {
  volatile uint64_t     _reserved [4];   /**< Reserved bytes for
					      injection fifo status */
  volatile uint64_t     interruptStatus; /**< Fifo interrupt status.
				       This has interrupt bits for the entire 
				       group (4 subgroups).  Bits 0-31 are for
				       injection fifos.  See MUHWI_InjFifoStatus_t.
				       Bits 32-47 indicate the reception fifos
				       the group had a free-space threshold 
				       down-crossing (free-space has become 
				       smaller than the free-space specified
				       in the reception fifo threshold DCR 
				       register.  Bits 48-63 indicate the 
				       reception fifos in the group received a
				       packet with the interrupt bit set.
				       There are 4 bits per subgroup, one bit 
				       for each reception fifo in that subgroup. */
  volatile uint64_t     clearInterruptStatus; /**< Fifo clear interrupt status.
				       This has clear interrupt bits for the entire 
				       group (4 subgroups).  Bits 32-47 are for
				       clearing the threshold crossing interrupt
				       for each of the reception fifos.  Bits
				       48-63 are for clearing the packet arrival
				       interrupt for each of the reception fifos.
				       Setting a bit to 1 clears the interrupt 
				       (clears the corresponding bit in the 
				       interruptStatus).
				       Bits 0-31 are for injection fifos.
				       See MUHWI_InjFifoStatus_t.
				       There are 4 bits per subgroup, one bit 
				       for each reception fifo in that subgroup. */
} MUHWI_RecFifoStatus_t;


#endif
