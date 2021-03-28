	#ident	"@(#)sysconfa.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
		ident	"@(#)sysconfa.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	file	"sysconfa.s"



# C library -- sysconf

# 	trap vectors
	_m4_include_(SYSTRAP_H)
	

	global	_sysconf

_sysconf:

	mov.l	&sysconf%,%d0
	trap	&0
	bcc.b 	noerror_s%
	jmp	cerror%
	rts

noerror_s%:
	rts
