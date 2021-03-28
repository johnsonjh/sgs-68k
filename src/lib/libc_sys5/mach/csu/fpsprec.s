#ident	"@(#)fpsprec.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)fpsprec.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
# 
# Set FPU rounding precision to single precision
#


# The MC6888x MODE CONTROL BYTE is the low-order byte of the FPCR.
# We read the register into a 680xx register where we can fiddle
# with the bits that control rounding precision without losing
# the values of the other bits.

	global  _fpsprec

	text

## Single: set precision bits to 01
_fpsprec:

	fmov.l	%control,%d0
	and.l	&0xff3f,%d0
	or.l	&0x0040,%d0
	fmov.l	%d0,%control
	rts
