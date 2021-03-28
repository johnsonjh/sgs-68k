#ident	"@(#)shlibc.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT:#ident	"@(#)shlibc:shlibc.mk	1.13"

include	$(IROOT)/macdefs
include	$(IROOT)/sgsdefs

#
#
# makefile for shared libc
# 
#
# there will be no profiled objects with shared libraries.
#
LORDER = /bin/lorder
PCFLAGS=
PFX=$(PREFIX)
LIB=$(ROOT)/lib
SHLIB=$(ROOT)/shlib
DEFLIST=
SDEFLIST=
MKSHLIB=/mnt/shen/ckmbuild/root/bin/mkshlib
#MKSHLIB=/bin/mkshlib

# List of non-shared objects from libc-port
NONSH1=\
../port/gen/dial.o  ../port/gen/a64l.o ../port/gen/abort.o \
../port/gen/assert.o  ../port/gen/atof.o  ../port/gen/atoi.o \
../port/gen/atol.o  ../port/gen/bsearch.o  ../port/gen/calloc.o \
../port/gen/clock.o  ../port/gen/closedir.o  ../port/gen/crypt.o \
../port/gen/cftime.o  ../port/gen/ctime.o  ../port/gen/ctype.o \
../port/gen/ctype_def.o  ../port/gen/drand48.o  ../port/gen/dup2.o \
../port/gen/ecvt.o  ../port/gen/errlst.o  ../port/gen/err_def.o \
../port/gen/execvp.o ../port/gen/frexp.o \
../port/gen/ftok.o ../port/gen/ftw.o ../port/gen/gcvt.o \
../port/gen/getcwd.o  ../port/gen/getenv.o  ../port/gen/getgrent.o \
../port/gen/getgrgid.o ../port/gen/getgrnam.o ../port/gen/getlogin.o \
../port/gen/getopt.o ../port/gen/getpw.o ../port/gen/getpwent.o \
../port/gen/getpwnam.o ../port/gen/getpwuid.o ../port/gen/getut.o \
../port/gen/grent_def.o ../port/gen/hsearch.o ../port/gen/isatty.o \
../port/gen/l3.o ../port/gen/l64a.o ../port/gen/ldexp.o ../port/gen/lfind.o \
../port/gen/lsearch.o ../port/gen/malloc.o ../port/gen/mall_def.o \
../port/gen/mktemp.o ../port/gen/modf.o ../port/gen/mon.o ../port/gen/nlist.o \
../port/gen/opendir.o ../port/gen/old_tolower.o ../port/gen/old_toupper.o \
../port/gen/opt_data.o ../port/gen/perror.o ../port/gen/putenv.o \
../port/gen/putpwent.o ../port/gen/pwent_def.o ../port/gen/qsort.o \
../port/gen/rand.o ../port/gen/reall_def.o ../port/gen/readdir.o \
../port/gen/seekdir.o ../port/gen/sleep.o ../port/gen/ssignal.o \
../port/gen/strdup.o ../port/gen/strtod.o ../port/gen/strtok.o \
../port/gen/strtol.o ../port/gen/swab.o ../port/gen/tell.o \
../port/gen/telldir.o ../port/gen/tfind.o ../port/gen/time_comm.o \
../port/gen/tolower.o ../port/gen/toupper.o ../port/gen/tsearch.o \
../port/gen/ttyname.o ../port/gen/ttyslot.o ../port/print/zprnt_def.o \
../port/print/doprnt.o ../port/print/fprintf.o ../port/print/printf.o \
../port/print/sprintf.o ../port/print/vfprintf.o ../port/print/vprintf.o \
../port/print/vsprintf.o ../port/stdio/clrerr.o ../port/stdio/ctermid.o \
../port/stdio/cuserid.o ../port/stdio/data.o ../port/stdio/doscan.o \
../port/stdio/fdopen.o ../port/stdio/fgetc.o ../port/stdio/fgets.o \
../port/stdio/filbuf.o ../port/stdio/findiop.o ../port/stdio/flsbuf.o 
NONSH12=\
../port/stdio/fopen.o ../port/stdio/fputc.o ../port/stdio/fputs.o \
../port/stdio/fread.o ../port/stdio/fseek.o ../port/stdio/ftell.o \
../port/stdio/fwrite.o ../port/stdio/getchar.o ../port/stdio/getpass.o \
../port/stdio/gets.o ../port/stdio/getw.o ../port/stdio/popen.o \
../port/stdio/putchar.o ../port/stdio/puts.o ../port/stdio/putw.o \
../port/stdio/rew.o ../port/stdio/scanf.o ../port/stdio/setbuf.o \
../port/stdio/setvbuf.o ../port/stdio/stdio_def.o ../port/stdio/system.o \
../port/stdio/tempnam.o ../port/stdio/tmpfile.o ../port/stdio/tmpnam.o \
../port/stdio/ungetc.o ../port/sys/execl.o ../port/sys/execle.o \
../port/sys/execv.o ../port/sys/lockf.o ../port/sys/msgsys.o \
../port/sys/semsys.o ../port/sys/shmsys.o 


