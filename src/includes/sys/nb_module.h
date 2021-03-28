/*
 * (C) Copyright 1989, The Wollongong Group, All rights reserved.
 */

#ident "@(#)nb_module.h (TWG)        1.1      14:02:02 - 89/08/25"

#ifndef SYS_NB_MODULE_H
#define SYS_NB_MODULE_H

/* Debug print macro */
#ifdef DEBUG
#define NB_DEBUG_PRINTF(a, b)		if (netbios_debug & a) printf b
#else
#define NB_DEBUG_PRINTF(a, b)
#endif
#define NBSYSCALL   0x0001   /* trace syscall functions */
#define NBTOPDLS    0x0002   /* trace top DL state changes */
#define NBTOPWPUT   0x0004   /* trace top write put procedure */
#define NBTOPWSRV   0x0008   /* trace top write service procedure */
#define NBBOTWSRV   0x0010   /* trace bottom write service procedure */
#define NBBOTRPUT   0x0020   /* trace bottom read put procedure */
#define NBBOTRSRV   0x0040   /* trace bottom read service procedure */
#define NBCIRCUIT   0x0080   /* trace X.25 circuit procedures */
#define NBRECV      0x0100   /* trace receive from lower stream */
#define NBUSER      0x0200   /* trace user/daemon messages */
#define NBBOTDL     0x0400   /* trace bottom DL state changes */
#define NBSCHED     0x0800   /* trace X.25 circuit scheduler */
#define NBBOOKREQ   0x1000   /* trace directory requests */
#define NBCONCLOSE  0x2000   /* trace circuit shutdown actions */
#define NBCONOPEN   0x4000   /* trace circuit open actions */
#define NBFATAL     0x8000   /* trace serious errors */

/*
 * netbios module information
 */
#define NBMAXPKT	1024
#define NBHIWAT		(NBMAXPKT*8)
#define NBLOWAT		(NBMAXPKT*2)

#define NB_UDP_ID	0x2A01		/* stream logging ID - udp */
#define NB_TCP_ID	0x2A02		/* stream logging ID - tcp */

/* Top queue data structure */
typedef struct netbios {
    queue_t		*nb_qptr;
    queue_t		*nb_botq;
    struct nb_addr	nb_address;
    char		nb_type;
    unsigned char	nb_mdev;
    unsigned char	nb_state;
} netbios_t;

#define NB_SESSION	0x00
#define NB_DATAGRAM	0x01

/* Bottom queue data structure */
typedef struct netbiosb {
    queue_t		*nbb_qptr;
    queue_t		*nbb_topq;
    struct sockaddr_in	nbb_sockaddr;
    unsigned long	nbb_cookie;
} netbiosb_t;

#endif
