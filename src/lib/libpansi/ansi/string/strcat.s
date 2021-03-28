#ident	"@(#)strcat.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)strcat.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	MOTOROLA:strcat.s	2.2	
#
#	M68000 String(3C) Routine
#
#	(C) Copyright 1983 by Motorola Inc.
#
#	Written by: Steve Sheahan
#
# strcat - appends a copy of string s2 onto s1
	file	"strcat.s"
#
# Input:	the address of strings s1 and s2
#
# Output:	the address of s1
#
# Registers:	%a0 points to string s1
#		%a1 points to string s2

				# char *
				# strcat(s1, s2)
				# register char *s1, *s2;
	text
	global	strcat
strcat:
	MCOUNT
	mov.l	4(%sp),%a0	# addr(s1)
	mov.l	8(%sp),%a1	# addr(s2)
	link	%fp,&0
				# while(*s1++); - find the end of s1
L%1:
	tst.b	(%a0)+
	bne.b	L%1
				# --s1; - point to end of s1
	sub.l	&1,%a0
				# while(*s1++ = *s2++) - append s2 onto s1
L%2:
	mov.b	(%a1)+,(%a0)+
	bne.b	L%2
				# return(s1);
	unlk	%fp
	mov.l	4(%sp),%a0
	mov.l	%a0,%d0
	rts
