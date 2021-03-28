#ident	"@(#)locking.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)locking.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	locking.s	2.1 
# C library -- locking

# 	trap vectors
	_m4_include_(SYSTRAP_H)
	
	file	"locking.s"
	global	locking
	global  cerror%

locking:
	MCOUNT
	mov.l	&locking%,%d0
	trap	&0
	bcc.b 	noerror
	jmp 	cerror%
noerror:
	rts
