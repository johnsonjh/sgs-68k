#ident	"@(#)fpstart.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)fpstart.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
# 
# Hossein Raassi
# This routine is called from crt1.s just before the 
#  call to main (jsr main).
# 6-21-1989
#

	set     SIGNAL,48 
	set	SIGFPE,8	# signal number for floating point

# MC6888x Exception Enable Byte definitions

	set     fpcrOPERR,0x2000        # invalid operation
	set     fpcrOVFL,0x1000         # overflow
	set     fpcrUNFL,0x0800         # underflow
	set     fpcrDZ,0x0400           # divide by zero

	global  __fpstart       # floating-point environment initializer

	text

#  __fpstart
#
#  Called from crt1.s (or it's equivalent) for initializing
#  the float  environment. The default MC6888n (and IEEE) is no exceptions
#  enabled, round nearest and extended precision.  We change the exception
#  enables to OPERR, DIVZERO, OVERFLOW and UNDERFLOW.
#
#  Most floating-point programmers expect DIVZERO and OVERFLOW to cause
#  exceptions (OPERR is unique to IEEE) but not UNDERFLOW.  This is because
#  most systems underflow to zero which is usually desireable.
#
#  There was a nasty bug in the way that the signal system call was
#  invoked, which was causing the stack to get clobbered.  This was
#  fixed by doing a jsr to signal after puttig the arguments onto the
#  stack instead of attempting to trap directly to the kernel after
#  putting the value 48 (signal system call) into d0.  mer, 1-29-90

__fpstart:

# 	set exception mask
	fmov.l  &fpcrOPERR+fpcrOVFL+fpcrUNFL+fpcrDZ,%control 
	rts
