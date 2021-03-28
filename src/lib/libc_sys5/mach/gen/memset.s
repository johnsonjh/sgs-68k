#ident	"@(#)memset.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)memset.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	MOTOROLA:memset.s	2.1	
#
#	M68000 Memory(3C) Routine
#
#	(C) Copyright 1983 by Motorola Inc.
#
#	Written by: Steve Sheahan
#
# memset - set an array of nchars starting at s1 to the character c. Return s1.
	file	"memset.s"
#
# Input:	s1 - destination
#		 c - initializing character
#		 n - number of bytes to copy
#
# Output:	s1
#
# Registers:	%a0 - pointer into s1
#		%d0 - c
#		%d1 - loop counter

					# char *
					# memset(s1, c, n)
					# register char *s1, c;
					# register int n;
					# {
	text
	global	memset
memset:
	MCOUNT
	mov.l	4(%sp),%a0		# addr(s1)
	mov.b	11(%sp),%d0		# c
	mov.l	12(%sp),%d1		# n
	link	%fp,&0
					# while (--n >= 0)
	ble.b	L%3
	bra.b	L%2
L%0:
	swap	%d1			# lower half = 2**16 - 1 = 0xFF
L%1:
					# *s1++ = *s2++;
	mov.b	%d0,(%a0)+
L%2:
	dbra	%d1,L%1
	swap	%d1
	dbra	%d1,L%0			# decrement upper half of counter
L%3:
					# return (s1);
	unlk	%fp
	mov.l	4(%sp),%a0
	mov.l	%a0,%d0
	rts
