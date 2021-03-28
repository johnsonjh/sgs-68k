#ident	"@(#)_etox881.s	23.4	8/27/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)_etox881.s	23.4	8/27/91 Copyright (c) 1991 by Arix Corp."
#		Copyright (c) 1985 AT&T		#
#		All Rights Reserved		#
#	/OLD MOT:_etox881.s	6.1	
	



	file	"_etox881.s"

# unsigned int
# _etox881(x,y)
# double x;
# double *y;
# {
	set	F%1,-4
_m4_ifdef_(`__STRICT_STDC__',
	global	_etox881
_etox881:,
	global	etox881
etox881:)
	link.l	%fp,&F%1
# unsigned register int exc;		/* imagine exc as being %d0 */
# exc = fetox(x,&y);
	move.d	(8,%fp),-(%sp)
	jsr fetoxd
	add.w	&8,%sp
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
