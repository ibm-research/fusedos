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
/* (C) Copyright IBM Corp.  2011, 2012                              */
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

//! \file  JobctlMessages.h
//! \brief Declarations for bgcios::jobctl message classes.

#ifndef JOBCTLMESSAGES_H
#define JOBCTLMESSAGES_H

// Includes
#include <ramdisk/include/services/MessageHeader.h>
#include <hwi/include/bqc/classroute.h>
#include <inttypes.h>
#include <sys/types.h>
#include <netinet/in.h>

namespace bgcios
{

namespace jobctl
{

const uint16_t ErrorAck           = 2000;
const uint16_t Authenticate       = 2001;
const uint16_t AuthenticateAck    = 2002;
const uint16_t LoadJob            = 2003;
const uint16_t LoadJobAck         = 2004;
const uint16_t StartJob           = 2005;
const uint16_t StartJobAck        = 2006;
const uint16_t StartTool          = 2007;
const uint16_t StartToolAck       = 2008;
const uint16_t EndTool            = 2009;
const uint16_t EndToolAck         = 2010;
const uint16_t SignalJob          = 2011;
const uint16_t SignalJobAck       = 2012;
const uint16_t CleanupJob         = 2013;
const uint16_t CleanupJobAck      = 2014;
const uint16_t Reconnect          = 2015;
const uint16_t ReconnectAck       = 2016;
const uint16_t ExitJob            = 2017;
const uint16_t ExitJobAck         = 2018;
const uint16_t DiscoverNode       = 2019;
const uint16_t DiscoverNodeAck    = 2020;
const uint16_t SetupJob           = 2021;
const uint16_t SetupJobAck        = 2022;
const uint16_t ExitProcess        = 2023;
const uint16_t ExitProcessAck     = 2024;
const uint16_t ChangeConfig       = 2025;
const uint16_t ChangeConfigAck    = 2026;
const uint16_t CheckToolStatus    = 2027;
const uint16_t CheckToolStatusAck = 2028;
const uint16_t ExitTool           = 2029;
const uint16_t ExitToolAck        = 2030;
const uint16_t Heartbeat          = 2031;
const uint16_t HeartbeatAck       = 2032;
const uint16_t Completed          = 2099; // Always last since there is not a corresponding ack message.


//! Base port number for RDMA connections.
const uint16_t BaseRdmaPort = 7100;

//! Current version of protocol.
const uint8_t ProtocolVersion = 9;

//! Size of unencrypted (plain text) data in Authenticate message.
const size_t PlainDataSize = 32;

//! Size of encrypted data in Authenticate message.
const size_t EncryptedDataSize = 64;

//! Maximum number of bytes for argument list.
const int MaxArgumentSize = 4096;

//! Maximum number of bytes for environment variable list.
const int MaxVariableSize = 8192;

//! Maximum number of bytes for path name.
const int MaxPathSize = 512;

//! Maximum number of secondary groups.
const int MaxGroups = 64;

//! Maximum number of compute nodes that can be serviced by one I/O node.
const size_t MaxComputeNodes = 512;

//! Maximum number of bytes for fields in struct utsname used by uname system call.
const size_t MaxUtsnameSize = 65;

//! Special value to disable system call tracing.
const int32_t DisableStrace = -1;

//! Special signal number to indicate hardware failure.
const int SIGHARDWAREFAILURE = 35;

//! \brief  Convert torus coordinates of node to a 32-bit node identifier.
//! \param  aCoord  A coordinate of node.
//! \param  bCoord  B coordinate of node.
//! \param  cCoord  C coordinate of node.
//! \param  dCoord  D coordinate of node.
//! \param  eCoord  E coordinate of node.
//! \return Node identifier value.

inline uint32_t coordsToNodeId(uint32_t aCoord, uint32_t bCoord, uint32_t cCoord, uint32_t dCoord, uint32_t eCoord)
{
   return ( ((aCoord & 0x3F) << 24) | ((bCoord & 0x3F) << 18) | ((cCoord & 0x3F) << 12) | ((dCoord & 0x3F) << 6) | (eCoord & 0x3F) );
}

//! Node torus coordinates.
struct NodeCoordinates
{
   uint8_t aCoord;                     //!< Torus 'a' coordinate.
   uint8_t bCoord;                     //!< Torus 'b' coordinate.
   uint8_t cCoord;                     //!< Torus 'c' coordinate.
   uint8_t dCoord;                     //!< Torus 'd' coordinate.
   uint8_t eCoord;                     //!< Torus 'e' coordinate.
   uint8_t core;                       //!< Core number.
};

//! Node class route settings.
struct NodeClassRoute
{
   struct NodeCoordinates coords;      //!< Torus coordinates of the node.
   ClassRoute_t subBlockClassRoute;       //!< Route for all active and inactive nodes in the job
   ClassRoute_t npClassRoute;          //!< Route for all active nodes in the job
};

//! Message to acknowledge a message that is in error.

struct ErrorAckMessage
{
   struct MessageHeader header;        //!< Message header.
};

//! Message to authenticate connection.

struct AuthenticateMessage
{
   struct MessageHeader header;                    //!< Message header.
   unsigned char plainData[PlainDataSize];         //!< Unencrypted (plain text) data.
   unsigned char encryptedData[EncryptedDataSize]; //!< Encrypted data.
};

//! Message to acknowledge authenticating connection.

struct AuthenticateAckMessage
{
   struct MessageHeader header;        //!< Message header.
};

//! Message to load a job on compute nodes.   

struct LoadJobMessage
{
   struct MessageHeader header;             //!< Message header.
   uid_t userId;                            //!< User id.
   gid_t groupId;                           //!< Primary group id.
   uint16_t numSecondaryGroups;             //!< Number of secondary group ids.                                         
   gid_t secondaryGroups[MaxGroups];        //!< List of secondary group ids.
   mode_t umask;                            //!< Initial value of umask.
   uint32_t rankForStdin;                   //!< Rank that is allowed to read from stdin.
   int32_t rankForStrace;                   //!< Rank that has system call tracing enabled (-1 to disable strace).
   uint16_t numArguments;                   //!< Number of arguments.
   uint16_t argumentSize;                   //!< Number of valid bytes in arguments array.
   uint16_t numVariables;                   //!< Number of environment variables.
   uint16_t variableSize;                   //!< Number of valid bytes in variables array.
   char arguments[MaxArgumentSize];         //!< List of arguments (first argument is executable path).
   char variables[MaxVariableSize];         //!< List of environment variables.
   char workingDirectoryPath[MaxPathSize];  //!< Path to initial working directory.
};

//! Message to acknowledge loading a job on compute nodes.

struct LoadJobAckMessage
{
   struct MessageHeader header;        //!< Message header.
};

//! Message to start a job on compute nodes.

struct StartJobMessage
{
   struct MessageHeader header;        //!< Message header.
   uint32_t numRanksForIONode;         //!< Number of ranks using services on IO node.
   uint64_t currentTime;               //!< Current time of day.
};

//! Message to acknowledge starting a job on compute nodes.

struct StartJobAckMessage
{
   struct MessageHeader header;        //!< Message header.
};

//! Message to start a tool on I/O nodes.

struct StartToolMessage
{
   struct MessageHeader header;        //!< Message header.
   uint32_t toolId;                    //!< Unique identifier for tool.
   uint16_t numArguments;              //!< Number of arguments.
   char arguments[MaxArgumentSize];    //!< List of arguments (first argument is executable path).
};

//! Message to acknowledge starting a tool on I/O nodes.

struct StartToolAckMessage
{
   struct MessageHeader header;        //!< Message header.
   uint32_t toolId;                    //!< Unique identifier for tool.
};

//! Message to end a tool on I/O nodes.

struct EndToolMessage
{
   struct MessageHeader header;        //!< Message header.
   uint32_t toolId;                    //!< Unique identifier for tool.
   int signo;                          //!< Signal number to send to end tool.
};

//! Message to acknowledge ending a tool on I/O nodes.

struct EndToolAckMessage
{
   struct MessageHeader header;        //!< Message header.
   uint32_t toolId;                    //!< Unique identifier for tool.
};

//! Message to send a signal to the compute nodes in a job.

struct SignalJobMessage
{
   struct MessageHeader header;        //!< Message header.
   int signo;                          //!< Signal number to send.
};

//! Message to acknowledge sending a signal to the compute nodes.

struct SignalJobAckMessage
{
   struct MessageHeader header;        //!< Message header.
};

//! Message to end job and cleanup resources on compute nodes and I/O nodes.

struct CleanupJobMessage
{
   struct MessageHeader header;        //!< Message header.
};

//! Message to acknowledge cleaning up job.

struct CleanupJobAckMessage
{
   struct MessageHeader header;        //!< Message header.
   uint32_t epilogReturnCode;          //!< Result from job epilog program.
   uint32_t epilogErrorCode;           //!< Error detail from job epilog program.
};

//! Message to reconnect to a running I/O node.

struct ReconnectMessage
{
   struct MessageHeader header;        //!< Message header.
   uint32_t numJobs;                   //!< Number of jobs loaded or started on I/O node.
};

//! Message to acknowledge reconnecting to a running I/O node.

struct ReconnectAckMessage
{
   struct MessageHeader header;        //!< Message header.
};

//! Message to report exit status of job.

struct ExitJobMessage
{
   struct MessageHeader header;        //!< Message header.
   int status;                         //!< Exit status of job.
   time_t endTime;                     //!< Time job ended.
};

//! Message to acknowledge reporting exit status of job.

struct ExitJobAckMessage
{
   struct MessageHeader header;        //!< Message header.
};

//! Message to discover a compute node.

struct DiscoverNodeMessage
{
   struct MessageHeader header;        //!< Message header.
   struct NodeCoordinates coords;      //!< Torus coordinates of the node.
   uint32_t blockId;                   //!< Block identifier.
   uint64_t uci;                       //!< Universal component identifier of node.
   struct NodeCoordinates bridge;      //!< Torus coordinates of bridge node with I/O link.
};

//! Message to acknowledge discovering a compute node.

struct DiscoverNodeAckMessage
{
   struct MessageHeader header;        //!< Message header.
   uint32_t serviceId;                 //!< Unique identifier for connecting to I/O services.
   in_addr_t deviceAddress;            //!< Address of device for connecting to I/O services.
   uint32_t  sysiod_port;              //!< port target of sysiod for connecting to function-ship services
   uint32_t  toolctld_port;            //!< port target of tooltcltd for connecting to toolctld services
   uint64_t currentTime;               //!< Current time of day.
   char nodeName[MaxUtsnameSize];      //!< Node name string returned by uname system call.
   char release[MaxUtsnameSize];       //!< Release string returned by uname system call.
};

//! Message to setup for a job on compute nodes and I/O nodes.

struct SetupJobMessage
{
   struct MessageHeader header;        //!< Message header.
   uint32_t blockId;                   //!< Block identifier.
   struct NodeCoordinates jobLeader;   //!< Torus coordinates of job leader compute node.
   struct NodeCoordinates corner;      //!< Torus coordinates of corner compute node in job.
   struct NodeCoordinates shape;       //!< Size of each torus dimension in job.
   char mapping[6];                    //!< Predefined mapping (e.g. "ABCDET").
   char mapFilePath[MaxPathSize];      //!< Path to node rank map file.
   uint32_t numNodes;                  //!< Number of nodes in NodeClassRoute array.
   struct NodeClassRoute nodesInJob[MaxComputeNodes]; //!< "Nodes in job" class route settings for each compute node using I/O node.
   uint32_t numRanks;                  //!< Number of ranks participating in job.
   uint16_t numRanksPerNode;           //!< Number of ranks running on each node.
};

//! Message to acknowledge setting up for a job on a set of compute nodes.

struct SetupJobAckMessage
{
   struct MessageHeader header;        //!< Message header.
};

//! Message to report exit status of one process.

struct ExitProcessMessage
{
   struct MessageHeader header;        //!< Message header.
   int status;                         //!< Exit status of process.
   uint32_t agent;                     //!< 0:normal process 1:AppAgent1 2:AppAgent2
};

//! Message to acknowledge reporting exit status of one process.

struct ExitProcessAckMessage
{
   struct MessageHeader header;        //!< Message header.
};

//! Message to change the configuration of the daemon.

struct ChangeConfigMessage
{
   struct MessageHeader header;        //!< Message header.
   char jobctldTraceLevel;             //!< New ibm.cios.jobctld trace level.
   char commonTraceLevel;              //!< New ibm.cios.common trace level.
};

//! Message to acknowledge changing the configuration of the daemon.

struct ChangeConfigAckMessage
{
   struct MessageHeader header;        //!< Message header.
};

//! Message to check status of currently active tools.

struct CheckToolStatusMessage
{
   struct MessageHeader header;        //!< Message header.
   uint32_t seconds;                   //!< Previous number of seconds to check for tool activity.
};

//! Message to acknowledge checking status of currently active tools.

struct CheckToolStatusAckMessage
{
   struct MessageHeader header;        //!< Message header.
   uint32_t activeTools;               //!< Number of tools that are still active.
};

//! Message to report exit status of a tool.

struct ExitToolMessage
{
   struct MessageHeader header;        //!< Message header.
   uint32_t toolId;                    //!< Unique identifier for tool. 
   int status;                         //!< Exit status of tool.
};

//! Message to acknowledge reporting exit status of a tool.

struct ExitToolAckMessage
{
   struct MessageHeader header;        //!< Message header.
   uint32_t toolId;                    //!< Unique identifier for tool.
};

//! Message to indicate previous message exchange has completed.

struct CompletedMessage
{
   struct MessageHeader header;        //!< Message header.
};

//! Message to check status of jobctld

struct HeartbeatMessage
{
   struct MessageHeader header;        //!< Message header.
};

//! Message to acknowledge heartbeat

struct HeartbeatAckMessage
{
   struct MessageHeader header;        //!< Message header.
};

} // namespace jobctl

} // namespace bgcios

#endif // JOBCTLMESSAGES_H


