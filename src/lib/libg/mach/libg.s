#	START NEW ARIX SCCS HEADER
#
#	@(#) libg.s: version 25.1 created on 12/2/91 at 19:31:23
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	ident	"@(#)libg.s	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
#
#	END NEW ARIX SCCS HEADER
#
	ident	"@(#)libg.s	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
#	dbxxx.s	2.1 
#	ident	"@(#)libg:libg.s	25.1"
	file	"dbxxx.s"
        comm   _dbargs,512

        text
        global  _dbsubc
        global  _dbsubn
        global  _dbsubx

_dbsubc:
	link	%fp,&0
        mov.l   &_dbargs,%a0
	mov.l	(%a0)+,%a1	# address of procedure
	mov.l	(%a0)+,%d0	# number of arguments
	mov.l	%d0,%d1
	asl.l	&2,%d1
	add.l	%d1,%a0		# arguments in table are backwards
	bra.b	L%2
L%1:	mov.l	-(%a0),-(%sp)	# move arguments onto stack
L%2:	dbra	%d0,L%1
	jsr	(%a1)		# enter procedure
	unlk	%fp		# clean up stack

_dbsubn:
        trap	&1		# breakpoint - return to sdb
_dbsubx:
	nop
