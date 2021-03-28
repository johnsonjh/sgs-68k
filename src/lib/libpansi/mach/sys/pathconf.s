#ident	"@(#)pathconf.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)pathconf.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	MOT:setpgid.s	2.1 

# C library -- pathconf

# 	trap vectors
	_m4_include_(SYSTRAP_H)
	
	file	"pathconf.s"
	global	pathconf
	global	fpathconf

#/* pathconf (path,name)
# * path - points to the pathname of a file or directory
# * name - represents the variable to be queried
# *
# * This function is used provide a method for the application to 
# * determine the current value of a configurable limit or option that
# * is associated with a file or directory
#*/

pathconf:

	mov.l	&pathconf%,%d0

	trap	&0

	bcc.b 	noerror_p%
	jmp	cerror%

noerror_p%:
	rts

#/* fpathconf (fildes,name)
# * fildes - an open file descriptor
# *
# * This function provides the same service as pathconf
#*/

fpathconf:

	mov.l	&fpathconf%,%d0

	trap	&0

	bcc.b 	noerror_fp%
	jmp	cerror%

noerror_fp%:
	rts
