	#ident	"@(#)isnand.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
		ident	"@(#)isnand.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	file	"isnand.s"
#(#	)isnand.s
#  C-library isnand

	global	isnand

isnand:
	mov.l	4(%sp),%d1
	mov.l	8(%sp),%d0
	mov.l	%d2,-(%sp)
	mov.l	%d3,-(%sp)
	mov.l	%d0,%d2
	lsr.l	&20,%d0
	and.w	&0x07ff,%d0
	mov.w   &0x07ff,%d3
	cmp.w	%d0,%d3			# if ( exp != 0x7ff )
	bne	false			#	its not a NaN
	clr.l	%d3
	and.l	&0x000fffff,%d2
	cmp.l	%d2,%d3
	bne	nan
	cmp.l	%d1,%d3
	beq	false			# if ( fraction == 0 )
					#	its an infinity
nan:
	mov.l	&1,%d0
	mov.l	(%sp)+,%d3
	mov.l	(%sp)+,%d2
	rts
false:
	mov.l 	&0,%d0
	mov.l	(%sp)+,%d3
	mov.l	(%sp)+,%d2
	rts
