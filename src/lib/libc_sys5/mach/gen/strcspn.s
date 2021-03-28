#ident	"@(#)strcspn.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)strcspn.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	MOTOROLA:strcspn.s	2.2	
#
#	M68000 String(3C) Routine
#
#	(C) Copyright 1983 by Motorola Inc.
#
#	Written by: Steve Sheahan
#
# strcspn - returns the length of the initial segment of "string" which
#	    consists entirely of characters NOT from string "charset"
	file	"strcspn.s"
#
# Input:	string -  in question
#		charset - string containing forbidden characters
#
# Output:	length of the initial string segment which consists 
#		entirely of characters not from charset
#
# Registers:	%a0 - string in question (q)
#		%a1 - charset (p)
#		%d0 - scratch register AND output value

					# int
					# strcspn(string, charset)
					# char	*string;
					# register char	*charset;
					# {
	text
	global	strcspn
strcspn:
	MCOUNT
	mov.l	4(%sp),%a0		# addr(string)
	link	%fp,&0
					# for(q=string; *q != '\0'; ++q) {
L%1:
	tst.b	(%a0)
	beq.b	L%5
					# for(p=charset; *p != '\0' && *p != *q; ++p);
	mov.l	12(%sp),%a1		# addr(charset) modified (+4) for link
L%2:
	tst.b	(%a1)			# *p != '\0'
	beq.b	L%4
	mov.b	(%a1)+,%d0
	cmp.b	%d0,(%a0)		# *p++ != *q
	beq.b	L%5
	bra.b	L%2
					# }
L%4:
	add.l	&1,%a0			# ++q
	bra.b	L%1
L%5:
					# return(q-string);
	unlk	%fp
	mov.l	%a0,%d0
	sub.l	4(%sp),%d0
	rts
