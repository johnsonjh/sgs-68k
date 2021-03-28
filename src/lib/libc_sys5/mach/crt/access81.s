#ident	"@(#)access81.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)access81.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#		Copyright (c) 1985 AT&T		#
#		All Rights Reserved		#
#	MOT:access81.s	6.1	

	file	"access81.s"



#	The following routines provide access from C to the system
#	registers for the MC68881 floating point chip.
#
# long rd881_status()
# {
	text
	global	rd881_status
rd881_status:
	MCOUNT
	fmovm.l	%status,%d0
# }
	rts

#
# long rd881_iaddr()
# {
	text
	global	rd881_iaddr
rd881_iaddr:
	MCOUNT
	fmovm.l	%iaddr,%d0
# }
	rts

#
# long rd881_control()
# {
	text
	global	rd881_control
rd881_control:
	MCOUNT
	fmovm.l	%control,%d0
# }
	rts

#
# void wr881_control(newvalue)
# long new_value;
# {
	text
	global	wr881_control
wr881_control:
	MCOUNT
	fmovm.l	4(%sp),%control
# }
	rts