# List of non-shared objects from libc-mach
NONSH2=\
../mach/crt/zmcrt_def.o ../mach/crt/access81.o ../mach/crt/cerror.o \
../mach/crt/mcount.o ../mach/csu/crt1.o \
../mach/gen/strpbrk.o \
../mach/gen/strrchr.o ../mach/gen/strspn.o ../mach/gen/sysm68k.o \
../mach/gen/abs.o ../mach/gen/cuexit.o \
../mach/gen/gen_def.o ../mach/gen/mach_data.o ../mach/gen/memccpy.o \
../mach/gen/memchr.o ../mach/gen/memcmp.o ../mach/gen/memcpy.o \
../mach/gen/memset.o ../mach/gen/setjmp.o ../mach/gen/strcat.o \
../mach/gen/strchr.o ../mach/gen/strcmp.o ../mach/gen/strcpy.o \
../mach/gen/strcspn.o ../mach/gen/strlen.o ../mach/gen/strncat.o \
../mach/gen/strncmp.o ../mach/gen/strncpy.o ../mach/sys/getppid.o \
../mach/sys/rmdir.o ../mach/sys/zdvect_def.o ../mach/sys/getuid.o \
../mach/sys/rmount.o ../mach/sys/gtty.o ../mach/sys/rumount.o \
../mach/sys/ioctl.o ../mach/sys/sbrk.o ../mach/sys/kill.o \
../mach/sys/semsyscall.o ../mach/sys/_sysm68k.o ../mach/sys/link.o \
../mach/sys/setgid.o ../mach/sys/access.o ../mach/sys/lseek.o \
../mach/sys/setpgrp.o ../mach/sys/acct.o ../mach/sys/mkdir.o \
../mach/sys/setuid.o ../mach/sys/advfs.o ../mach/sys/mknod.o \
../mach/sys/shmsyscall.o ../mach/sys/alarm.o ../mach/sys/mount.o \
../mach/sys/stat.o ../mach/sys/chdir.o ../mach/sys/msgsyscall.o \
../mach/sys/statfs.o ../mach/sys/chmod.o ../mach/sys/netboot.o \
../mach/sys/stime.o ../mach/sys/chown.o ../mach/sys/netunboot.o \
../mach/sys/stty.o ../mach/sys/chroot.o ../mach/sys/nexect.o \
../mach/sys/sync.o ../mach/sys/close.o ../mach/sys/nice.o 

NONSH22=\
../mach/sys/syscall.o ../mach/sys/creat.o ../mach/sys/open.o \
../mach/sys/sysfs.o  ../mach/sys/dup.o ../mach/sys/pause.o \
../mach/sys/time.o ../mach/sys/execve.o ../mach/sys/pipe.o \
../mach/sys/times.o ../mach/sys/exit.o ../mach/sys/plock.o \
../mach/sys/uadmin.o ../mach/sys/fcntl.o ../mach/sys/poll.o \
../mach/sys/ulimit.o  ../mach/sys/fork.o ../mach/sys/profil.o \
../mach/sys/umask.o ../mach/sys/fstat.o ../mach/sys/ptrace.o \
../mach/sys/umount.o ../mach/sys/fstatfs.o ../mach/sys/putmsg.o \
../mach/sys/unadvfs.o ../mach/sys/getdents.o ../mach/sys/rdebug.o \
../mach/sys/uname.o ../mach/sys/getegid.o ../mach/sys/rdump.o \
../mach/sys/unlink.o ../mach/sys/geteuid.o ../mach/sys/read.o \
../mach/sys/ustat.o ../mach/sys/getgid.o ../mach/sys/rfstart.o \
../mach/sys/utime.o ../mach/sys/getmsg.o ../mach/sys/rfstop.o \
../mach/sys/wait.o ../mach/sys/getpid.o ../mach/sys/rfsys.o \
../mach/sys/write.o ../mach/sys/signal.o ../mach/fp/fpgetrnd.o \
../mach/fp/fpsetrnd.o ../mach/fp/fpgetmask.o ../mach/fp/fpsetmask.o \
../mach/fp/isnand.o ../mach/fp/fpgetsticky.o ../mach/fp/fpsetsticky.o 


