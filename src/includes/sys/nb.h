/*
 * (C) Copyright 1989, The Wollongong Group, All rights reserved.
 */

#ident "@(#)nb.h (TWG)        1.1      14:04:02 - 89/08/25"

#ifndef SYS_NB_H
#define SYS_NB_H

/*
 * NETBIOS TLI address format
 */
#define	NB_NAME_SIZE	16
typedef struct nb_addr {
    unsigned char name[NB_NAME_SIZE];
    unsigned char reserved;
    unsigned char flags;
} nb_addr_t;

/* flags defines */
#define NB_GROUP_FLAG	0x80

#endif
