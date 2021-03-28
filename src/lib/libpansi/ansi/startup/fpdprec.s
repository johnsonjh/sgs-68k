#ident	"@(#)fpdprec.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)fpdprec.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
# 
# Set FPU rounding precision to double precision
#


# The MC6888x MODE CONTROL BYTE is the low-order byte of the FPCR.
# We read the register into a 680xx register where we can fiddle
# with the bits that control rounding precision without losing
# the values of the other bits.

	global  _fpdprec

	text

## Double: set precision bits to 10
_fpdprec:

	fmov.l	%control,%d0
	and.l	&0xff3f,%d0
	or.l	&0x0080,%d0
	fmov.l	%d0,%control
	rts
