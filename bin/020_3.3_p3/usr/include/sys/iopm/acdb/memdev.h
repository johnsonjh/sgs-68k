/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) memdev.h: version 24.1 created on 10/28/91 at 17:39:10	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)memdev.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#ident "@(#)memdev.h   24.1"
/*
 *	memdev.h
 */
/*------------------------------------------------------------------------*/
#define START_OF_SRAM_CODE	(unchar *)0xf0010000
#define START_OF_OCTARTS	(unchar *)0xf0030000
#define ACE_ID			(unchar *)0xf0030280
#define START_OF_SRAM_SPEC_TAB	(unchar *)0xf0040000
#define START_OF_SRAM_BUF	(unchar *)0xf005e000
#define START_OF_SRAM_INPUT	(unchar *)0xf0060000
#define SRAM_CONTROL_REG 	(unchar *)0xf0080000
#define SRAM_PC_REG 		(unchar *)0xf0080001

/*------------------------------------------------------------------------*/
#define SRAM_CODE_SIZE		0x800
#define ACE_SIZE		0x200	/* size of ace */
#define OCTART_SIZE		0x100	/* size of octart */
#define TYD_NUM_PORTS		64
#define TYD_PORTS_PER_ACE	16
/*------------------------------------------------------------------------*/
/*
 *	SRAM_CONTROL_REG bits
 */
#define SRAM_CTL_SEQEN		1	/*let uSeq access uSeq code */
#define SRAM_CTL_ASYNC		2	/*let uSeq Octart interupts*/
#define SRAM_CTL_RESET		4	/*disable device board reset*/
#define SRAM_CTL_HALT		8	/*stop uSeq from running */
#define SRAM_CTL_STEP		0x10	/*toggling to 1 lets uSeq do 1 step */
#define SRAM_CTL_PC_TO_0	0x20	/*allow pc counter to advance*/
/*------------------------------------------------------------------------*/
