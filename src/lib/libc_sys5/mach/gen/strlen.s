#ident	"@(#)strlen.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)strlen.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	MOTOROLA:strlen.s	2.2	
#
#	M68000 String(3C) Routine
#
#	(C) Copyright 1983 by Motorola Inc.
#
#	Written by: Steve Sheahan
#
# strlen - returns the number of characters in s, not including the
#	   terminating null character
	file	"strlen.s"
#
# Input:	s - the string in question
#
# Output:	the length of string "s" 
#
# Registers:	%a0 - points into s
#		%a1 - points at s + 1 for subtraction
#		%d0 - scratch

					# int
					# strlen(s)
					# register char *s;
					# {
	text
	global	strlen
strlen:
	MCOUNT
	mov.l	4(%sp),%a0		# addr(s)
	link	%fp,&0
	lea.l	1(%a0),%a1		# s0 = s + 1
					# while (*s++ != '\0')
L%1:
	tst.b	(%a0)+
	bne.b	L%1
					# return (s - s0);
	mov.l	%a0,%d0
	sub.l	%a1,%d0
	unlk	%fp
	rts
