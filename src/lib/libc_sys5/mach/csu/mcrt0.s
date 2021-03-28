#ident	"@(#)mcrt0.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	ident	"@(#)mcrt0.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	OLDMOTmcrt0.s	2.1 
#	3.0 SID #	1.2
# C runtime startoff including monitoring


	file	"mcrt0.s"
	set	exit%,1
#	set	cbufs,600
	set	cbufs,2000
	set	hdr,12
	set	cnt,8

	global	_start
	global	monitor
	global	sbrk
	global	main
	global	exit
	global	IEH3exit
	global	etext
	global	environ
	global	_cleanup
	global	___Argv
	data
	even
	comm	_countbase,4


	text
_start:
	sub.l	&8,%sp
	mov.l	8(%sp),(%sp)	#  argc
	lea.l	12(%sp),%a0
	mov.l	%a0,4(%sp)	#  argv
	mov.l	%a0,%a1
	mov.l	%a0,___Argv
L%1:
	tst.l	(%a0)+		#  null args term ?
	bne.b	L%1
	cmp.l	%a0,(%a1)	#  end of 'argv[]' and in *argv[] strings ?
	blt.b	L%2		#  skip if %a0 is less than start of *argv[]
	sub.l	&4,%a0		#  else back up one to set env = 0 (NULL)
L%2:
	mov.l	%a0,8(%sp)	#  env
	mov.l	%a0,environ	#  indir is 0 if no env ; not 0 if env

	mov.l	&cbufs,-(%sp)	# entrance counters (arg5 for monitor)
	mov.l	&etext,%d1
	sub.l	&L%eprol,%d1	# text size
	add.l	&1,%d1
	bclr	&0,%d1
	add.l	&hdr+cnt*cbufs,%d1	# add entrance count plus header
	asr.l	&1,%d1		# cvt byte length to `short' count
	mov.l	%d1,-(%sp)	# bufsiz (arg4 for monitor)
	add.l	%d1,%d1		# cvt `short' count back to byte count
	mov.l	%d1,-(%sp)	# for sbrk
	jsr	sbrk
	add.l	&4,%sp
	cmp.l	%a0,&-1
	beq.b	L%nospace
	mov.l	%a0,-(%sp)	# buf ptr (arg3 for monitor)
	add.l	&hdr,%a0
	mov.l	%a0,_countbase	# initialize countbase
	mov.l	&etext,-(%sp)
	mov.l	&L%eprol,-(%sp)
	jsr	monitor
	add.l	&20,%sp
	jsr	main		#  no stack cleanup needed
	mov.l	%d0,(%sp)
	bsr.b	_exit
L%3:
	mov.l	&exit%,%d0
	trap	&0
#
	data
	even
environ:	space	4
L%emsg:
	byte	'N,'o,' ,'s,'p,'a,'c,'e,' ,'f,'o,'r,' 
	byte	'm,'o,'n,'i,'t,'o,'r,' ,'b,'u,'f,'f,'e,'r,0xa,0x0
L%em1:
	text

L%nospace:
	mov.l	&L%em1-L%emsg,-(%sp)
	mov.l	&L%emsg,-(%sp)
	mov.l	&2,%d0
	mov.l	%d0,-(%sp)
	jsr	write
	bra.b	L%3

exit:
IEH3exit:
_exit:
	clr.l	-(%sp)
	jsr	monitor
	add.l	&4,%sp
	jsr	_cleanup
	bra.b	L%3
L%eprol:
