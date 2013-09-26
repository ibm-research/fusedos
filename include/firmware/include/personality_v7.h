/* begin_generated_IBM_copyright_prolog                             */
/*                                                                  */
/* This is an automatically generated copyright prolog.             */
/* After initializing,  DO NOT MODIFY OR MOVE                       */
/*  --------------------------------------------------------------- */
/*                                                                  */
/* Licensed Materials - Property of IBM                             */
/* Blue Gene/Q                                                      */
/* (C) Copyright IBM Corp.  2010, 2012                              */
/* US Government Users Restricted Rights - Use, duplication or      */
/*   disclosure restricted by GSA ADP Schedule Contract with IBM    */
/*   Corp.                                                          */
/*                                                                  */
/* This software is available to you under the Eclipse Public       */
/* License (EPL).                                                   */
/*                                                                  */
/*  --------------------------------------------------------------- */
/*                                                                  */
/* end_generated_IBM_copyright_prolog                               */



#ifndef _PERSONALITY_V7_H_
#define _PERSONALITY_V7_H_


typedef struct Personality_Kernel_v7_t
                {
		fw_uint64_t  UCI;                //!< Universal Component Identifier.
                fw_uint64_t  NodeConfig;         //!< Kernel device and core enables (see PERS_ENABLE_* masks)
                fw_uint64_t  TraceConfig;        //!< Kernel tracing enables
                fw_uint32_t  RASPolicy;          //!< Verbosity level, and other RAS Reporting Controls
                fw_uint32_t  FreqMHz;            //!< Clock_X1 Frequency in MegaHertz (e.g. 1000 for 1GHz)
                fw_uint64_t  ClockStop;          //!< Clockstop value
} Personality_Kernel_v7_t;

typedef struct Personality_DDR_v7_t
                {
                fw_uint32_t  DDRFlags;         //!< Miscellaneous flags and settings (see below)
                fw_uint32_t  SRBS0;            //!< Controller 0 SRBS/CK settings
                fw_uint32_t  SRBS1;            //!< Controller 1 SRBS/CK settings
                fw_uint32_t  SRBS2;            //!< Controller 2 SRBS/CK settings
                fw_uint32_t  SRBS3;            //!< Controller 3 SRBS/CK settings
                fw_uint32_t  Throttle;         //!< DDR throttle
                fw_uint32_t  DDRSizeMB;        //!< Total DDR size in mega bytes (likely 16GB).
                fw_uint16_t  Chips;            //!< Type of DDR chips
                fw_uint16_t  CAS;              //!< CAS Latency (3, 4, or 5)
                }
                Personality_DDR_v7_t;

typedef struct Personality_Networks_v7_t
                {
                fw_uint32_t  BlockID;         //!< a.k.a. PartitionID
                fw_uint64_t  NetFlags;        //!< See below
		fw_uint64_t  NetFlags2;
		fw_uint8_t   Anodes,          //!< The A dimension
          		  Bnodes,          //!< The B dimension
                          Cnodes,          //!< The C dimension
                          Dnodes,          //!< The D dimension
                          Enodes,          //!< The E dimension
                          Acoord,          //!< The A coordinate
                          Bcoord,          //!< The B coordinate
                          Ccoord,          //!< The C coordinate
                          Dcoord,          //!< The D coordinate
                          Ecoord;          //!< The E coordinate

		  struct ClassRoutes {
		    fw_uint16_t GlobIntUpPortInputs;           // Global Interrupt Up Tree Inputs.  Only the 12 LSBs are relevant (11 torus links + 1 internal).
		    fw_uint16_t GlobIntUpPortOutputs;          // Global Interrupt Up Tree Outputs. Only the 12 LSBs are relevant (11 torus links + 1 internal).
		    fw_uint16_t CollectiveTypeAndUpPortInputs; // Collective type and Up Tree Inputs.  See encodings below
		    fw_uint16_t CollectiveUpPortOutputs;       // Collective Up Tree Outputs.  Only the 12 LSBs are relevant (11 torus links + 1 internal).
		  } ClassRoutes[16]; 

		  fw_uint32_t  ZoneRoutingMasks[4];        // Each contains 5 masks.  See bit assignments below
		  fw_uint64_t  MuFlags;  //!< Message Unit flags (see below)
		  fw_uint8_t   cnBridge_A, cnBridge_B, cnBridge_C, cnBridge_D, cnBridge_E; //!< Torus coordinates of compute node bridge
                  fw_uint32_t  latencyFromRoot;  //!< GI Latency from root node in pclk.
                }
                Personality_Networks_v7_t;



typedef struct Personality_Ethernet_v7_t
                {
		  struct {
		    char         name[16];                                //!< e.g. "eth0"
		    fw_uint16_t  MTU;                                     //!< The initial emac MTU size
		    IP_Addr_t    IPAddress;                               //!< The IPv6/IPv4 address of this node
		    IP_Addr_t    IPNetmask;                               //!< The IPv6/IPv4 network mask
		    IP_Addr_t    IPBroadcast;                             //!< The IPv6/IPv4 broadcast address
		    fw_uint8_t   ID[16];
		  } interface[2];
		  IP_Addr_t      IPGateway;                               //!< The IPv6/IPv4 initial gateway (zero if none)
		  IP_Addr_t      NFSServer;                               //!< The IPv6/IPv4 NFS system software server address
		  IP_Addr_t      ServiceNode;                             //!< The IPv6/IPv4 address of service node
		  char           NFSExportDir[PERSONALITY_LEN_NFSDIR];    //!< The NFS export directory.
		  char           NFSMountDir[ PERSONALITY_LEN_NFSDIR];    //!< The NDF mount directory.
		  fw_uint8_t     SecurityKey[ PERSONALITY_LEN_SECKEY ];   //!< Security Key for Service Node authentication
                }
                Personality_Ethernet_v7_t;


typedef struct Personality_v7_t
                {
		fw_uint16_t                 CRC;                   //!< Crc16n starting from .Version
		fw_uint8_t                  Version;               //!< The personality version (see also PERSONALITY_VERSION constant)
                fw_uint8_t                  PersonalitySizeWords;  //!< (sizeof(Personality_t) >> 2)
 	        Personality_Kernel_v7_t     Kernel_Config;         //!< The kernel configuration
		Personality_DDR_v7_t        DDR_Config;            //!< The DDR configuration
		Personality_Networks_v7_t   Network_Config;        //!< The network configurtion
		Personality_Ethernet_v7_t   Ethernet_Config;       //!< The ethernet configuration
                }
                Personality_v7_t;


#endif // Add nothing below this line.
