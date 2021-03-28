#ident	"@(#)strrchr.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)strrchr.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	MOTOROLA:strrchr.s	2.2	
#
#	M68000 String(3C) Routine
#
#	(C) Copyright 1983 by Motorola Inc.
#
#	Written by: Steve Sheahan
#
# strrchr - Returns a pointer to the LAST occurrence of character c in string s1
	file	"strrchr.s"
#
# Input:	s1 - string to search
#		 c - character being sought
#
# Output:	pointer to LAST occurence of c or NULL
#
# Registers:	%a0 - current location of c
#		%a1 - pointer into s1
#		%d0 - character c
#
					# char *
					# strrchr(s1, c)
					# register char *s1, c;
					# {
	text
	global	strrchr
strrchr:
	MCOUNT
	mov.l	4(%sp),%a1		# addr(s1)
	mov.b	11(%sp),%d0		# character c
					# register char *r;
					# r = NULL;
	link	%fp,&0
	sub.l	%a0,%a0
					# do {
L%1:
					# if(*s1 == c)
	cmp.b	%d0,(%a1)
	bne.b	L%2
					# r = s1;
	mov.l	%a1,%a0
					# } while(*s1++);
L%2:
	tst.b	(%a1)+
	bne.b	L%1
					# return(r);
	mov.l	%a0,%d0
	unlk	%fp
	rts
