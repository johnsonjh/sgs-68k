#ident	"@(#)strncat.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)strncat.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	MOTOROLA:strncat.s	2.2	
#
#	M68000 String(3C) Routine
#
#	(C) Copyright 1983 by Motorola Inc.
#
#	Written by: Steve Sheahan
#
# strncat - appends at most n characters from string s2 onto string s1
	file	"strncat.s"
#
# Input:	s1 - string to append to
#		s2 - appending string
#		n  - maximum number of characters to copy from s2 to s1
#
# Output:	pointer to s1
#
# Registers:	%a0 - points into s1
#		%a1 - points into s2
#		%d0 - n
					# char *
					# strncat(s1, s2, n)
					# register char *s1, *s2;
					# register n;
					# {
	text
	global	strncat
strncat:
	MCOUNT
	mov.l	4(%sp),%a0		# addr(s1)
	mov.l	8(%sp),%a1		# addr(s2)
	mov.l	12(%sp),%d0		# maximum length to copy
	link	%fp,&0
	ble.b	L%5
					# while(*s1++)
L%1:					# find end of s1
	tst.b	(%a0)+
	bne.b	L%1
					# --s1;
	sub.l	&1,%a0
					# for (d = n-1; n > 0; --n)
	or.l	%d0,%d0			# set condition code != 0
	bra.b	L%4
L%2:
	swap	%d0			# lower half = 2**16-1 = 0xffff
L%3:
					# if (*s1++ = *s2++) break;
	mov.b	(%a1)+,(%a0)+
L%4:
	dbeq	%d0,L%3
	beq.b	L%5
	swap	%d0			# decrement top half
	dbra	%d0,L%2

					# *s1 = '\0';
	clr.b	(%a0)
					# return(os1);
L%5:
	unlk	%fp
	mov.l	4(%sp),%a0
	mov.l	%a0,%d0
	rts
