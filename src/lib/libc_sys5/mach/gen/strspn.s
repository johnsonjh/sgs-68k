#ident	"@(#)strspn.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)strspn.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	MOTOROLA:strspn.s	2.2	
#
#	M68000 String(3C) Routine
#
#	(C) Copyright 1983 by Motorola Inc.
#
#	Written by: Steve Sheahan
#
# strspn - returns the number of characters in the maximum leading segment
#	   of string which consists solely of characters from charset
	file	"strspn.s"
#
# Input:	string  - input string in question
#		charset - character set of legal characters
#
# Output:	length of substring consisting entirely of characters from
#		charset
#
# Registers:	%a0 - string
#		%a1 - charset
#		%d0 - scratch register AND result
#
					# int
					# strspn(string, charset)
					# char	*string;
					# register char	*charset;
					# {
	text
	global	strspn
strspn:
	MCOUNT
					# register char *p, *q;
					# for(q=string; *q != '\0'; ++q) {
	mov.l	4(%sp),%a0		# addr(string)
	link	%fp,&0
L%1:
					# for(p=charset; *p != '\0' && *p != *q; ++p);
	mov.l	12(%sp),%a1		# addr(charset)	modified (+4) for link
L%2:
	tst.b	(%a1)			# *p != '\0'
	beq.b	L%4
	mov.b	(%a1)+,%d0
	cmp.b	%d0,(%a0)
	beq.b	L%3			# *p++ == *q
	bra.b	L%2
L%3:
	add.l	&1,%a0			# try next character in q
	tst.b	(%a0)
	bne.b	L%1
L%4:
					# return(q-string);
	unlk	%fp
	mov.l	%a0,%d0
	sub.l	4(%sp),%d0
	rts
