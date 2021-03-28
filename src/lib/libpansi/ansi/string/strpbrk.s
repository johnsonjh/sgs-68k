#ident	"@(#)strpbrk.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)strpbrk.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	MOTOROLA:strpbrk.s	2.2	
#
#	M68000 String(3C) Routine
#
#	(C) Copyright 1983 by Motorola Inc.
#
#	Written by: Steve Sheahan
#
# strpbrk - return ptr to first occurrence of any character from `brkset'
#	    found in the character string `string'; NULL if none exists.
	file	"strpbrk.s"
#
# Input:	string - source string
#		brkset - characters sought for
#
# Output:	pointer to character found or NULL if none found
#
# Registers:	%a0 - string
#		%a1 - brkset
#		%d0 - scratch
#
					# char *
					# strpbrk(string, brkset)
					# register char *string, *brkset;
					# {
	text
	global	strpbrk
strpbrk:
	MCOUNT
	mov.l	4(%sp),%a0		# addr(string)
	link	%fp,&0
					# do {
L%1:
					# for(p=brkset; *p != '\0' 
					#		&& *p != *string; ++p);
	mov.l	12(%sp),%a1		# addr(brkset)  modified (+4) for link
L%2:
	tst.b	(%a1)
	beq.b	L%3
	mov.b	(%a1)+,%d0
	cmp.b	%d0,(%a0)
	beq.b	L%4			# character found - terminate
	bra.b	L%2
					# while(*string++);
L%3:
	add.l	&1,%a0
	tst.b	(%a0)
	bne.b	L%1
					# return((char *) 0);
	sub.l	%a0,%a0
L%4:
	mov.l	%a0,%d0
	unlk	%fp
	rts
