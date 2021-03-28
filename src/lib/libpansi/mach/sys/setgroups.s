	#ident	"@(#)setgroups.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
		ident	"@(#)setgroups.s	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	file	"setgroups.s"

# C library -- setgroups

# 	trap vectors
	_m4_include_(SYSTRAP_H)
	
	global	setgroups
	global	getgroups

#/*setgroups (size,grouplist)
# * size - the number of valid elements in the grouplist array
# * grouplist - the address of the array which contains all of the group IDs 
# * of the groups to which the user belongs
# *
# * This function will store the supplementary groups IDs from the given list
# * into the U area of this user process
#*/

setgroups:
#	MCOUNT
	mov.l	&setgroups%,%d0
	trap	&0
	bcc.b 	noerror_sg%
	jmp	cerror%

noerror_sg%:
	mov.l   &0,%d0
	rts



#/* getgroups (gidsetsize,grouplist)
# * gidsetsize - the number of elements in the supplied array
# * grouplist  - an array allocated for the storage of the supplementary
# * group IDs
# *
# * This function will return up to gidsetsize group IDs, or the size con-
# * figured in the kernel, whichever is less, in the supplied array
# * "grouplist"
# */
getgroups:
#	MCOUNT

	mov.l	&getgroups%,%d0
	trap	&0
	bcc.b 	noerror_gs%
	jmp	cerror%

noerror_gs%:
	rts

