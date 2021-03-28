#ifndef SYS_SBUS_IOPM_H
#define SYS_SBUS_IOPM_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) sbus_iopm.h: version 24.1 created on 10/28/91 at 18:37:36	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)sbus_iopm.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif

#define IOPM_DB_BASE        (uint)0xf0000000	/* start of dev board */
#define IOPM_DB_ID_REG      (uint)0xf0000000	/* start of dev board */
#define IOPM_RAM_START      (uint)0xf8000000	/* start of IOPM memory */
#define MAP_BASE_REG        (uint)0xfc000000	/* R/W char */
#define IOPM_COUNTER        (ushort *)0xfc000002	/* R short */
#define CONFIG_REG          (ushort *)0xfc800000	/* R/W short */
#define CSS_PAR_REG         (uchar *)0xfd000000	/* W char - diag */
#define STATUS_CTRL_REG     (ushort *)0xfd000000	/* R short */
#define LED_CTRL_REG  	    (uchar *)0xfd800000	/* W char */
#define ERROR_STAT_REG      (uchar *)0xfd800000	/* R char */
#define PIPE_DIAG_DATA_REG  (uint *)0xfe800000
#define PIPE_DIAG_ADD_REG   (uint *)0xfe800040
#define LOWER_LAST_RESP     (uint *)0xfe800080
#define UPPER_LAST_RESP     (uint *)0xfe8000a0
#define INT_CTRL_REG  	    (uchar *)0xfffffffc	/* W char */
#define INT_REG_OFF         (uint)0x000ffffc	/* off into last meg of IOPM */
#define BOARD_ID_REG  	    (uchar *)0xffffffff	/* R char or long */

/* CONFIG_REG - 0xfc800000 - R/W short - configuration control register */
#define MEM_PAR_POL_0  0x0001
#define MEM_PAR_POL_1  0x0002
#define MEM_PAR_POL_2  0x0004
#define MEM_PAR_POL_3  0x0008
#define CSS_CONTR_WR   0x0010
#define DIS_PROM       0x0020
#define EXP_LOOPB      0x0040
#define DIS_CMD_SEQ    0x0080
#define FLUSH_PIPE_OUT 0x0100
#define FLUSH_PIPE_IN  0x0200
#define ENA_DEV_BD     0x0400
#define PROTECT_RAM    0x0800
#define DO_CSS_RESET   0x1000
#define DEV_RST_OUT    0x2000
#define CLR_ERR        0x4000

/* LED_CTRL_REG - led control register */
#define READY_LED_ON  0x0c
#define READY_LED_OFF 0x04
#define FAULT_LED_ON  0x05
#define FAULT_LED_OFF 0x0d
#define LED2_ON       0x06	/* this disagrees with the doc but */
#define LED2_OFF      0x0e	/* agrees with the label on the card */
#define LED1_ON       0x07
#define LED1_OFF      0x0f

/* INT_CTRL_REG - interrupt control register */
#define INTR1    0x01
#define INTR2    0x02
#define INTR3    0x03
#define INTR4    0x04
#define INTR5    0x05
#define INTR6    0x06
#define INTR7    0x07
#define INTR_ON  0x08
#define INTR_OFF 0x00

/* STATUS_CTRL_REG - 0xfd000000 - status register */
#define CONSOLE_MODE 0x0002
#define INTERFACE_EN 0x0004
#define IN_CSS       0x0008
#define SLOT_MASK    0x00f0
#define CMD2_OUT     0x0100
#define CMD1_OUT     0x0200
#define RX_CMD       0x0400
#define NO_DIAG_BAG  0x0800
#define SOFT_INTR_7  0x1000
#define NOT_SYS_FRZ  0x2000
#define NOT_DEV_NMI  0x4000
#define DEV_SUSP     0x8000

/* ERROR_STAT_REG - 0xfd800000 - error register - all lo active */
#define MEM_PRO_ERR 0x01
#define DEV_BERROR  0x01
#define MEM_PERROR  0x04
#define NACK_RCVED  0x08
#define NO_ACK      0x10
#define NACK_SENT   0x20
#define CS_TIMEOUT  0x40
#define UP_TIMEOUT  0x80

/* IOPM Device Board ID numbers */
#define IOPM_DB_ID_DSDB		0x81
#define IOPM_DB_ID_DSDB_FIX	0x82
#define IOPM_DB_ID_ACDB		0x02
#define IOPM_DB_ID_LAN_WAN	0x03
#define IOPM_DB_ID_IOPM_TEST1	0x04
#define IOPM_DB_ID_IOPM_TEST2	0x84
#define IOPM_DB_ID_ACDB_38K	0x06

#define IOPM_NO_DB		0xff

#endif /* SYS_SBUS_IOPM_H */
