#ident	"@(#)strcmp.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)strcmp.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	MOTOROLA:strcmp.s	2.2	
#
#	M68000 String(3C) Routine
#
#	(C) Copyright 1983 by Motorola Inc.
#
#	Written by: Steve Sheahan
#
# strcmp - compares its argument and returns an integer less than, equal to,
#	   or greater than 0, according as s1 is lexicographically less than, 
#	   equal to, or greater than s2.

	file	"strcmp.s"
#
# Input:	strings s1 and s2
#
# Output:	returns an integer according to the following
#		if s1 > s2  then int > 0 returned
#		if s1 = s2  then 0 returned
#		if s1 < s2  then int < 0 returned
#
# Registers:	%a0 - points to s1
#		%a1 - points to s2
#		%d0 - scratch register AND holds return value
#		%d1 - scratch register

					# int
					# strcmp(s1, s2)
					# register char *s1, *s2;
					# {
	text
	global	strcmp
strcmp:
	MCOUNT
	mov.l	4(%sp),%a0		# addr(s1)
	mov.l	8(%sp),%a1		# addr(s2)
	link	%fp,&0
					# if(s1 == s2)
	cmp.l	%a0,%a1
	beq.b	L%1
					# while(*s1 == *s2++)
L%0:
	mov.b	(%a0),%d0
	cmp.b	%d0,(%a1)+
	bne.b	L%2
					#    if(*s1++ == '\0')
	tst.b	(%a0)+
	bne.b	L%0
L%1:
					# 	return(0);
	clr.l	%d0
	br.b	L%3
L%2:
					# return(*s1 - *--s2);
	clr.l	%d0
	mov.b	(%a0),%d0
	clr.l	%d1
	mov.b	-(%a1),%d1
	sub.l	%d1,%d0
L%3:
	unlk	%fp
	rts
