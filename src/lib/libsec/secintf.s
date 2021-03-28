#	START NEW ARIX SCCS HEADER
#
#	@(#) secintf.s: version 25.1 created on 12/2/91 at 20:14:56
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	ident	"@(#)secintf.s	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
#
#	END NEW ARIX SCCS HEADER
#
	ident	"@(#)secintf.s	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
	ident	"@(#)libsec:secintf.s	1.0"

	set	kernmac%,143
	set	kernpriv%,142
	set	kernaud%,145
	set	kernref%,144
	global	kernpriv
	global	kernmac
	global	kernaud
	global	kernref


kernmac:
	mov.l	&kernmac%,%d0
	trap	&0
	bcc.b 	kern_ok%
	jmp	cerror%


kernpriv:
	mov.l	&kernpriv%,%d0
	trap	&0
	bcc.b 	kern_ok%
	jmp	cerror%

kernaud:
	mov.l	&kernaud%,%d0
	trap	&0
	bcc.b 	kern_ok%
	jmp	cerror%

kernref:
	mov.l	&kernref%,%d0
	trap	&0
	bcc.b 	kern_ok%
	jmp	cerror%

kern_ok%:
	rts
