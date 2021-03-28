#ident	"@(#)_tan881.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)_tan881.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#		Copyright (c) 1985 AT&T		#
#		All Rights Reserved		#
#	/OLD MOT:_tan881.s	6.1	
	



	file	"_tan881.s"

# unsigned int
# _tan881(x,y)
# double x;
# double *y;
# {
	set	F%1,-4
_m4_ifdef_(`__STRICT_STDC__',
	global	_tan881
_tan881:,
	global	tan881
tan881:)
	link.l	%fp,&F%1
# unsigned register int exc;		/* imagine exc as being %d0 */
# exc = ftan(x,&y);
_m4_ifdef_(`EMU',
`       mov.d (8,%fp),-(%sp)
	jsr ftand
	add.w   &8,%sp',
`       ftan.d (8,%fp),%fp0')
	fmovm.l	%status,%d0		# contains any exception or
					# condition code info
	fmov.d	%fp0,([16.w,%fp])
	fmovm.l	%status,%d1		# may contain overflow info
	or.l	%d1,%d0
# return (exc);				/* it is already in %d0 */
# }
L%12:
	unlk	%fp
	rts
