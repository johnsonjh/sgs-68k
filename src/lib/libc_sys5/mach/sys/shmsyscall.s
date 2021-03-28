	#ident	"@(#)shmsyscall.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
		ident	"@(#)shmsyscall.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	file	"shmsyscall.s"
#	MOT:shmsys.a68	1.3

# C library -- shmsys

	set	shmsys%,52
	global	shmsys
	global  cerror%

shmsys:
	MCOUNT
	mov.l	&shmsys%,%d0
	trap	&0
	bcc.b 	noerror
	jmp 	cerror%
noerror:
	rts
