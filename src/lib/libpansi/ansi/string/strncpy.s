#ident	"@(#)strncpy.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)strncpy.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	MOTOROLA:strncpy.s	2.2	
#
#	M68000 String(3C) Routine
#
#	(C) Copyright 1983 by Motorola Inc.
#
#	Written by: Steve Sheahan
#
# strncpy - Copy s2 to s1, truncating or null-padding to always copy n bytes
	file	"strncpy.s"
#
# Input:	s1 - destination string
#		s2 - source string
#		 n - number of characters to fill in s1
#
# Output:	s1 - pointer to destination string
#
# Registers:	%a0 - points into s1
#		%a1 - points into s2
#		%d0 - loop counter
#		%d1 - scratch register
					# char *
					# strncpy(s1, s2, n)
					# register char *s1, *s2;
					# register int n;
					# {
	text
	global	strncpy
strncpy:
	MCOUNT
	mov.l	4(%sp),%a0		# addr(s1)
	mov.l	8(%sp),%a1		# addr(s2)
	mov.l	12(%sp),%d0		# n
	link	%fp,&0
	ble.b	L%6
	bra.b	L%2
L%0:
	swap	%d0			# lower half = 2**16 - 1 = 0xffff
L%1:
	mov.b	(%a1)+,(%a0)+		# copy characters until n runs out
					# OR end of string copied
L%2:
	dbeq	%d0,L%1
	beq.b	L%5			# end of string copied
	swap	%d0
	dbra	%d0,L%0			# decrement upper half 
	bra.b	L%6

L%3:
	swap	%d0
L%4:
	clr.b	(%a0)+			# fill rest of string with 0
L%5:
	dbra	%d0,L%4
	swap	%d0
	dbra	%d0,L%3

L%6:
	unlk	%fp
	mov.l	4(%sp),%a0		# return s1
	mov.l	%a0,%d0
	rts
