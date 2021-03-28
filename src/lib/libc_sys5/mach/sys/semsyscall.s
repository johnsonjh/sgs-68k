	#ident	"@(#)semsyscall.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
		ident	"@(#)semsyscall.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	file	"semsyscall.s"
#	MOT:semsys.a68	1.3

# C library -- semsys

	set	semsys%,53
	global	semsys
	global  cerror%

semsys:
	MCOUNT
	mov.l	&semsys%,%d0
	trap	&0
	bcc.b 	noerror
	jmp 	cerror%
noerror:
	rts
