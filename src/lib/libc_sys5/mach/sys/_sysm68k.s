#ident	"@(#)_sysm68k.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)_sysm68k.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	MOT:_sysm68k.s	2.1	

# C library - sysm68k

	file	"_sysm68k.s"
	set	sysm68k%,50
	global	_sysm68k
	global	cerror%

_sysm68k:
	MCOUNT
	mov.l	&sysm68k%,%d0
	trap	&0
	bcc.b	noerror
	jmp	cerror%
noerror:
	rts
