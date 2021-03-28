#ident	"@(#)strcpy.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)strcpy.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	MOTOROLA:strcpy.s	2.2	
#
#	M68000 String(3C) Routine
#
#	(C) Copyright 1983 by Motorola Inc.
#
#	Written by: Steve Sheahan
#
# strcpy - copies string s2 to s1, stopping after the null character has 
#	   been copied
	file	"strcpy.s"
#
# Input:	pointer to strings s1 and s2
#
# Output:	pointer to s1
#
# Registers:	%a0 - points at s1
#		%a1 - points at s2

					# char *
					# strcpy(s1, s2)
					# register char *s1, *s2;
					# {
	text
	global	strcpy
strcpy:
	MCOUNT
	mov.l	4(%sp),%a0		# addr(s1)
	mov.l	8(%sp),%a1		# addr(s2)
	link	%fp,&0
					# while(*s1++ = *s2++)
L%1:
	mov.b	(%a1)+,(%a0)+
	bne.b	L%1
					# return(s1);
	unlk	%fp
	mov.l	4(%sp),%a0
	mov.l	%a0,%d0
	rts
