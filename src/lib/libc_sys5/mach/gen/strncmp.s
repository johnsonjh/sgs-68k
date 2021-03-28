#ident	"@(#)strncmp.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)strncmp.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	MOTOROLA:strncmp.s	2.2	
#
#	M68000 String(3C) Routine
#
#	(C) Copyright 1983 by Motorola Inc.
#
#	Written by: Steve Sheahan
#
# strncmp - Compares at most n bytes of strings s1 and s2 and
#	    returns: >0 if s1>s2 
#		     0  if s1=s2
#		     <0 if s1< s2
	file	"strncmp.s"
#
# Input:	s1, s2 - strings to compare
#		n - maximum number of characters to compare
#
# Output:	described above
#
# Registers:	%a0 - points into s1
#		%a1 - points into s2
#		%d0 - scratch register AND return value
#		%d1 - scratch register
#
					# int
					# strncmp(s1, s2, n)
					# register char *s1, *s2;
					# register n;
					# {
	text
	global	strncmp
strncmp:
	MCOUNT
	mov.l	4(%sp),%a0		# addr(s1)
	mov.l	8(%sp),%a1		# addr(s2)
	link	%fp,&0
					# if(s1 == s2)
	cmp.l	%a0,%a1
	beq.b	L%4
					# while(--n > -1 && *s1++ == *s2++)
	mov.l	16(%sp),%d0		# get n  modified (+4) for link
	ble.b	L%4
	sub.l	&1,%d0			# enter at top of loop for dbne
	bra.b	L%1
L%0:
	swap	%d0			# lower half = 2**16-1 = 0xffff
L%1:
	mov.b	(%a0)+,%d1
	beq.b	L%2a			# if *s1 == '\0'
	cmp.b	%d1,(%a1)+
L%2:
	dbne	%d0,L%1
	bne.b	L%3
	swap	%d0
	dbra	%d0,L%0
	bra.b	L%4
L%2a:	
	add.l	&1,%a1			# resync s2 for following subtract
L%3:
					# return((*--s1 - *--s2));
	clr.l	%d0
	mov.b	-(%a0),%d0
	clr.l	%d1
	mov.b	-(%a1),%d1
	sub.l	%d1,%d0
	bra.b	L%5
L%4:
	clr.l	%d0			# return(0)
L%5:
	unlk	%fp
	rts
