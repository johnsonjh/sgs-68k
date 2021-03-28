
#
#	MOT:MAKE.RES	7.1	

#ident	"@(#)f77/f77optim:MAKE.RES	25.1"

#****************************************************************************
#
# FUNCTION: MAKE.RES
#
# 	Shell script to control the creation of a resident version  of optim
# 	for the M68000, M68010, or M68020 processors. 
#
# COMMAND LINE and PARAMETERS:
#
#	MAKE.RES [<PROC>] [<macro definitions>]
#
#	PROC:			specifies the target processor
#	macro definitions:	a list, possibly NULL, of macro definitions
#	
# IMPORTED KEY WORDS:
#
#	$DEFPROC:	The default target processor 
#			(set in user's .profile file).
#
#	$DESTM68000:	These three variables specify the partition(s)
#	$DESTM68010:	on which the product will be installed depending
#	$DESTM68020:	on the specified processor. 
#			(set in user's .profile file)
#
# EXPORTED KEY WORDS:
#
#	none
#
# LOCAL KEY WORDS:
#
#	$DESTINATION:	specifies the partition(s) which the new optim will be
#			copied to
#
#	$SUFFIX:	the last two digits of the processor's name (i.e
#			00, 10, or 20).  This string is used to specify
#			the directories where the object and executable
#			files will be stored.
#
#	$ERROR:		contains the status of the make command which 
#			builds optim
#
# SHELL SCRIPTS/MAKEFILES called by this shell:
#
#	makefile:	optim's makefile
#
#	where:		returns the name of the current environment
#
#	do68:		these three routines specify the environment
#	do10:		in which the compilation should execute
#	do20:	
#
# SHELL SCRIPTS/MAKEFILES which call this shell:
#
#	mk.res		this shell controls the creation of a log file
#
# ALGORITHM:
#
#	This shell accomplishes its described task through the following
#	steps:
#
#	1. Determine the target processor.  This is accomplished either
#	   by the first positional parameter, by the user specifying it
#	   in interactive mode, or by default.
#
#	2. Check to see if the current environment is correct. If not,
#	   then change to it.
#
#	3. Delete the local copies of optim and its object files (we
#	   don't know which environment it was developed in.)
#
#	4. Get a copy of the last version of optim and its object files
#	   from the appropriate directory.
#
#	5. Make a new version of optim.
#
#	6. Install the new version on the appropriate file systems.
#
#	7. Copy the new version of optim and its object files back into
#	   the appropriate directory.
#
# ERRORS:
#
#	The keyword parameters DEFPROC, DESTM68010, DESTM68020, and DESTM68000
#	must be defined and exported in your .profile.
#
#****************************************************************************

echo "************************* optim MAKE.RES **************************"
echo "\nStarting MAKE.RES for optim. `date`\n"
echo "command line: mk.res $*\n"

#
#	Scan the first positional parameter. If it is a PROC specification
#	then remember it and shift it off.  The PROC argument determines
#	which environment this shell should execute in.
#

case $1 in
   68000 | m68000 | M68000 | 00 |\
   68010 | m68010 | M68010 | 10 |\
   68020 | m68020 | M68020 | 20) 
	PROC=$1
	shift;;
   *) 	PROC="";;
esac

#	Verify that the user specified a PROC.  If one was not specified
#	then request that the user input a legal PROC. If the job is not
#	interactive, assign the default PROC.
#
#	Once a PROC has been determined, set up the ".do" 
#	environment, and initialize SUFFIX and DESTINATION.
#
#

SUFFIX=""
while [ "$SUFFIX" = "" ] 
   do
   case $PROC in

#	M68000 processor selected: set the environment for compilation
#	and establish the destination directory and file partitions.

      68000 |m68000 | M68000 | 00)
				SUFFIX="00"
				if [ "`where`" != "m68k" ]
				    then
				    . do68
				fi
			        if [ "$DESTM68000" = "" ]
			           then
			           echo "**** ERROR **** Please define and export DESTM68000 in your .profile"
			           exit 255
			        else
			           DESTINATION="$DESTM68000"
			        fi;;

#	M68010 processor selected: set the environment for compilation
#	and establish the destination directory and file partitions.

      68010 | m68010 | M68010 | 10)
				SUFFIX="10"
				if [ "`where`" != "M68010" ]
				    then
				    . do10
				fi
			        if [ "$DESTM68010" = "" ]
			           then
			           echo "**** ERROR **** Please define and export DESTM68010 in your .profile"
			           exit 255
			        else
			           DESTINATION="$DESTM68010"
			        fi;;

#	M68020 processor selected: set the environment for compilation
#	and establish the destination directory and file partitions.

      68020 | m68020 | M68020 | 20)
				SUFFIX="20"
				if [ "`where`" != "M68020" ]
				    then
				    . do20
				fi
			        if [ "$DESTM68020" = "" ]
			           then
			           echo "**** ERROR **** Please define and export DESTM68020 in your .profile"
			           exit 255
			        else
			           DESTINATION="$DESTM68020"
			        fi;;

#	The target processor has not been specified. Depending on the
#	means in which this shell was invoked (background or interactive)
#	obtain the target processor's name.

      *) if [ -t 0 ]
	    then
	    echo "Designate the target processor: M68000, M68010, or M68020"
	    PROC="`line`"
	 else
	    case "$DEFPROC" in
	       68000 | m68000 | M68000 | 00 |\
	       68010 | m68010 | M68010 | 10 |\
	       68020 | m68020 | M68020 | 20) PROC="$DEFPROC";;
	       *)
	          echo "**** ERROR **** Please define and export DEFPROC in your .profile"
	          exit 255;;
	    esac
	 fi;;
      esac
done
echo "\nWorking in the M680$SUFFIX environment \n"

#
#	Retrieve old object and optim after cleaning up
#

make clobber
mv resident$SUFFIX/* .

#
#	Make optim 
#

echo "Make command line: make build$*\n"

make build $*

#
#	Install optim onto $DESTINATION iff make was successful
#

ERROR=$?
if [ $ERROR = 0 ]
   then
   strip optim
   for i in $DESTINATION
      do
      cvout optim $i/lib/optim
      chmod 0775 $i/lib/optim
      chgrp port $i/lib/optim
      chown port $i/lib/optim
      echo "optim installed on $i"
      done
else
   echo "make did not return successfully. return value = $ERROR"
fi
#
#	Save executable and object files
#

mv optim optab.c *.o resident$SUFFIX
chmod 660 resident$SUFFIX/*.o
echo "executable and object files saved in resident$SUFFIX"
