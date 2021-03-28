/*
 *  @(#) Copyright (c) 1987  The Wollongong Group, Inc.  All Rights Reserved
 */

#ident "@(#)win_hook.h  (TWG)      1.4     89/07/20 "

#define WIN_HOOK_NODE "/dev/win-net"
#define WIN_HOOK_MASK ('W' << 8)

/*
 * System Call hooks for the psuedo driver.
 */
#define WIN_SELECT	0
#define WIN_READ	1
#define WIN_WRITE	2
#define WIN_EXEC	3
#define WIN_CLOSE	4
#define WIN_FCNTL	5
#define WIN_GETLOAD	6
#define N_WIN_SYSCALLS	6

#if !(KERNEL || INKERNEL)
extern int __win_hook_fd;
#endif
