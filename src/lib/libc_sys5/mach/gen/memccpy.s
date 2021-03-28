#ident	"@(#)memccpy.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)memccpy.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	MOTmemccpy.s	2.1	
#
#	M68000 Memory(3C) Routine
#
#	(C) Copyright 1983 by Motorola Inc.
#
#	Written by: Steve Sheahan
#
# memccpy - copies characters from memory area s2 into s1, stopping after
#	    the first occurrence of character c has been copied, or after
#	    n characters have been copied, whichever comes first. It 
#	    returns a pointer to the character after the copy of c in s1
#	    or a NULL pointer if c was not found in the first n characters
#	    of s2.


	file	"memccpy.s"
#
# Input:	s1 - destination
#		s2 - source
#		 c - possible terminating character
#		 n - maximum number of characters to copy
#
# Output:	pointer to the byte after 'c' or NULL if 'c' didn't exist
#
# Registers:	%a0 - pointer into s1
#		%a1 - pointer into s2
#		%d0 - c
#		%d1 - n 

					# char *
					# memccpy(s1, s2, c, n)
					# register char *s1, *s2;
					# register int c, n;
					# {
	text
	global	memccpy
memccpy:
	MCOUNT
	mov.l	4(%sp),%a0		# addr(s1)
	mov.l	8(%sp),%a1		# addr(s2)
	mov.b	15(%sp),%d0		# c
	mov.l	16(%sp),%d1		# n
	link	%fp,&0
					# while (--n >= 0) {
	ble.b	L%3
	bra.b	L%2
L%0:
	swap	%d1			# lower half = 2**16 - 1
					# if ((*s1++ = *s2++) == c)
L%1:
	mov.b	(%a1),(%a0)+
	cmp.b	%d0,(%a1)+		# compare with 'c'
L%2:
	dbeq	%d1,L%1
	beq.b	L%4			# terminate if character matched
	swap	%d1			# get upper half of counter
	dbra	%d1,L%0
					
L%3:
	sub.l	%a0,%a0			# return (0);
L%4:
	mov.l	%a0,%d0
	unlk	%fp
	rts
