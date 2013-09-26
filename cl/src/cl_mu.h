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

// cl_mu.h

#ifndef __CL_MU_H
#define __CL_MU_H


#include <hwi/include/bqc/MU_Macros.h>
#include <hwi/include/bqc/classroute.h>

#ifndef __CL_MU_EXTERN_
#define __CL_MU_EXTERN_ extern
#endif

__CL_MU_EXTERN_ int inj_fifo_fds[ BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE * BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP ];
__CL_MU_EXTERN_ int rec_fifo_fds[ BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE * BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP ];
__CL_MU_EXTERN_ int bat_fds[ BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE * BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP ];
__CL_MU_EXTERN_ int coll_cr_fds[ BGQ_COLL_CLASS_MAX_CLASSROUTES ];
__CL_MU_EXTERN_ int gi_cr_fds[ BGQ_GI_CLASS_MAX_CLASSROUTES ];


#define __CL_MU_INLINE static inline

__CL_MU_INLINE int _get_inj_fifo_fd(uint32_t fifo_id)
{
    return inj_fifo_fds[ fifo_id ];
}

__CL_MU_INLINE void _set_inj_fifo_fd(uint32_t fifo_id, int fd)
{
    inj_fifo_fds[ fifo_id ] = fd;
}

__CL_MU_INLINE int get_inj_fifo_fd(uint32_t subgrid, uint32_t fifo_idx_in_subgrp)
{
    return _get_inj_fifo_fd( subgrid * BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP + fifo_idx_in_subgrp);
}

__CL_MU_INLINE void set_inj_fifo_fd(uint32_t subgrid, uint32_t fifo_idx_in_subgrp, int fd)
{
    _set_inj_fifo_fd(subgrid * BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP + fifo_idx_in_subgrp, fd);
}

__CL_MU_INLINE int have_inj_fifo_fd(uint32_t subgrid, uint32_t fifo_idx_in_subgrp)
{
    return get_inj_fifo_fd(subgrid, fifo_idx_in_subgrp) != -1;
}


__CL_MU_INLINE int _get_rec_fifo_fd(uint32_t fifo_id)
{
    return rec_fifo_fds[ fifo_id ];
}

__CL_MU_INLINE void _set_rec_fifo_fd(uint32_t fifo_id, int fd)
{
    rec_fifo_fds[ fifo_id ] = fd;
}


__CL_MU_INLINE int get_rec_fifo_fd(uint32_t subgrid, uint32_t fifo_idx_in_subgrp)
{
    return _get_rec_fifo_fd( subgrid * BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP + fifo_idx_in_subgrp);
}

__CL_MU_INLINE int get_rec_fifo_fd_grp(uint32_t groupid, uint32_t fifo_idx_in_grp)
{
	return _get_rec_fifo_fd(groupid * BGQ_MU_NUM_REC_FIFOS_PER_GROUP + fifo_idx_in_grp);
}

__CL_MU_INLINE void set_rec_fifo_fd(uint32_t subgrid, uint32_t fifo_idx_in_subgrp, int fd)
{
    _set_rec_fifo_fd(subgrid * BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP + fifo_idx_in_subgrp, fd);
}

__CL_MU_INLINE int have_rec_fifo_fd(uint32_t subgrid, uint32_t fifo_idx_in_subgrp)
{
    return get_rec_fifo_fd(subgrid, fifo_idx_in_subgrp) != -1;
}

__CL_MU_INLINE int have_rec_fifo_fd_grp(uint32_t groupid, uint32_t fifo_idx_in_grp)
{
	return get_rec_fifo_fd_grp(groupid, fifo_idx_in_grp) != -1;
}


__CL_MU_INLINE int _get_bat_fd(uint32_t bat_id)
{
    return bat_fds[ bat_id ];
}

__CL_MU_INLINE void _set_bat_fd(uint32_t bat_id, int fd)
{
    bat_fds[ bat_id ] = fd;
}


__CL_MU_INLINE int get_bat_fd(uint32_t subgrid, uint32_t bat_idx_in_subgrp)
{
    return _get_bat_fd( subgrid * BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP + bat_idx_in_subgrp);
}

__CL_MU_INLINE void set_bat_fd(uint32_t subgrid, uint32_t bat_idx_in_subgrp, int fd)
{
    _set_bat_fd(subgrid * BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP + bat_idx_in_subgrp, fd);
}

__CL_MU_INLINE int have_bat_fd(uint32_t groupid, uint32_t bat_idx_in_grp)
{
	return get_bat_fd(groupid, bat_idx_in_grp) != -1;
}


__CL_MU_INLINE int get_coll_cr_fd(uint32_t coll_cr_id)
{
	return coll_cr_fds[coll_cr_id];
}

__CL_MU_INLINE void set_coll_cr_fd(uint32_t coll_cr_id, int fd)
{
	coll_cr_fds[coll_cr_id] = fd;
}

__CL_MU_INLINE int have_coll_cr_fd(uint32_t coll_cr_id)
{
	return get_coll_cr_fd(coll_cr_id) != -1;
}

__CL_MU_INLINE int get_gi_cr_fd(uint32_t gi_cr_id)
{
	return gi_cr_fds[gi_cr_id];
}

__CL_MU_INLINE void set_gi_cr_fd(uint32_t gi_cr_id, int fd)
{
	gi_cr_fds[gi_cr_id] = fd;
}

__CL_MU_INLINE int have_gi_cr_fd(uint32_t gi_cr_id)
{
	return get_gi_cr_fd(gi_cr_id) != -1;
}
#endif
