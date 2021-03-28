	#ident	"@(#)setpgid.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
		ident	"@(#)setpgid.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	file	"setpgid.s"




# C library -- setpgid

# 	trap vectors
	_m4_include_(SYSTRAP_H)
	
	global	setpgid
	global	security

#/* setpgid (pid,pgid)
# * pid - process id of the process group to be changed
# * pgid - the group to which the process is to be changed
# *
# * This function is used to either join an existing process group or
# * to create a new process group within the session of the calling
# * process
#*/

setpgid:

	mov.l	&setpgid%,%d0

	trap	&0

	bcc.b 	noerror_g%
	jmp	cerror%

noerror_g%:
	mov.l   &0,%d0
	rts

#/* security (pgrp)
# * pgrp - the group to which the process is to be changed
# *
# * This function sets the process group ID of the calling process to pgrp
#*/

security:

	mov.l	&security%,%d0

	trap	&0

	bcc.b 	noerror_sec%
	jmp	cerror%

noerror_sec%:
	rts
