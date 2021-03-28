#ident	"@(#)_access81.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)_access81.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#		Copyright (c) 1985 AT&T		#
#		All Rights Reserved		#
#	MOT:_access81.s	6.1	

	file	"_access81.s"



#	The following routines provide access from C to the system
#	registers for the MC68881 floating point chip.
#
# long _rd881_status()
# {
	text
_m4_ifdef_(`_ANSI',
	global	_rd881_status
_rd881_status:,
	global	rd881_status
rd881_status:
)
	MCOUNT
	fmovm.l	%status,%d0
# }
	rts

#
# long _rd881_iaddr()
# {
	text
_m4_ifdef_(`_ANSI',
	global	_rd881_iaddr
_rd881_iaddr:,
	global	rd881_iaddr
rd881_iaddr:
)
	MCOUNT
	fmovm.l	%iaddr,%d0
# }
	rts

#
# long _rd881_control()
# {
	text
_m4_ifdef_(`_ANSI',
	global	_rd881_control
_rd881_control:,
	global	rd881_control
rd881_control:
)
	MCOUNT
	fmovm.l	%control,%d0
# }
	rts

#
# void _wr881_control(newvalue)
# long new_value;
# {
	text
_m4_ifdef_(`_ANSI',
	global	_wr881_control
_wr881_control:,
	global	wr881_control
wr881_control:
)
	MCOUNT
	fmovm.l	4(%sp),%control
# }
	rts

