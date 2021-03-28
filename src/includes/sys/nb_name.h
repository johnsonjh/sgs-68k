/*
 * (C) Copyright 1989, The Wollongong Group, All rights reserved.
 */

#ident "@(#)nb_name.h (TWG)        1.1      14:02:15 - 89/08/25"

#ifndef SYS_NB_MSG_H
#define SYS_NB_MSG_H

/* Command types */
#define	NBU_CMD_OPEN	0x01
#define	NBU_CMD_CLOSE	0x02
#define	NBU_CMD_JOIN	0x03

typedef struct nb_msg {
    unsigned long nbu_command;
    unsigned long nbu_type;
    unsigned long nbu_id;
    unsigned long nbu_error;
} nb_msg_t;

#endif
