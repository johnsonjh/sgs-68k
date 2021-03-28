#ident	"@(#)strchr.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)strchr.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	MOTOROLA:strchr.s	2.2	
#
#	M68000 String(3C) Routine
#
#	(C) Copyright 1983 by Motorola Inc.
#
#	Written by: Steve Sheahan
#
# strchr - returns a pointer to the first occurrence of character "c"
#	   in string s1
	file	"strchr.s"
#
# Input:	s1 - string to be searched
#		c  - character being sought
#
# Output:	pointer to location of c in s1 or NULL if it is not present
#
# Registers:	%a0 - pointer to s1
#		%d0 - character c


					# char *
					# strchr(s1, c)
					# register char *s1, c;
					# {
	text
	global	strchr
strchr:
	MCOUNT
	mov.l	4(%sp),%a0		# addr(s1)
	mov.b	11(%sp),%d0		# character
	link	%fp,&0
					# do {
L%1:
					#    if(*s1 == c)
	cmp.b	%d0,(%a0)
					# 	return(s1);
	beq.b	L%2
					# } while(*s1++);
	tst.b	(%a0)+
	bne.b	L%1
					# return(0);
        sub.l	%a0,%a0
L%2:
	mov.l	%a0,%d0
	unlk	%fp
	rts
