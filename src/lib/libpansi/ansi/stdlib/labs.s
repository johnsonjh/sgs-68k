#ident	"@(#)labs.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)labs.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	OLD MOT:labs.s	1.0 
#	/* Assembler program to implement the following C program */
#	long int labs( long int arg )
#	{
#		return((arg < 0)? -arg: arg);
#	}



	file	"labs.s"
	global	labs
labs:
	MCOUNT
	mov.l	4(%sp),%d0
	bge.b	labsl
	neg.l	%d0
labsl:
	rts
