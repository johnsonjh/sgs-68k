#ident	"@(#)mcount.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)mcount.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	OLD MOT:mcount.s	2.1 
# count subroutine called during profiling



	file	"mcount.s"
	global	mcount%
	comm	_countbase,4

	text
mcount%:
	mov.l	4(%sp),%a0
	link	%fp,&0
	mov.l	(%a0),%d0
	beq.b	init
	mov.l	%d0,%a1
incr:
	add.l	&1,(%a1)
return:
	unlk	%fp
	rts
init:
	mov.l	_countbase,%d0
	beq.b	return
	mov.l	%d0,%a1
	add.l	&8,%d0
	mov.l	%d0,_countbase
	mov.l	4(%sp),(%a1)+	# modified (+4) because of link
	mov.l	%a1,(%a0)
	bra.b 	incr
