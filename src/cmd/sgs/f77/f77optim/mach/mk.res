
#
#	MOT:mk.res	7.1	

#ident	"@(#)f77/f77optim:mk.res	25.1"

################################################################################
#
# FUNCTION:  mk.res
#
#	The sole purpose of this shell script is to invoke MAKE.RES and
#	save the messages from MAKE.RES and its children in a log file,
#	and still output these messages to the standard output.
#	
# COMMAND LINE and PARAMETERS:
#
#	see MAKE.RES for the syntax of the shell's parameters
#
# IMPORTED KEY WORDS:
#
#	see MAKE.RES
#
# EXPORTED KEY WORDS:
#
#	none
#
# SHELL SCRIPTS/MAKEFILES which call this shell
#
#	the driver shell for the compiler and its utilities
#
################################################################################

MAKE.RES $* 2>&1 | tee `date +%\h%\d%\H%\M.res`
