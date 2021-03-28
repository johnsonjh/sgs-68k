#ident	"@(#)setjmp.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)setjmp.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved 

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	
#	The copyright notice above does not evidence any   
#	actual or intended publication of such source code.

# C library -- setjmp, longjmp

#	longjmp(a,v)
# will	generate a "return(v)" from
# the last call to
#	setjmp(a)
# by restoring a1-a7,d2-d7 from 'a'
# and doing a return.
#

	file	"setjmp.s"
	global	_setjmp
	global	setjmp
	global	longjmp

_setjmp:
setjmp:
	MCOUNT
	mov.l	(%sp)+,%a1	# pop return pc into %a1
	mov.l	(%sp),%a0	# a
	movm.l	&0xfefc,(%a0)	# save a1-a7, d2-d7 in a
	clr.l	%d0		# return 0
	jmp	(%a1)

longjmp:
	MCOUNT
	add.l	&4,%sp		# throw away this return pc
	mov.l	(%sp)+,%a0	# a
	mov.l	(%sp),%d0	# v
	bne.b	L0
	mov.l	&1,%d0		# force v non-zero
L0:
	movm.l	(%a0),&0xfefc	# restore a1-a7, d2-d7 from a
	jmp	(%a1)		# return v (but as if from setjmp)