# removed ../mach/sys/_sysm68k.o

all: 
	$(MAKE) -$(MAKEFLAGS)f shlibc.mk shared  PROF=@#  SDEFLIST=-DSHLIB 

shared:
	#
	# compile portable library modules
	cd port; $(MAKE) -$(MAKEFLAGS)ef port.mk
	#
	# compile machine-dependent library modules
	cd mach; $(MAKE) -$(MAKEFLAGS)ef mach.mk
	#
	# create the host and target modules
	-rm -f host target
	$(MKSHLIB) -q -s spec -t target -h host
	#
	# build the mixed archive in the temporary directory "shobject"
	-rm -rf shobject
	mkdir shobject
	cd shobject; $(MAKE) -$(MAKEFLAGS)ef ../shlibc.mk mixed
	# 
	# remove temprary directory "shobject"
	#-rm -rf shobject
mixed:
	#
	# copy to the current directory the non-shared object files
	cp $(NONSH1) .
	cp $(NONSH12) .
	cp $(NONSH2) .
	cp $(NONSH22) .
	#
	# extract from the host all the object files and
	# reorder the objects in the same way they are in libc.a
	#
	$(AR) -x ../host
	#
	# preserve libc.a object order
	mv cuexit.o             cuexit.x
	mv signal.o             signal.x
	mv kill.o               kill.x
	mv getpid.o             getpid.x
	mv cerror.o             cerror.x
	mv gen_def.o	        gen_def.x
	$(LORDER) *.o  |tsort > objlist
	mv cuexit.x	        cuexit.o
	mv signal.x 		signal.o
	mv kill.x 		kill.o
	mv getpid.x 		getpid.o
	mv cerror.x 		cerror.o
	mv gen_def.x 		gen_def.o
	$(AR) rcv ../libc_s.a  `cat objlist`  \
	signal.o kill.o getpid.o cerror.o gen_def.o zdvect_def.o cuexit.o hst* 
move:
	# First change the name of the old shared library target 
	# module  so that the target just created can be installed
	# Move the shared archive into the correct directory
	# Move the target file into the correct directory
	#
	#-mv $(SHLIB)/libc_s $(SHLIB)/libc_s-`date +%j\%H\%M`
	$(INS) libc_s.a $(LIB)/libc_s.a 
	$(AR) ts $(LIB)/libc_s.a
	$(INS) target $(SHLIB)/libc_s 
	chmod 644 $(LIB)/libc_s.a
	chmod 755 $(SHLIB)/libc_s
	chgrp bin $(LIB)/libc_s.a $(SHLIB)/libc_s
	chown bin $(LIB)/libc_s.a $(SHLIB)/libc_s


install:all move

clean:
	#
	# remove intermediate files
#	-rm -rf *.o hst* *.x objlist 
	-rm -rf *.o  *.x objlist 
	cd mach;	$(MAKE) -$(MAKEFLAGS)ef mach.mk clean
	cd port;	$(MAKE) -$(MAKEFLAGS)ef port.mk clean
	#
clobber:
	#
	# 
	#
	-rm -rf $(SHLIB)/libc_s-* 
	-rm -rf target libc_s.a
	-rm -rf *.o lib*.libc lib*.contents obj* hst* *.x
	cd mach;	$(MAKE) -$(MAKEFLAGS)ef mach.mk clobber
	cd port;	$(MAKE) -$(MAKEFLAGS)ef port.mk clobber
	# done
	#
