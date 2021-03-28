#ident	"@(#)sbrk.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)sbrk.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

	file	"sbrk.s"
#(#	)sbrk.s	1.3
#  C-library sbrk


#  oldend = sbrk(increment);

#  sbrk gets increment more core, and returns a pointer
#	to the beginning of the new core area
#
# The m4 macro calls are for shared libraries.
# If SHLIB is defined, substitute end for a pointer to end (_libc__end)
#

	set break , 17
	global	sbrk
_m4_ifdef_(`SHLIB',
`	global	_libc_end
',
`	global	end
')
	global	cerror%

	even
sbrk:
	MCOUNT
	mov.l	%a7,%a0			#copy stack ptr
_m4_ifdef_(`SHLIB',
`	mov.l	_libc_end,%d0			#addl3	nd,4(ap),-(%sp)
',
`	mov.l	nd,%d0			#addl3	nd,4(ap),-(%sp)
')
	add.l	4(%a0),%d0
	mov.l	%d0,-(%a7)
	mov.l	&1,-(%a7)			#pushl	$1
	mov.l	&break,%d0
	trap	&0
	add.l	&8,%a7			#restore stack ptr
	bcc.b 	noerr1
	jmp 	cerror%
noerr1:
_m4_ifdef_(`SHLIB',
`	mov.l	_libc_end,%d0	
',
`	mov.l    nd,%d0
')
	mov.l	%d0,%d1
	add.l	4(%a7),%d1
	bge.b	sb1			#the result <0?
	clr.l	%d1			#yes, then set it to 0
sb1:
_m4_ifdef_(`SHLIB',
`	mov.l	%d1,_libc_end		#& store the new end in nd
',
`	mov.l	%d1,nd			#& store the new end in nd
')
	mov.l	%d0,%a0			# to return a pointer for pcc2
	rts

#  brk(value)
#  as described in man2.
#  returns 0 for ok, -1 for error.

	set break , 17
	global	brk
_m4_ifdef_(`SHLIB',
`	global	_libc_end
',
`	global	end
')
	global	cerror%

brk:
	MCOUNT
	mov.l	&break,%d0
	trap	&0
	bcc.b 	noerror
	jmp	cerror%
noerror:
	mov.l	4(%a7),%d0		#movl	4(ap),nd
	bge.b	b1			#is the new end negative?
	clr.l	%d0			#yes, then set it to 0
b1:
_m4_ifdef_(`SHLIB',
`	mov.l	%d0,_libc_end		#no, then store the new end in nd
',
`	mov.l	%d0,nd			#no, then store the new end in nd
')
	clr.l	%d0			#clear any erroneous returns
	rts

_m4_ifdef_(`SHLIB',
`',
`	data
nd:
	long	end
')
