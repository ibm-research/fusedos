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
#include "Kernel.h"

#if defined __FUSEDOS__                                                                                               // FUSEDOS
#include <grp.h>                                                                                                       // FUSEDOS
#endif                                                                                                                 // FUSEDOS

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
uint64_t sc_NoSys(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}



uint64_t sc__sysctl(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_acct(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_add_key(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_adjtimex(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_afs_syscall(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_bdflush(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_break(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_capget(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_capset(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_chroot(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_clock_getres(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_clock_gettime(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_clock_nanosleep(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_clock_settime(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_create_module(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_delete_module(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_epoll_create(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_epoll_ctl(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_epoll_wait(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_execve(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_fadvise64(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_fchdir(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_fdatasync(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_fork(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_ftime(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_get_kernel_syms(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}



uint64_t sc_getpgid(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_getpgrp(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_getpmsg(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_getppid(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_getpriority(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_getresgid(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_getresuid(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_getsid(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_gtty(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_idle(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_init_module(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_io_cancel(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_io_destroy(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_io_getevents(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_io_setup(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_io_submit(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_ioperm(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_iopl(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_ipc(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_kexec_load(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_keyctl(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_lock(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_lookup_dcookie(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_mincore(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_mknod(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_modify_ldt(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_mount(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_mpx(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_mq_getsetattr(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_mq_notify(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_mq_open(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_mq_timedreceive(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_mq_timedsend(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_mq_unlink(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_multiplexer(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_nfsservctl(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_nice(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_oldfstat(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_oldlstat(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_oldolduname(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_oldstat(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_olduname(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_pciconfig_iobase(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_pciconfig_read(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_pciconfig_write(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_personality(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_pipe(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_pivot_root(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_prof(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_profil(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_ptrace(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_putpmsg(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_query_module(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_quotactl(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_readahead(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_readdir(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_reboot(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_remap_file_pages(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_request_key(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_rtas(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_sched_rr_get_interval(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_select(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_sendfile(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_sendfile64(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_setdomainname(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_setfsgid(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_setfsuid(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}
#endif                                                                                                                 // FUSEDOS


uint64_t sc_setgroups(SYSCALL_FCN_ARGS)
{
#if defined __FUSEDOS__                                                                                               // FUSEDOS
        size_t size = (size_t)r3;                                                                                      // FUSEDOS
        const gid_t* list =  (const gid_t*)r4;                                                                         // FUSEDOS
                                                                                                                       // FUSEDOS
        TRACESYSCALL(("(I) %s%s: size=%lu, list=0x%p\n", __func__, whoami(), size, list));                             // FUSEDOS
                                                                                                                       // FUSEDOS
        int rc = setgroups(size, list);                                                                                // FUSEDOS
        if (rc == -1) {                                                                                                // FUSEDOS
            return CNK_RC_FAILURE(errno);                                                                              // FUSEDOS
        } else {                                                                                                       // FUSEDOS
            return CNK_RC_SUCCESS(0);                                                                                  // FUSEDOS
        }                                                                                                              // FUSEDOS
#else                                                                                                                  // FUSEDOS
	return CNK_RC_FAILURE(ENOSYS);
#endif                                                                                                                 // FUSEDOS
}


#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
uint64_t sc_sethostname(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_setpgid(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_setpriority(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_setregid(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_setresgid(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_setresuid(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_setreuid(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_setsid(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_settimeofday(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_stime(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_stty(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_swapcontext(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_swapoff(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_swapon(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_sync(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_sys_debug_setcontext(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_sysfs(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_sysinfo(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_syslog(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_timer_create(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_timer_delete(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_timer_getoverrun(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_timer_gettime(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_timer_settime(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}

uint64_t sc_tuxcall(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_ulimit(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_umount(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_umount2(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_uselib(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_ustat(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_utimes(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_vfork(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_vhangup(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_vm86(SYSCALL_FCN_ARGS)
{
	return CNK_RC_FAILURE(ENOSYS);
}

uint64_t sc_undefined224(SYSCALL_FCN_ARGS)
{
    return CNK_RC_FAILURE(ENOSYS);
}

uint64_t sc_undefined257(SYSCALL_FCN_ARGS)
{
    return CNK_RC_FAILURE(ENOSYS);
}

uint64_t sc_undefined258(SYSCALL_FCN_ARGS)
{
    return CNK_RC_FAILURE(ENOSYS);
}

uint64_t sc_undefined259(SYSCALL_FCN_ARGS)
{
    return CNK_RC_FAILURE(ENOSYS);
}

uint64_t sc_undefined260(SYSCALL_FCN_ARGS)
{
    return CNK_RC_FAILURE(ENOSYS);
}

uint64_t sc_undefined261(SYSCALL_FCN_ARGS)
{
    return CNK_RC_FAILURE(ENOSYS);
}
#endif                                                                                                                 // FUSEDOS
