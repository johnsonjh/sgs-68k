#ident	"@(#)cuexit.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)cuexit.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	OLD MOT;cuexit.s	2.1 
# C library -- exit


# exit(code)
# code is return in r0 to system

	file	"cuexit.s"
	set	exit%,1
	global	exit
	global	_cleanup

exit:
	MCOUNT
	jsr	_cleanup
	mov.l	&exit%,%d0
	trap	&0
	stop	&0
