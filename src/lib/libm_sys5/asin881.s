#ident	"@(#)asin881.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)asin881.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#		Copyright (c) 1985 AT&T		#
#		All Rights Reserved		#
#	/OLD MOT:asin881.s	6.1	
	

	file	"asin881.s"
# unsigned int
# asin881(x,y)
# double x;
# double *y;
# {
	set	F%1,-4
	global	asin881
asin881:
	link.l	%fp,&F%1
# unsigned register int exc;		/* imagine exc as being %d0 */
# exc = fasin(x,&y);
_m4_ifdef_(`EMU',
`	mov.d (8,%fp),-(%sp)
        jsr fasind
        add.w   &8,%sp' ,
`	fasin.d (8,%fp),%fp0')	
	fmovm.l	%status,%d0		# contains any exception or
					# condition code info
	fmov.d	%fp0,([16.w,%fp])
# return (exc);				/* it is already in %d0 */
# }
	unlk	%fp
	rts


# unsigned int
# acos881(x,y)
# double x;
# double *y;
# {
	set	F%2,-4
	global	acos881
acos881:
	link.l	%fp,&F%2
# unsigned register int exc;		/* imagine exc as being %d0 */
# exc = facos(x,&y);
_m4_ifdef_(`EMU',
`       mov.d (8,%fp),-(%sp)
        jsr facosd
        add.w   &8,%sp',
`	facos.d (8,%fp),%fp0')
	fmovm.l	%status,%d0		# contains any exception or
					# condition code info
	fmov.d	%fp0,([16.w,%fp])
# return (exc);				/* it is already in %d0 */
# }
	unlk	%fp
	rts
