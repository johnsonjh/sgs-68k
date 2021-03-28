#ident	"@(#)memcmp.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)memcmp.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	MOTOROLA:memcmp.s	2.1	
#
#	M68000 Memory(3C) Routine
#
#	(C) Copyright 1983 by Motorola Inc.
#
#	Written by: Steve Sheahan
#
# memcmp - compares its arguments, looking at the first n characters only,
#	   and returns an integer less than, equal to, or greater than 0,
#	   according as s1 is lexicographically less than, equal to, or greater 
#	   than s2.
	file	"memcmp.s"
#
# Input:	s1 and s2 - the strings to compare
#		n - maximum number of bytes to compare
#
# Output:	0 if s1 == s2
#		<0 if s1 < s2
#		>0 if s1 > s2
					# int
					# memcmp(s1, s2, n)
					# register char *s1, *s2;
					# register int n;
					# {
	text
	global	memcmp
memcmp:
	MCOUNT
	mov.l	4(%sp),%a0		# addr(s1)
	mov.l	8(%sp),%a1		# addr(s2)
	mov.l	12(%sp),%d0		# n
	link	%fp,&0
	ble.b	L%3
					# if (s1 != s2)
	cmp.l	%a0,%a1
	beq.b	L%3
	sub.l	&1,%d0			# enter at top of loop for dbne
	bra.b	L%1
					# while (--n >= 0)
L%0:
	swap	%d0			# lower half = 2**16 - 1 = 0xFFFF
L%1:
	mov.b	(%a0)+,%d1
	cmp.b	%d1,(%a1)+
L%2:
	dbne	%d0,L%1
	bne.b	L%4			# terminate if they are not equal
	swap	%d0
	dbra	%d0,L%0			# decrement the top half of %d0
L%3:
					# return (0);
	clr.l	%d0
	bra.b	L%5
L%4:
					# return (s1 - s2)
	clr.l	%d0
	mov.b	-(%a0),%d0
	clr.l	%d1
	mov.b	-(%a1),%d1
	sub.l	%d1,%d0
L%5:
	unlk	%fp
	rts
