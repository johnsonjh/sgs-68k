#ident	"@(#)memcpy.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)memcpy.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	MOTOROLA:memcpy.s	2.1	
#
#	M68000 Memory(3C) Routine
#
#	(C) Copyright 1983 by Motorola Inc.
#
#	Written by: Steve Sheahan
#
# memcpy - copies n characters from s2 to s1, and returns s1
	file	"memcpy.s"
#
# Input:	s1 - destination
#		s2 - source
#		 n - number of bytes to copy
#
# Output:	s1
#
# Registers:	%a0 - pointer into s1
#		%a1 - pointer into s2
#		%d0 - loop counter

					# char *
					# memcpy(s1, s2, n)
					# register char *s1, *s2;
					# register int n;
					# {
	text
	global	memcpy
memcpy:
	MCOUNT
	mov.l	4(%sp),%a0		# addr(s1)
	mov.l	8(%sp),%a1		# addr(s2)
	mov.l	12(%sp),%d0		# n
	link	%fp,&0
					# while (--n >= 0)
	ble.b	L%3
	bra.b	L%2
L%0:
	swap	%d0			# lower half = 2**16 - 1 = 0xFF
L%1:
					# *s1++ = *s2++;
	mov.b	(%a1)+,(%a0)+
L%2:
	dbra	%d0,L%1
	swap	%d0
	dbra	%d0,L%0			# decrement upper half of counter
L%3:
 				# return (os1);
	unlk	%fp
	mov.l	4(%sp),%a0
	mov.l	%a0,%d0
	rts
