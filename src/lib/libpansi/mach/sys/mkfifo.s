#ident	"@(#)mkfifo.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)mkfifo.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	MOT:mkfifo.s	2.1 

# C library -- mkfifo

# 	trap vectors
	_m4_include_(SYSTRAP_H)
	
	file	"mkfifo.s"
	global	mkfifo

#/* mkfifo (path,mode)
# * path - points to the pathname of a file or directory
# * mode - file permission bits of the new FIFO
# *
# * This function creates a new FIFO special file named by the pathname
# * pointed to by path
#*/

mkfifo:

	mov.l	&mkfifo%,%d0

	trap	&0

	bcc.b 	noerror_m%
	jmp	cerror%

noerror_m%:
	mov.l   &0,%d0
	rts

