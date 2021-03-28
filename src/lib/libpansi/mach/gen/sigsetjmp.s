#ident	"@(#)sigsetjmp.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)sigsetjmp.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	MOTOROLA:setjmp.s	2.1 
# C library -- setjmp, longjmp

#	longjmp(a,v)
# will	generate a "return(v)" from
# the last call to
#	setjmp(a)
# by restoring a1-a7,d2-d7 from 'a'
# and doing a return.
#

	file	"sigsetjmp.s"
	global	_sigsetjmp
	global	sigsetjmp
	global	siglongjmp
	global	sigprocmask
	set	SAVEMASK,52
	set	PROCMASK,56
	set	SIG_SETMASK,4

_sigsetjmp:
sigsetjmp:
	MCOUNT
	mov.l	4(%sp),%a0	# a
	mov.l	8(%sp),%d0
	tst.l	%d0
	beq	nosave%
	mov.l	&1,(SAVEMASK,%a0)
	mov.l	%a0,%d0
	add.l	&PROCMASK,%d0
	mov.l   %d0,-(%sp)
	mov.l	&0,-(%sp)
	mov.l	&SIG_SETMASK,-(%sp)
	jsr	sigprocmask
	add.l	&12,%sp
	mov.l	(%sp)+,%a1	# pop return pc into %a1
	mov.l	(%sp),%a0	# a
	movm.l	&0xfefc,(%a0)	# save a1-a7, d2-d7 in a
	clr.l	%d0		# return 0
	jmp	(%a1)
nosave%:
	mov.l	(%sp)+,%a1	# pop return pc into %a1
	mov.l	(%sp),%a0	# a
	mov.l	&0,(SAVEMASK,%a0)
	movm.l	&0xfefc,(%a0)	# save a1-a7, d2-d7 in a
	clr.l	%d0		# return 0
	jmp	(%a1)
	

siglongjmp:
	MCOUNT
	add.l	&4,%sp		# throw away this return pc
	mov.l	(%sp),%a0	# a
	tst.l	(%a0,SAVEMASK)
	beq.w	no_mask_reset%
	mov.l	%a0,%d0
	add.l	&PROCMASK,%d0
	mov.l	&0,-(%sp)
	mov.l   %d0,-(%sp)
	mov.l	&SIG_SETMASK,-(%sp)
	jsr	sigprocmask
	add.l	&12,%sp
no_mask_reset%:
	mov.l	(%sp)+,%a0	# a
	mov.l	(%sp),%d0	# v
	bne.b	L0
	mov.l	&1,%d0		# force v non-zero
L0:
	movm.l	(%a0),&0xfefc	# restore a1-a7, d2-d7 from a
	jmp	(%a1)		# return v (but as if from setjmp)
