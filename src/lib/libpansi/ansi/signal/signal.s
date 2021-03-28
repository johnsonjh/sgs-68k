	#ident	"@(#)signal.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
		ident	"@(#)signal.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	#ident	"@(#)signal.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	file	"signal.s"
#   C-library signal() routine recoded for IEEE floating-point
# REMEMBER: SIGFPE is special in this environment!
#
#   In SVR3, five new system calls are added to this file. They are
#   sigset(), sighold(), sigrelse(), sigignore() and sigpause().	


# 	trap vectors
	_m4_include_(SYSTRAP_H)

# **********
# Global function names
# **********
	global  signal
_m4_ifdef_(`_ANSI',
	,
	global	sigset
	global	sighold
	global	sigignore
	global	sigrelse
	global	sigpause
	global	sigaction
	global	sigemptyset
	global	siginitset
	global	sigfillset
	global	sigaddset
	global	sigdelset
	global	sigismember
	global	sigpending
	global	sigprocmask
	global	sigsuspend)

	global	cerror%
_m4_ifdef_(`SHLIB',
`	global	_libc_d%vect
',
_m4_ifdef_(`_ANSI',
`'
,
`	global	d%vect')
)

	set	NSIG,31
	set	EINVAL,22
	set	SIGILL,4
	set	SIGTRAP,5
	set	SIGPWR,19
	set	SIG_DFL,0	# "default" signal
	set	SIG_IGN,1	# "ignore" signal
	set	SIG_HOLD,2	# "hold" signal

	# The following 5 definitions are also defined in signal.h
	set	SIGDEFER,0x100	  # "defer" signal after caught one
	set	SIGDEFER_BIT,0x8  # "defer" signal after caught one
	set	SIGHOLD,0x200	  # "hold" signal
	set	SIGHOLD_BIT,0x9	  # "hold" signal
	set	SIGRELSE,0x400	  # "release" signal
	set	SIGRELSE_BIT,0xa  # "release" signal
	set	SIGIGNORE,0x800	  # "ignore" signal
	set	SIGIGNORE_BIT,0xb # "ignore" signal
	set	SIGPAUSE,0x1000	  # "release" held signal and pause
	set	SIGPAUSE_BIT,0xc  # "release" held signal and pause

	set	REGSAVE,4*2	# 4 bytes times the number of regs to save
	set	SIG_SETMASK,0x0004
	set	ALLSET,0x0fffffff  #all of the signals that we support



#
# ******************
# Definition of sighold()
# ******************
#
# sighold(n)		/* hold signal n */
#
sighold:
	MCOUNT

	# is the signal number valid?  (0 < n < NSIG)
	mov.l	(4.w,%sp),%d0
	ble.w	ninvalid%
	mov.l	&NSIG,%d1
	cmp.l	%d1,%d0			# did i get the right order?
	ble.w	ninvalid%

#	or.l	&SIGHOLD,%d0		# turn on SIGHOLD bit
	bset	&SIGHOLD_BIT,%d0	# turn on SIGHOLD bit
	mov.l	%d0,(4.w,%sp)
	bra.b 	nost%

#
# ******************
# Definition of sigrelse()
# ******************
#
# sigrelse(n)		/* release signal n if there is one held */
#
sigrelse:
	MCOUNT

	# is the signal number valid?  (0 < n < NSIG)
	mov.l	(4.w,%sp),%d0
	ble.b	ninvalid%
	mov.l	&NSIG,%d1
	cmp.l	%d1,%d0			# did i get the right order?
	ble.b	ninvalid%

#	or.l	&SIGRELSE,%d0		# turn on SIGRELSE bit
	bset	&SIGRELSE_BIT,%d0	# turn on SIGRELSE bit
	mov.l	%d0,(4.w,%sp)
	bra.b 	nost%

#
# ******************
# Definition of sigignore()
# ******************
#
# sigignore(n)		/* ignore signal n */
#
sigignore:
	MCOUNT

	# is the signal number valid?  (0 < n < NSIG)
	mov.l	(4.w,%sp),%d0
	ble.b	ninvalid%
	mov.l	&NSIG,%d1
	cmp.l	%d1,%d0			# did i get the right order?
	ble.b	ninvalid%

#	or.l	&SIGIGNORE,%d0		# turn on SIGIGNORE bit
	bset	&SIGIGNORE_BIT,%d0	# turn on SIGIGNORE bit
	mov.l	%d0,(4.w,%sp)
	bra.b 	nost%

#
# ******************
# Definition of sigpause()
# ******************
#
# sigpause(n)		/* hold signal n */
#
sigpause:
	MCOUNT

	# is the signal number valid?  (0 < n < NSIG)
	mov.l	(4.w,%sp),%d0
	ble.b	ninvalid%
	mov.l	&NSIG,%d1
	cmp.l	%d1,%d0			# did i get the right order?
	ble.b	ninvalid%

#	or.l	&SIGPAUSE,%d0		# turn on SIGPAUSE bit
	bset	&SIGPAUSE_BIT,%d0	# turn on SIGPAUSE bit
	mov.l	%d0,(4.w,%sp)
#	bra.b 	nost%

nost%:
	mov.l	&sig%,%d0
	trap	&0
	bcc.b 	noerror
	jmp 	cerror%
noerror:
	rts

ninvalid%:
	mov.l	&EINVAL,%d0
	jmp 	cerror%

#
# ************
# Definition of sigset()
# ************
#
# sigset(n, SIG_DFL); 	/* default action on signal n */
# sigset( n, SIG_IGN);	/* ignore signal n		*/
# sigset( n, handler);	/* call user handler on signal	*/

#
sigset:
	MCOUNT
	#SAVEREGS HERE
	mov.l	%d2,-(%sp)
	mov.l	%a2,-(%sp)
	mov.l	&1,%d2		# signify that this is sigset, not signal
	bra.b	sig1%		# go to signal routine

#
# **********
# Definition of signal()
# **********
#
# signal( n, SIG_DFL);	/* default action on signal n	*/
# signal( n, SIG_IGN);	/* ignore signal n		*/
# signal( n, handler);	/* call user handler on signal	*/

signal:
	MCOUNT
	#SAVEREGS HERE
	mov.l	%d2,-(%sp)
	mov.l	%a2,-(%sp)
	mov.l	&0,%d2		# signify that this is signal, not sigset

sig1%:
	# is the signal number valid?  (0 < n < NSIG)
	mov.l	(REGSAVE+8.w,%sp),%a0	# get the new user label
	mov.l	(REGSAVE+4.w,%sp),%d0	# get the signal number

	ble.w	invalid%
	mov.l	&NSIG,%d1
	cmp.l	%d1,%d0			# did i get the right order?
	ble.w	invalid%


	mov.l	%d0,signalno
	mov.l	%d0,%d1			# convert signal number into offset
	asl.l	&2,%d1			# into a table of 4 byte vectors
	mov.l	%d1,%a1
_m4_ifdef_(`SHLIB',
`	mov.l	(_libc_d%vect,%a1),%a2	# save the old entry on stack
',
`	mov.l	(d%vect,%a1),%a2	# save the old entry on stack
')
	bra.b	sig2%

_m4_ifdef_(`SHLIB',
`	mov.l	%a0,(_libc_d%vect,%a1)	# just put user label in d%vect table
',
`	mov.l	%a0,(d%vect,%a1)	# just put user label in d%vect table
')
	mov.l	%a2,%d0			# and return the last user label
	mov.l	%d0,%a0

	#RESTORE REGS HERE
	mov.l	(%sp)+,%a2
	mov.l	(%sp)+,%d2
	rts

sig2%:
	# if the signal action is changing from anything to DFL, IGN or HOLD
	# must call the OS before updating d%vect.
	mov.l	&SIG_HOLD,%d1		
	cmp.l	%a0,%d1		# why is this wrong
				# A> if we change dvect first, then make the os
				# call, a signal may get sent to the process
				# whose user sig_func address will be DFL, IGN
				# or HOLD.  mcm			
	ble.b	sig3%			

	# update d%vect[%a1] before issuing the system call
_m4_ifdef_(`SHLIB',
`	mov.l	%a0,(_libc_d%vect,%a1)	
',
`	mov.l	%a0,(d%vect,%a1)	
')

	# and replace user entry by signal's own t%vect[n]
	# hanna, what is this table, and TSIZE may need previous definition???
# A> see the 5.3 signal.s for the documentation of dvect and tvect.  the
# comments seem to have been stripped out of this source.  mcm
	mov.l	%d0,%d1			# compute index into t%vect table
	muls.l	&TSIZE,%d1
#	mov.l	(t%vect,%d1),(REGSAVE+8.w,%sp) 
	add.l	&t%vect,%d1
	mov.l	%d1,(REGSAVE+8.w,%sp) 

sig3%:
	clr.b	(setflg,%d0)		# setflg == 1 for sigset
					# setflg == 0 for signal
	tst.b	%d2			# if signal, don't turn on the 
					# SIGDEFER flag
	beq.b	ost%

	mov.b	&1,(setflg,%d0)		# turn on setflg to remember 
					# that common code was entered
					# from sigset instead of signal
#	or.l	&SIGDEFER,%d0		# turn on SIGDEFER bit for sigset
	bset	&SIGDEFER_BIT,%d0	# turn on SIGDEFER bit for sigset
	mov.l	%d0,(REGSAVE+4.w,%sp)
ost%:
	# RESTORE REGS HERE
#	mov.l	+(%sp),%a2
#	mov.l	+(%sp),%d2
						#copy sig arguments
	mov.l	(REGSAVE+8.w,%sp),-(%sp)	# the address arg
	mov.l	(REGSAVE+8.w,%sp),-(%sp)	# the signum arg
	mov.l	(REGSAVE+8.w,%sp),-(%sp)	# the return address
	mov.l	&sig%,%d0
	trap	&0
	add.w	&12,%sp				# pop the args
	bcc.b 	noerror1%
	jmp 	cerror0%			# if kernel returned error

noerror1%:
	# the kernel will return SIG_IGN, SIG_DFL, SIG_HOLD or the old handler.
	# This may be different from the value (argument we saved in a2)
	cmp.l	%d0,&SIG_HOLD
	ble.b	around%


	# kernel returned the old handler (t%vect[n], not d.vect[n])
	# so replace with the old value of d.vect[n] off the stack
	mov.l	%a2,%d0
	
around%:
	# if we did not update d.vect[] above & 8(%sp)(addr arg) is DFL or IGN,
	# then we need to update d.vect[] here.
	mov.l	&SIG_IGN,%d1		
	cmp.l	%d1,(REGSAVE+8.w,%sp)
	blt.b	around2%
_m4_ifdef_(`SHLIB',
`	mov.l	(REGSAVE+8.w,%sp),(_libc_d%vect,%a1) #update d.vect entry
',
`	mov.l	(REGSAVE+8.w,%sp),(d%vect,%a1) #update d.vect entry
')

around2%:
	mov.l	%d0,%a0		# & copy pointer return to %a0
	#RESTORE REGS HERE?
	mov.l	%d0,-(%sp)	# store d0 
	mov.l	signalno,%d0
	mov.b	&0,(pos_flg,%d0)		# set this sig is set by posix
	mov.l	&0,signalno	# zero it out; no further confusion
	mov.l	(%sp)+,%d0	#restore d0, see 3 lines above
	mov.l	(%sp)+,%a2
	mov.l	(%sp)+,%d2
	rts

invalid%:
	mov.l	&EINVAL,%d0
	mov.l	%d0,%a0			# and copy for a pointer return
cerror0%:
	mov.l	(%sp)+,%a2
	mov.l	(%sp)+,%d2
	jmp 	cerror%
	

############################################################################
	set NSIG,0
_m4_ifdef_(`_ANSI',
	`'
	,
`global t%vect
global sighnd%')
t%vect:
sighnd%:
	jsr	savr%;	short NSIG;	set NSIG,NSIG+1	# entry 0, unused
					set TSIZE,~-t%vect # but to size jsr
	jsr	savr%;	short NSIG;	set NSIG,NSIG+1	#  1:	SIGHUP
	jsr	savr%;	short NSIG;	set NSIG,NSIG+1	#  2:	SIGINT
	jsr	savr%;	short NSIG;	set NSIG,NSIG+1	#  3:	SIGQUIT
	jsr	savr%;	short NSIG;	set NSIG,NSIG+1	#  4:	SIGILL
	jsr	savr%;	short NSIG;	set NSIG,NSIG+1	#  5:	SIGTRAP
	jsr	savr%;	short NSIG;	set NSIG,NSIG+1	#  6:	SIGABRT
	jsr	savr%;	short NSIG;	set NSIG,NSIG+1	#  7:	SIGEMT
	jsr	savr%;	short NSIG;	set NSIG,NSIG+1	#  8:	SIGFPE <- blecch
	jsr	savr%;	short NSIG;	set NSIG,NSIG+1	#  9:	SIGKILL <unused>
	jsr	savr%;	short NSIG;	set NSIG,NSIG+1	# 10:	SIGBUS
	jsr	savr%;	short NSIG;	set NSIG,NSIG+1	# 11:	SIGSEGV
	jsr	savr%;	short NSIG;	set NSIG,NSIG+1	# 12:	SIGSYS
	jsr	savr%;	short NSIG;	set NSIG,NSIG+1	# 13:	SIGPIPE
	jsr	savr%;	short NSIG;	set NSIG,NSIG+1	# 14:	SIGALRM
	jsr	savr%;	short NSIG;	set NSIG,NSIG+1	# 15:	SIGTERM
	jsr	savr%;	short NSIG;	set NSIG,NSIG+1	# 16:	SIGUSR1
	jsr	savr%;	short NSIG;	set NSIG,NSIG+1	# 17:	SIGUSR2
	jsr	savr%;	short NSIG;	set NSIG,NSIG+1	# 18:	SIGSIGCLD
	jsr	savr%;	short NSIG;	set NSIG,NSIG+1	# 19:	SIGPWR
	jsr	savr%;	short NSIG;	set NSIG,NSIG+1	# 20:	SIGWIND
	jsr	savr%;	short NSIG;	set NSIG,NSIG+1	# 21:	SIGPHONE
	jsr	savr%;	short NSIG;	set NSIG,NSIG+1	# 22:	SIGSEL
	jsr	savr%;	short NSIG;	set NSIG,NSIG+1	# 23:	SIGSTOP
	jsr	savr%;	short NSIG;	set NSIG,NSIG+1	# 24:	SIGCONT
	jsr	savr%;	short NSIG;	set NSIG,NSIG+1	# 25:	SIGTSTP
	jsr	savr%;	short NSIG;	set NSIG,NSIG+1	# 26:	SIGTTIN
	jsr	savr%;	short NSIG;	set NSIG,NSIG+1	# 27:	SIGTTOU
	jsr	savr%;	short NSIG;	set NSIG,NSIG+1	# 28:	SIGTVTARLM
	jsr	savr%;	short NSIG;	set NSIG,NSIG+1	# 29:	SIGPROF
	jsr	savr%;	short NSIG;	set NSIG,NSIG+1	# 30:	SIGSAK
	jsr	savr%;	short NSIG;	set NSIG,NSIG+1	# 31:	DUMMY: NSIG ?

# define the parallel set of user vectors
# NOTE: the contents of d.vect[i] will not correspond to the OS's idea:
#	1) for SIGKILL, where any attempt to catch, or ignore causes
#	   the kernel to return EINVAL to the signal() call.
#	2) if signal() is used and a signal has been caught,
#	   changing the handler to SIG_DFL.
#	3) if sigset() is used and a signal has been caught,
#	   holding subsequent signals of the same type.

	data	#define the parallel set of user vectors
	align	4

_m4_ifdef_(`SHLIB',
`',
`d%vect:	space	NSIG * 4	# and initialize to SIG_DFL
')
setflg:	space	NSIG 			# sigset flag vector - one per signal
pos_flg: space	NSIG			# array to indicate signal is set by
					# posix sigaction.

signalno: space	4
old_hdlr: space	4
	text

#
# **********
# Definition of signal entry point from the kernel
# **********
#
# SAVR: common entry point for all but SIGFPE
# The stack upon entry to savr is:
#
#	%sp ->	|		|
#		+---------------+
#		| t%vect ret.	| <- address of t%vect[n+1]
#		+===============+
#		| returnmask 	| : pushed only in case of posix.
#		+---------------+
#		| trapped PSW	| : GATE
#		+---------------+
#		| trapped PC	|
#		+===============+
#		| user stack	|
#
# Before doing anything we must save r0 - r8, ap, and fp,
# check the floating-point hardware status, and save fp hardware state.
# If the user handler is in assembly, it may not do a "SAVE" on entry.
# We then use the t%vect return address to calculate our signal number,
# call the user handler, restore the registers, and RETG to the user.
# this is known as sigrte% in the old signal routines

savr%:
	movm.l	&0xfffe,-(%sp)		# save all regs
#	fmovm	&0xff,-(%sp)		# SAVE FLOATING POINT REGS HERE
	mov.l	(60,%sp),%a0		# pc aft jsr savr pts to short NSIG
					# offset will be different if floats
	mov.w	(%a0),%d0		# get signal number
	ext.l	%d0			# make it long
	tst.b	(pos_flg,%d0)		# check if sig. was set by sigaction
	bne	pos_savr%
	mov.l	%d0,-(%sp)		# it will be the arg to user routine

	mov.l	%d0,%d1			# convert signal number into offset
	asl.l	&2,%d1			# into a table of 4 byte vectors
_m4_ifdef_(`SHLIB',
`	mov.l	(_libc_d%vect,%d1),%a0	# save the old entry on stack
',
`	mov.l	(d%vect,%d1),%a0	# save the old entry on stack
')


	# if the signal is SIGILL, SIGTRAP, SIGPWR, don't reset it
	cmp.l	%d0,&SIGILL
	beq.b	cont%
	cmp.l	%d0,&SIGTRAP
	beq.b	cont%
	cmp.l	%d0,&SIGPWR
	beq.b	cont%
	tst.b	(setflg,%d0)		# if from setsig * kelly
	bne.b	cont%			# then don't reset addr * kelly
_m4_ifdef_(`SHLIB',
`	mov.l	&0,(_libc_d%vect,%d1)		# otherwise reset to SIG_DFL
',
`	mov.l	&0,(d%vect,%d1)		# otherwise reset to SIG_DFL
')

cont%:
	jsr	(%a0)
	add.l	&4,%sp			# pop signum arg

	mov.l	60(%sp),%a1		# pc aft jsr savr pts to short NSIG
					# offset will be different if floats
	mov.w	(%a1),%d1		# get signal number
	tst.b	(setflg,%d1)		# check to see whether it is 
					# sigset or signal
					# if sigset, release held signal
	beq.b	norelse%

	mov.l	%d1,-(%sp)		# could replace with explicit 
	jsr	sigrelse		# sigrelse code?
	add.l	&4,%sp

norelse%:
#	fmovm	(%sp)+,&0xff		# RESTORE FLOATING POINT REGS HERE
	movm.l	(%sp)+,&0x7fff		# restore all regs
	add.l	&4,%sp			# pop return address so return is as if
					# from handler above
	mov.w	(%sp)+,%cc		# restor status register saved by kernel
	rts

					# POSIX way of handling signals...
pos_savr%:
	mov.l	%d0,-(%sp)		# it will be the arg to user routine

	mov.l	%d0,%d1			# convert signal number into offset
	asl.l	&2,%d1			# into a table of 4 byte vectors
_m4_ifdef_(`SHLIB',
`	mov.l	(_libc_d%vect,%d1),%a0	# get the actual user's 
						# signal-hdlr 
',
`	mov.l	(d%vect,%d1),%a0	# get the actual user's signal-hdlr
)

	jsr	(%a0)
	add.l	&4,%sp			# pop signum arg

#	Build the stack to call sigprocmask.
#	Now the stack looks like
#               sp-> |            |
#		     +------------+
#		     | returnmask |
#		     +------------+
#		     |status regs |
#		     +------------+
#		     | user's pc  |
#
#
#	Build the stack as follows:
#               sp-> |            |
#		     +------------+
#		     |SIG_SETMASK |	
#		     +------------+
#		     |addr. of    |
#		     |return mask |
#		     +------------+
#		     |0(zero)     |   we don't want the old mask back.
#		     +------------+
#		     |returnmask  |   previous stage...
#
	mov.l	&0,-(%sp)		# ptr to get the old mask
	mov.l	%sp,%d1                 # addr. of return mask(set)
	add.l	&68,%d1			# add 64 for the addr of returnmask
	mov.l	%d1,-(%sp)
	mov.l   &SIG_SETMASK,-(%sp)     # set the mask
	jsr	sigprocmask
	add.l	&12,%sp			# 12 for sigprocmask
	movm.l	(%sp)+,&0x7fff		# restore all regs
	add.l	&8,%sp			# 4 for addr & 4 for returnmask.
	mov.w	(%sp)+,%cc		# restor status register saved by kernel
	rts

####
#
# Sigaction system call entry point.
#
#
sigaction:
	MCOUNT
	mov.l	%d2,-(%sp)
	mov.l	%a2,-(%sp)

	# is the signal number valid?  (0 < n < NSIG)
	mov.l	(REGSAVE+4.w,%sp),%d0		# get the signal number
	ble.w	invalid%
	mov.l	&NSIG,%d1
	cmp.l	%d1,%d0				# did i get the right order?
	ble.w	invalid%
	mov.l	%d0,signalno
	mov.l	%d0,%d1				# convert signo. into offset
	asl.l	&2,%d1				# into a table of 4 byte vectors
	mov.l	%d1,%a1

						# save the old-hdlr;
						# will be safe...
_m4_ifdef_(`SHLIB',
`	mov.l	(_libc_d%vect,%a1),%a2
',
`	mov.l	(d%vect,%a1),%a2
')

	mov.l	(REGSAVE+8.w,%sp),%a0		# get the pointer for new 
						# sigaction structure
	tst.l	%a0
	ble.w	nullptr%
	mov.l	(%a0),%a0		        # get the new handler from
						# sigaction structure
_m4_ifdef_(`SHLIB',
`	mov.l	%a0,(_libc_d%vect,%a1)      # put the user hdlr in table
',
`	mov.l	%a0,(d%vect,%a1)	
')

	mov.l	%a0,old_hdlr

						# if it is SIG_DFL or SIG_IGN
						# then don't send addr of savr.
	mov.l	&SIG_IGN,%d1
	cmp.l	%a0,%d1	
	ble.b	callsys%			

	mov.l	%d0,%d1				# compute index into 
						# t%vect table
	muls.l	&TSIZE,%d1
	add.l	&t%vect,%d1
	mov.l	(REGSAVE+8.w,%sp),%a0
	mov.l	%d1,(%a0) 
	jmp	callsys%

nullptr%:
	mov.l	(REGSAVE+12.w,%sp),%a0		# get the pointer for old 
						# sigaction structure
	tst.l	%a0
	ble.w	outnull%

callsys%:
	mov.l	(REGSAVE+12.w,%sp),-(%sp)	# get the oldact number
	mov.l	(REGSAVE+12.w,%sp),-(%sp)	# get the newact number
	mov.l	(REGSAVE+12.w,%sp),-(%sp)	# get the signal number
	mov.l	(REGSAVE+12.w,%sp),-(%sp)	# get the return pc....
	mov.l	&sigaction%,%d0

	trap	&0

	bcc.b 	noerror9%
	add.l	&16,%sp
	mov.l	(REGSAVE+8.w,%sp),%a0		# update the newact with given
	tst.l	%a0				# hdlr addr in old_hdlr
	beq.w	errskip%
	mov.l	old_hdlr,(%a0)
_m4_ifdef_(`SHLIB',
`	mov.l	%a2,(_libc_d%vect,%a1)      	# put the old_hdlr back
',
`	mov.l	%a2,(d%vect,%a1)	
')
errskip%:
	mov.l 	&0,signalno
	mov.l	&0,old_hdlr
	jmp	cerror0%

noerror9%:
	mov.l	signalno,%d0
	mov.b	&1,(pos_flg,%d0)		# set this sig is set by posix
	mov.l	&0,signalno
	add.l	&16,%sp				# pop the args
	mov.l	(REGSAVE+8.w,%sp),%a0		# update the newact with given
	tst.l	%a0				# hdlr addr in old_hdlr
	beq.w	skip%
	mov.l	old_hdlr,(%a0)
	mov.l	&0,old_hdlr
skip%:
	mov.l	(REGSAVE+12.w,%sp),%a0		# update the oact with old
						# sighandler address.
	tst.l	%a0				# if oact ptr is null...
	ble.w	outnull%
	mov.l	(%a0),%d0
	mov.l	&SIG_IGN,%d1
	cmp.l	%d0,%d1	
	ble.b	outnull%			
	mov.l	%a2,(%a0)			# old hdlr is safe in a2.
outnull%:
	clr.l	%d0				# no error..congratulations...
	mov.l	(%sp)+,%a2
	mov.l	(%sp)+,%d2
	rts

# siginitset(n)
# sigemptyset(n)		/* initialize signal set 'n' to exclude all
#				POSIX defined signals */
#
siginitset:
sigemptyset:
	MCOUNT
	mov.l	(4.w,%sp),%a0
	mov.l	(%a0),%d0
	mov.l	&ALLSET,%d1
	not.l	%d1
	and.l	%d1,%d0
	mov.l	%d0,(%a0)
	mov.l   &0,%d0
	rts

# sigfillset(n)		/* initialize signal set 'n' to include all
#				POSIX defined signals */
#
sigfillset:
	MCOUNT
	mov.l	(4.w,%sp),%a0
	mov.l	(%a0),%d0
	or.l	&ALLSET,%d0
	mov.l	%d0,(%a0)
	mov.l   &0,%d0
	rts

# sigaddset(n,signo)		/* add the signal 'signo' to the set 'n'
#
sigaddset:
	MCOUNT
	# is the signal number valid?  (0 < n < NSIG)
	mov.l	(8.w,%sp),%d0		#get the signal number
	ble.w	ninvalid%
	mov.l	&NSIG,%d1
	cmp.l	%d1,%d0			# did i get the right order?
	ble.w	ninvalid%

	mov.l 	&1,%d1			#put a one in d1
	sub.l	&1,%d0			#adjust for signal array
	lsl.l	%d0,%d1			#shift this by the signal number
	mov.l	(4.w,%sp),%a0		#get the signal set address
	mov.l	(%a0),%d0		#load the set value in d0
	or.l	%d1,%d0			#set the requested bit
	mov.l	%d0,(%a0)		#reload the value in the given address
	mov.l	&0,%d0			#no error
	rts


# sigdelset(n,signo)		/* add the signal 'signo' to the set 'n'
#
sigdelset:
	MCOUNT
	# is the signal number valid?  (0 < n < NSIG)
	mov.l	(8.w,%sp),%d0		#get the signal number
	ble.w	ninvalid%
	mov.l	&NSIG,%d1
	cmp.l	%d1,%d0			# did i get the right order?
	ble.w	ninvalid%

	mov.l 	&1,%d1			#put a one in d1
	sub.l	&1,%d0			#adjust for signal array
	lsl.l	%d0,%d1			#shift this by the signal number
	not.l	%d1			#get the complement
	mov.l	(4.w,%sp),%a0		#get the signal set address
	mov.l	(%a0),%d0		#load the set value in d0
	and.l	%d1,%d0			#set the requested bit
	mov.l	%d0,(%a0)		#reload the value in the given address
	mov.l	&0,%d0			#no error
	rts



# sigismember(n,signo)		/* add the signal 'signo' to the set 'n'
#
sigismember:
	MCOUNT
	# is the signal number valid?  (0 < n < NSIG)
	mov.l	(8.w,%sp),%d0		#get the signal number
	ble.w	pos_ninvalid%
	mov.l	&NSIG,%d1
	cmp.l	%d1,%d0			# did i get the right order?
	ble.w	pos_ninvalid%

	mov.l	(4.w,%sp),%a0		#get the signal set address
	mov.l	(%a0),%d1		#load the set value in d0
	sub.l	&1,%d0			#adjust for signal array
	btst	%d0,%d1			#test the requested bit
	bne.b	noerror%
	mov.l	&0,%d0			#not a set member
	rts
noerror%:
	mov.l	&1,%d0			#this is a set member
	rts

pos_ninvalid%:
	mov.l	&EINVAL,errno
	mov.l	&-1,%d0
	mov.l	%d0,%a0
	rts

# sigprocmask(how,set,oldset)	/* examine and/or change the calling process'
#				signal mask 
#
sigprocmask:
	MCOUNT
	mov.l	(12.w,%sp),-(%sp)	# get the oldset number
	mov.l	(12.w,%sp),-(%sp)	# get the newset number
	mov.l	(12.w,%sp),-(%sp)	# get the how to parameter
	mov.l	(12.w,%sp),-(%sp)	# get the return pc....
	mov.l	&sigprocmask%,%d0

	trap	&0

	bcc.b 	noerror_s%
	add.l	&16,%sp
	jmp 	cerror%

noerror_s%:
	add.l	&16,%sp				# pop the args
	mov.l 	&0,%d0				# no error
	rts

# sigpending(mask)		/* stores the set of blocked signals in the
#				address pointed to by "mask"
#
sigpending:
	MCOUNT
	mov.l	(4.w,%sp),-(%sp)	# get the mask address
	mov.l	(4.w,%sp),-(%sp)	# get the return pc....
	mov.l	&sigpending%,%d0

	trap	&0

	bcc.b 	noerror_p%
	add.l	&8,%sp
	jmp 	cerror%
	add.l	&8,%sp
	rts


# sigsuspend(mask)		/* replaces the process' signal mask with the
#				contents of "mask" and then suspends the process
sigsuspend:
	MCOUNT
	mov.l	(4.w,%sp),-(%sp)	# get the mask address
	mov.l	(4.w,%sp),-(%sp)	# get the return pc....
	mov.l	&sigsuspend%,%d0

	trap	&0

	bcc.b 	noerror_p%
	add.l	&8,%sp
	jmp 	cerror%

noerror_p%:
	add.l	&8,%sp				# pop the args
	mov.l 	&0,%d0				# no error
	rts
