#ident	"@(#)crt1.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)crt1.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."


# C runtime startoff

	file	"crt1.s"
	set	exit%,1
	global	exit
	global	_start
	global	_istart
	global	mcount
	global	main
	global	_environ
#	global	setchrclass

#
#	C language startup routine

	text
_start:
	sub.l	&8,%sp
	mov.l	8(%sp),(%sp)	#  argc
	lea.l	12(%sp),%a0
	mov.l	%a0,4(%sp)	#  argv
	mov.l	%a0,%a1
L%1:
	tst.l	(%a0)+		#  null args term ?
	bne.b	L%1
	cmp.l	%a0,(%a1)	#  end of 'argv[]' and in *argv[] strings ?
	blt.b	L%2		#  skip if %a0 is less than start of *argv[]
	sub.l	&4,%a0		#  else back up one to set env = 0 (NULL)
L%2:
	mov.l	%a0,8(%sp)	#  env
	mov.l	%a0,_environ	#  indir is 0 if no env ; not 0 if env

	jsr	_istart		#  execute initialization code
#	mov.l	&0,-(%sp)	#  setchrclass: routine to initialize _ctype[]
#	jsr	setchrclass	
#	addq.w	&4,%sp
#	jsr	__fpstart
	jsr	_fp_prec	# floating point rounding precision
	jsr	main		#  no stack cleanup needed
	mov.l	%d0,(%sp)
	jsr	exit		#  no stack cleanup needed
	mov.l	&exit%,%d0
	trap	&0
mcount:				# cute, dummy for object compiled with -p
	rts			# but don't have the mcount routine
#
	data
	even

_environ:	
	space	4

	section	init,"x"

_istart:
	#	SAVE	%fp	#what is this -- should this be a link?
