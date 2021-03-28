#ident	"@(#)_times.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)_times.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
# C library -- _times

	file	"_times.s"
	set	times%,43
	global	cerror%
	global	times

times:
	MCOUNT
	mov.l	&times%,%d0
	trap	&0
	bcc.b	noerror
	jmp	cerror%
noerror:
	rts
