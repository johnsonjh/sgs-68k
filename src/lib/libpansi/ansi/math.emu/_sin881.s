#ident	"@(#)_sin881.s	23.4	8/27/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)_sin881.s	23.4	8/27/91 Copyright (c) 1991 by Arix Corp."
#		Copyright (c) 1985 AT&T		#
#		All Rights Reserved		#
#	/OLD MOT:_sin881.s	6.1	
	



	file	"_sin881.s"

# unsigned int
# _sin881(x,y)
# double x;
# double *y;
# {
	set	F%1,-4
_m4_ifdef_(`__STRICT_STDC__',
	global	_sin881
_sin881:,
	global	sin881
sin881:)
	link.l	%fp,&F%1
# unsigned register int exc;		/* imagine exc as being %d0 */
# exc = fsin(x,&y);
	move.d	(8,%fp),-(%sp)
	jsr fsind
	add.w	&8,%sp
	fmovm.l	%status,%d0		# contains any exception or
					# condition code info
	fmov.d	%fp0,([16.w,%fp])
# return (exc);				/* it is already in %d0 */
# }
	unlk	%fp
	rts


# unsigned int
# _cos881(x,y)
# double x;
# double *y;
# {
	set	F%2,-4
_m4_ifdef_(`__STRICT_STDC__',
	global	_cos881
_cos881:,
	global	cos881
cos881:)
	link.l	%fp,&F%2
# unsigned register int exc;		/* imagine exc as being %d0 */
# exc = fcos(x,&y);
	move.d	(8,%fp),-(%sp)
	jsr fcosd
	add.w	&8,%sp
	fmovm.l	%status,%d0		# contains any exception or
					# condition code info
	fmov.d	%fp0,([16.w,%fp])
# return (exc);				/* it is already in %d0 */
# }
	unlk	%fp
	rts
