#	START NEW ARIX SCCS HEADER
#
#	@(#) fakcu.s: version 25.1 created on 12/2/91 at 19:52:48
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	ident	"@(#)fakcu.s	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
#
#	END NEW ARIX SCCS HEADER
#
	ident	"@(#)fakcu.s	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
#/*LINTLIBRARY*/
#/*
# *       This is a dummy _cleanup routine to place at the end
# *       of the C library in case no other definition is found.
# *       If the standard I/O routines are used, they supply a
# *       real "_cleanup"  routine in file flsbuf.c
# */
#void
#_cleanup()
#{
#}


	file	"fakcu.s"

	global	_cleanup
_cleanup:
	MCOUNT
	rts


