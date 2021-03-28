#ident	"@(#)msgsyscall.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)msgsyscall.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	MOT:msgsys.s	1.3

# C library -- msgsys

# 	trap vectors
	_m4_include_(SYSTRAP_H)
	
	file	"msgsyscall.s"
	global	msgsys
	global  cerror%

msgsys:
	MCOUNT
	mov.l	&msgsys%,%d0
	trap	&0
	bcc.b 	noerror
	jmp 	cerror%
noerror:
	rts
