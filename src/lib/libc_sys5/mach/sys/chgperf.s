#ident	"@(#)chgperf.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)chgperf.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."

	file	"chgperf.s"

	set	chgperf%,157
	set	fchgperf%,158

	global	chgperf
	global	fchgperf
	global	cerror%

chgperf:
	MCOUNT
	mov.l	&chgperf%,%d0
	trap	&0
	bcc.b 	noerror
	jmp 	cerror%
noerror:
	rts

fchgperf:
	MCOUNT
	mov.l	&fchgperf%,%d0
	trap	&0
	bcc.b 	no2error
	jmp 	cerror%
no2error:
	rts
