	#ident	"@(#)nexect.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
		ident	"@(#)nexect.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	file	"nexect.s"
#	MOT:nexect.a68	1.3

# C library -- exect

# 	trap vectors
	_m4_include_(SYSTRAP_H)
	
# exect(file, argv, env);

# where argv is a vector argv[0] ... argv[x], 0
# last vector element must be 0

# The same as execve except that it sets the TBIT causing
# a trace trap on the first instruction of the executed process,
# to give a chance to set breakpoints.

	global	exect
	global	cerror%

exect:
	MCOUNT
	mov.l	&execve%,%d0
	trap	&15
	jmp 	cerror%		#note ALWAYS goto error routine
