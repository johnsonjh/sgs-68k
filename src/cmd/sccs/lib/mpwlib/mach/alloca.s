#	START NEW ARIX SCCS HEADER
#
#	@(#) alloca.s: version 25.1 created on 12/2/91 at 17:12:02
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	ident	"@(#)alloca.s	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
#
#	END NEW ARIX SCCS HEADER
#
	ident	"@(#)alloca.s	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
	text
# like alloc, but automatic
# automatic free in return
global alloca
alloca:
	mov.l	(%sp)+,%a1	# save return
	mov.l	(%sp),%d1	# this many bytes
	add.l	&3,%d1
	and.l	&0xfffffffc,%d1 # round up the count to long words
	sub.l	%d1,%sp
	tst.w	-136(%sp)	# probe
	mov.l	%sp,%d0		# return pointer to allocated space
	sub.l	&4,%sp		# take care of stack adjust after return
	jmp	(%a1)		# funny return
