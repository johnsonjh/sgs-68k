#	START NEW ARIX SCCS HEADER
#
#	@(#) makefile: version 25.1 created on 12/2/91 at 19:21:36
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)makefile	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.
#	ATT: #ident	"curses:screen/makefile	1.31.1.45"

#ident	"@(#)lib/libcurses:makefile	25.1"

#
# cursor package maker
# SYSV makefile for UNIX System V
#

# Sources
include $(IROOT)/macdefs

#To include SIGTSTP from <sys/signal.h> for initscr.c and tstp.c
LOCAL_CFLAGS=-DSIGTSTP=24

SLIB=	libcurses.a
MYBIN=	$(ROOT)/usr/bin
USRLIB = $(ROOT)/usr/lib
CTAGS=	ctags
BUILT=	term.h keycaps curses.h keyname.c termcap.c tiget.c tnames.c tinames.c tifnames.c tifget.c
LINT=	lint
LPASS1=	/usr/lib/lint1
TMP=	./tmp
DESTDIR = $(ROOT)
RM=	rm
LN=	ln
CPPDEFS= -I.
DCPPDEFS= -DDEBUG $(CPPDEFS)
DFLAGS=	-g $(DCPPDEFS)
TFLAGS=	-O $(DCPPDEFS)
SUBOBJS=
LIBS= 
SHELL=	/bin/sh
.SUFFIXES: .p .d .t

HEADERS=curses.h unctrl.h curses_inc.h curshdr.h term.h uparm.h

#
#	tic source and header files
#
TICHDRS= compiler.h object.h
TICSRC= tic_main.c tic_hash.c tic_captab.c tic_scan.c tic_error.c \
	tic_parse.c tic_read.c tnames.c tinames.c tifnames.c
TICOBJS= tic_main.o tic_hash.o tic_captab.o tic_scan.o tic_error.o \
	tic_parse.o tic_read.o tnames.o tinames.o tifnames.o

CHDRS= print.h otermcap.h

BSRC= print.c iexpand.c
BOBJ= print.o iexpand.o

CSRC= captoinfo.c otermcap.c $(BSRC)
COBJ= captoinfo.o otermcap.o $(BOBJ)

ISRC= infocmp.c cexpand.c infotocap.c $(BSRC)
IOBJ= infocmp.o cexpand.o infotocap.o $(BOBJ)

tools: $(SLIB) tic tput captoinfo infocmp show dumpti chgtinfo

CFILES1=_addch.c _addchnstr.c _addchstr.c _addnstr.c _addstr.c \
	_attroff.c _attron.c _attrset.c _beep.c _bkgd.c \
	_bkgdset.c _border.c _box.c _clear.c _clrtobot.c \
	_clrtoeol.c _crmode.c d_curterm.c _delch.c _deleteln.c \
	_echo.c _echochar.c _erase.c _fixterm.c _flash.c \
	gbdlins.c gbdwin.c _getch.c _getstr.c \
	_halfdelay.c _hline.c _inch.c _inchnstr.c _inchstr.c \
	_innstr.c _insch.c _insdelln.c _insertln.c _insnstr.c \
	_insstr.c _instr.c _intrflush.c _meta.c _move.c \
	_mvaddch.c Maddchnstr.c Maddchstr.c _mvaddnstr.c \
	_mvaddstr.c _mvdelch.c _mvgetch.c _mvgetstr.c _mvhline.c \
	_mvinch.c Minchnstr.c _mvinchstr.c _mvinnstr.c \
	_mvinsch.c _mvinsnstr.c _mvinsstr.c _mvinstr.c _mvvline.c \
	_mvwaddch.c Mwaddchnst.c Mwaddchstr.c Mwaddnstr.c \
	_mvwaddstr.c _mvwdelch.c _mvwgetch.c _mvwgetstr.c \
	_mvwhline.c _mvwinch.c Mwinchnst.c Mwinchstr.c \
	_mvwinnstr.c _mvwinsch.c Mwinsnstr.c _mvwinsstr.c \
	_mvwinstr.c _mvwvline.c _newterm.c _nl.c _nocrmode.c \
	_noecho.c _nonl.c _noqiflush.c _overlay.c _overwrite.c \
	_qiflush.c _refresh.c _resetterm.c _saveterm.c \
	_scr_init.c s_restore.c _scr_set.c _scrl.c _scroll.c \
	s_curterm.c _set_term.c _setscrreg.c _slk_init.c \
	_standend.c _standout.c _subpad.c _timeout.c _touchline.c \
	_unctrl.c _vline.c _waddchstr.c _waddstr.c _wclear.c \
	_wdeleteln.c _werase.c _winsertln.c _winsstr.c \
	_wstandend.c _wstandout.c __sscans.c _sprintw.c \
	V2.makenew.c V3.box.c V3.initscr.c V3.m_addch.c m_addstr.c \
	V3.m_clear.c V3.m_erase.c m_initscr.c V3.m_move.c m_newterm.c \
	m_refresh.c V3.newterm.c pchchar.c upd_old_y.c \
	V3.vidattr.c V3.vidputs.c V3.waddch.c wtroff.c V3.wattron.c \
	wtrset.c wchchar.c V3.winsch.c baudrate.c cbreak.c \
	chkinput.c clearok.c copywin.c curs_set.c \
	curserr.c curses.c def_prog.c delay.c delay_out.c \
	delkey.c delkeymap.c delscreen.c delterm.c delwin.c \
	derwin.c doupdate.c draino.c dupwin.c endwin.c \
	erasechar.c flushinp.c getattrs.c getbegyx.c getbkgd.c \
	getmaxyx.c getparyx.c getsyx.c gettmode.c getwin.c \
	getyx.c has_ic.c has_il.c idcok.c idlok.c \
	immedok.c init_acs.c init_costs.c initscr.c is_linetou.c \
	is_wintou.c keyname.c keypad.c killchar.c leaveok.c can_change.c \
	sc_restore.c

CFILES2=longname.c makenew.c memSset.c meta.c mvcur.c \
	mvderwin.c mvprintw.c mvscanw.c mvwin.c mvwprintw.c \
	mvwscanw.c napms.c newkey.c newpad.c newscreen.c \
	newwin.c nocbreak.c nodelay.c noraw.c \
	notimeout.c outch.c overlap.c pechochar.c pnoutref.c \
	prefresh.c printw.c putp.c putwin.c quick_echo.c \
	raw.c redrawwin.c reset_sh.c resetty.c restart.c \
	ring.c ripoffline.c savetty.c scanw.c scr_all.c \
	scr_dump.c s_ll_dump.c scr_reset.c scrollok.c scurscreen.c \
	setcurterm.c setecho.c setkeymap.c setnonl.c setqiflush.c \
 	setsyx.c setterm.c setupterm.c slk_atroff.c slk_atron.c \
 	slk_atrset.c slk_clear.c slk_label.c \
	s_noutref.c s_refresh.c s_restore.c slk_set.c slk_start.c \
	slk_touch.c subwin.c syncok.c tcsearch.c termattrs.c \
	termcap.c termerr.c termname.c tgetch.c tgoto.c \
	tifget.c tifnames.c tiget.c tinames.c tinputfd.c \
	tnames.c touchwin.c tparm.c tputs.c trace.c \
	tstp.c ttimeout.c typeahead.c unctrl.c ungetch.c \
	untouchwin.c vidputs.c vidupdate.c vsscanf.c vwprintw.c \
	vwscanw.c waddch.c waddchnstr.c waddnstr.c wattroff.c \
	wattron.c wattrset.c wbkgd.c wbkgdset.c wborder.c \
	wclrtobot.c wclrtoeol.c wdelch.c wechochar.c wgetch.c \
	wgetstr.c whline.c winch.c winchnstr.c winchstr.c \
	winnstr.c winsch.c winsdelln.c winsnstr.c winstr.c \
	wmove.c wnoutref.c wprintw.c wredrawln.c wrefresh.c \
	wscanw.c wscrl.c wsetscrreg.c wsyncdown.c wsyncup.c \
 	wtimeout.c wtouchln.c wvline.c \
 	color_cont.c init_color.c init_pair.c has_colors.c \
 	pair_cont.c start_col.c 

all: date $(SLIB) llib-lcurses.l tools
 
date:
	@echo Started: `date`

# Regular, optimized objects
OBJS1=	$(CFILES1:.c=.o)
OBJS2=	$(CFILES2:.c=.o)
$(OBJS1): ${HEADERS}
$(OBJS2): ${HEADERS}

# Profiled, optimized objects
#POBJS=	$(CFILES:.c=.p)
#$(POBJS): ${HEADERS}

# Debugging objects
#DOBJS=	$(CFILES:.c=.d)
#$(DOBJS): ${HEADERS}

# Tracing objects
#TOBJS=	$(CFILES:.c=.t)
#$(TOBJS): ${HEADERS}

.c.o:
	${CC} ${CPPDEFS} ${CFLAGS} -c $*.c

.c.p:
	-mv $*.o $*.O
	${CC} ${CFLAGS} -p -c $*.c
	mv $*.o $*.p
	-mv $*.O $*.o
.c.d:
	-mv $*.o $*.O
	${CC} ${DFLAGS} -c $*.c
	mv $*.o $*.d
	-mv $*.O $*.o
.c.t:
	-mv $*.o $*.O
	${CC} ${TFLAGS} -c $*.c
	mv $*.o $*.t
	-mv $*.O $*.o

obj1:	$(OBJS1)

obj2:	$(OBJS2)

$(SLIB): obj1 obj2
	@echo
	@echo Building libcurses archive
	@echo
	${AR} rv $(SLIB) $(OBJS1)
	${AR} rv $(SLIB) $(OBJS2)
	@echo
	@echo libcurses has been archived
	@echo

p:	p$(SLIB)
p$(SLIB): ${POBJS}
	rm -f x.c
	${AR} rv p$(SLIB) $?

d:	d$(SLIB)
d$(SLIB): ${HEADERS} ${DOBJS}
	rm -f x.c
	${AR} lrv d$(SLIB) $?

t:	t$(SLIB)
t$(SLIB): ${OBJS}
	rm -f x.c
	${AR} rv t$(SLIB) $?

setupterm.o: term.h 
	${CC} ${CPPDEFS} ${CFLAGS} -DNOBLIT -c $*.c

term.h tnames.c tinames.c tifnames.c: caps maketerm.ed
	@if [ ! -d "$(TMP)" ] ;\
	then	echo "\t$(TMP) not found.";\
		echo "\n\tCreating $(TMP)\n";\
		mkdir $(TMP); \
	fi;
	@echo
	@echo Building term.h
	@echo
	ed - < maketerm.ed
	$(CH) chmod 0664 term.h tnames.c tinames.c tifnames.c
	@echo
	@echo "term.h has been built"
	@echo

keycaps: caps
	@echo
	@echo Building keycaps
	@echo
	rm -rf $(TMP)/keycaps keycaps
	grep "KEY_" caps > keycaps
	@echo 
	@echo "keycaps has been built"
	@echo

curses.h: keycaps curses.ed
	@if [ ! -d "$(TMP)" ] ;\
	then	echo "\t$(TMP) not found.";\
		echo "\n\tCreating $(TMP)\n";\
		mkdir $(TMP); \
	fi;
	@echo 
	@echo Building ./curses.h
	@echo 
	rm -f curses.h
	ed - < curses.ed
	$(CH) chmod 0664 curses.h
	rm -rf $(TMP)/keycaps
	@echo 
	@echo "curses.h has been built"
	@echo 

keyname.c: keycaps keyname.sh
	@echo
	@echo Building keyname.c
	@echo
	rm -f keyname.c
	sh keyname.sh
	$(CH) chmod 0664 keyname.c
	@echo
	@echo "keyname.c has been built"
	@echo

setkeymap.c: keycaps
	@if tail -1 keycaps | grep 'KEY_F(63)' > /dev/null;then :; \
		else echo new keys!! must change setkeymap.c; exit 1;fi

termcap.c:	termcap.ed caps
	@if [ ! -d "$(TMP)" ] ;\
	then	echo "\t$(TMP) not found.";\
		echo "\n\tCreating $(TMP)\n";\
		mkdir $(TMP); \
	fi;
	@echo 
	@echo Building termcap.c
	@echo
	rm -f termcap.c
	ed - < termcap.ed
	rm -f bool num str
	$(CH) chmod 0664 termcap.c
	@echo 
	@echo "termcap.c has been built"
	@echo

tifget.c:	tifget.ed caps
	@if [ ! -d "$(TMP)" ] ;\
	then	echo "\t$(TMP) not found.";\
		echo "\n\tCreating $(TMP)\n";\
		mkdir $(TMP); \
	fi;
	@echo
	@echo Building tifget.c
	@echo
	ed - < tifget.ed
	$(CH) chmod 0664 tifget.c
	@echo
	@echo "tifget.c has been built"
	@echo

tiget.c:	tiget.ed caps
	@if [ ! -d "$(TMP)" ] ;\
	then	echo "\t$(TMP) not found.";\
		echo "\n\tCreating $(TMP)\n";\
		mkdir $(TMP); \
	fi;
	@echo
	@echo Building tiget.c
	@echo
	ed - < tiget.ed
	$(CH) chmod 0664 tiget.c
	@echo
	@echo "tiget.c has been built"
	@echo

ticclob:
	rm -rf ${TICOBJS} tic

tic_captab.o:	tic_captab.c compiler.h
tic_error.o:	tic_error.c compiler.h
tic_hash.o:	tic_hash.c compiler.h
tic_main.o:	tic_main.c compiler.h curses.h
tic_parse.o:	tic_parse.c compiler.h object.h
tic_read.o:	tic_read.c object.h
tic_scan.o:	tic_scan.c compiler.h

new:	clean tags $(SLIB)

allnew:	clean tags $(SLIB) d$(SLIB) p$(SLIB) t$(SLIB)

allfour: $(SLIB) d$(SLIB) p$(SLIB) t$(SLIB)

clean:
	@echo
	@echo "Removing all .o's"
	@echo
	@rm -f $(OBJS1) $(SLIB)
	@rm -f $(OBJS2)
	@echo
	@echo "Removing all .d's"
	@echo
	@rm -f ${DOBJS} d$(SLIB)
	@echo
	@echo "Removing all .p's"
	@echo
	@rm -f ${POBJS} p$(SLIB)
	@echo
	@echo "Removing all .t's"
	@echo
	rm -rf *.o trace a.out
	rm -rf $(BUILT)

clobber: clean
	@rm -rf ${TOBJS} t$(SLIB) $(SLIB) ?$(SLIB) $(TMP)
	rm -rf captoinfo chgtinfo dumpti infocmp show llib-lcurses.l tic tput 

rmhdrs: 
	@echo
	@echo The build of libcurses generates programs and header files.
	@echo We remove any old versions of these files before we begin.
	@echo
	rm -f $(BUILT)

install:	all
	# install the library
		$(INS) $(SLIB) $(USRLIB)/$(SLIB)
		$(AR) ts $(USRLIB)/$(SLIB)
		chmod 644 $(USRLIB)/$(SLIB)
		chgrp bin $(USRLIB)/$(SLIB)
		chown bin $(USRLIB)/$(SLIB)
	# install the lint library files
		$(INS) llib-lcurses $(USRLIB)/llib-lcurses
		$(INS) llib-lcurses.l $(USRLIB)/llib-lcurses.l
		chmod 444 $(USRLIB)/llib-lcurses $(USRLIB)/llib-lcurses.l
		chgrp bin $(USRLIB)/llib-lcurses $(USRLIB)/llib-lcurses.l
		chown bin $(USRLIB)/llib-lcurses $(USRLIB)/llib-lcurses.l
	# install the pertinent includes if $(ROOT)/usr/$(SGS)include exists
		if [ -d $(ROOT)/usr/$(SGS)include ] ;\
		then \
		$(INS) curses.h $(ROOT)/usr/$(SGS)include/curses.h ;\
		$(INS) term.h $(ROOT)/usr/$(SGS)include/term.h ;\
		$(INS) unctrl.h $(ROOT)/usr/$(SGS)include/unctrl.h ;\
		cd $(ROOT)/usr/$(SGS)include ;\
		chmod 644 curses.h term.h unctrl.h ;\
		chgrp bin curses.h term.h unctrl.h ;\
		chown bin curses.h term.h unctrl.h ;\
		fi
	# link libcurses to the old names
		rm -f $(USRLIB)/libtermcap.a $(USRLIB)/libtermlib.a
		ln $(USRLIB)/libcurses.a $(USRLIB)/libtermcap.a
		ln $(USRLIB)/libcurses.a $(USRLIB)/libtermlib.a
	# install the tools
		$(INS) tic $(MYBIN)/tic
		$(INS) captoinfo $(MYBIN)/captoinfo 
		$(INS) infocmp $(MYBIN)/infocmp
		$(INS) tput $(MYBIN)/tput
	#	$(INS) dumpti $(MYBIN)/dumpti
		$(INS) show $(MYBIN)/show
		chmod 555 $(MYBIN)/tic $(MYBIN)/captoinfo 
		chmod 555 $(MYBIN)/infocmp $(MYBIN)/tput
	#	chmod 555 $(MYBIN)/dumpti 
		chmod 555 $(MYBIN)/show 
		chgrp bin $(MYBIN)/tic $(MYBIN)/captoinfo 
		chgrp bin $(MYBIN)/infocmp $(MYBIN)/tput
	#	chgrp bin $(MYBIN)/dumpti 
		chgrp bin $(MYBIN)/show
		chown bin $(MYBIN)/tic $(MYBIN)/captoinfo 
		chown bin $(MYBIN)/infocmp $(MYBIN)/tput
	#	chown bin $(MYBIN)/dumpti 
		chown bin $(MYBIN)/show
	# install the old termcap
		$(INS) ../termcap $(ROOT)/etc/termcap
		chmod 555 $(ROOT)/etc/termcap
		chgrp bin $(ROOT)/etc/termcap
		chown bin $(ROOT)/etc/termcap


SOURCES=${CTAGS} ${CFILES} $(HEADERS)

ctags:	
	ctags	$(SOURCES)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
tags:
	${CTAGS} ${CFILES} $(HEADERS)

lint:
	-ln llib-lcurses llib.c
	@echo ${LINT} ${CPPDEFS} > lint.out
	-${LINT} -hxb ${CPPDEFS} ${CFILES} llib.c >> lint.out
	@echo ${LINT} ${DCPPDEFS} >> lint.out
	-${LINT} -hxb ${DCPPDEFS} ${CFILES} llib.c >> lint.out
	rm -f llib.c

lpr:
	-pr `ls ${HEADERS} | sort` | lpr
	-pr `ls ${CFILES} | sort` | lpr
	-lpq

TPFILES= makefile makefile.bsd makefile.sysv \
	${HEADERS} ${CFILES} \
	${TICHDRS} ${TICSRC} \
	${CHDRS} ${BSRC} ${CSRC} ${ISRC} \
	curses.ed \
	termcap.ed tiget.ed tifget.ed \
	tags llib-lcurses TODO caps getopt.c \
	keyname.sh otic.c \
	show.c tterminfo.c tput.c

tp:
	tp crm0 `ls ${TPFILES} | uniq`

tar:
	tar crvf curses.tar `ls ${TPFILES} | uniq`

cpio:
	ls ${TPFILES} | uniq | cpio -o > curses.cpio

cpio-c:
	ls ${TPFILES} | uniq | cpio -oc > curses.cpio

cpiotape:
	ls ${TPFILES} | uniq | cpio -oBc > /dev/rmt8

llib-lcurses.l: llib-lcurses
	$(CC) -E -C -I. -SYSV -I$(INCRT) -Dlint llib-lcurses | ${LPASS1} -vx -H$(TMP)/hlint > llib-lcurses.l
	rm -f $(TMP)/hlint

cktmp:
	@echo 
	@echo libcurses.a is built using $(TMP) for work space.
	@echo Verifying the existence of $(TMP).
	@echo
	@if [ -d "$(TMP)" ] ;\
	then \
		echo "\t$(TMP) exists";\
		echo;\
	else  \
		echo "\t$(TMP) not found.";\
		echo "\n\tCreating $(TMP)\n";\
		mkdir $(TMP); \
	fi;

dumpti: setupterm.c curses.h term.h tnames.o tinames.o tifnames.o termerr.o
	@echo
	@echo Building dumpti
	@echo
	ln setupterm.c dumpti.c
	$(CC) $(CPPDEFS) $(CFLAGS) $(LDFLAGS) -DDUMPTI -DNOBLIT -s dumpti.c tnames.o tinames.o tifnames.o termerr.o -o dumpti ; rm -rf dumpti.c
	@echo
	@echo dumpti has been built
	@echo

captoinfo: $(COBJ) $(SLIB)
	$(CC) $(LDFLAGS) $(COBJ) $(SLIB) -s -o captoinfo

infocmp: $(IOBJ) $(SLIB)
	$(CC) $(LDFLAGS) $(IOBJ) $(SLIB) -s -o infocmp

captoinfo.o:	captoinfo.c curses.h otermcap.h print.h
otermcap.o:	otermcap.c otermcap.h
print.o:	print.c curses.h print.h
iexpand.o:	iexpand.c print.h
cexpand.o:	cexpand.c print.h
infotocap.o:	infotocap.c curses.h
infocmp.o:	infocmp.c curses.h term.h print.h

localtic: ${TICOBJS}
	@echo
	@echo Building the tic compiler
	@echo
	${CC} -s -I$(LOCALINCS) -o tic ${TICOBJS}
	@echo
	@echo the tic compiler has been built
	@echo

tic: ${TICOBJS}
	@echo
	@echo Building the tic compiler
	@echo
	${CC} -s $(LDFLAGS) -o tic ${TICOBJS}
	@echo
	@echo the tic compiler has been built
	@echo

otic: otic.o tnames.o tinames.o tifnames.o unctrl.o
	${CC} -s $(LDFLAGS) -o otic otic.o tnames.o tinames.o tifnames.o unctrl.o

linttic: ${TICSRC}
	${LINT} ${CPPDEFS} ${TICSRC}

lintinfocmp: ${ISRC}
	${LINT} ${CPPDEFS} ${ISRC}

lintcaptoinfo: ${CSRC}
	${LINT} ${CPPDEFS} ${CSRC}

show:	$(SLIB) show.o
	${CC} $(CPPDEFS) ${LDFLAGS} ${CFLAGS} -o show show.o $(SLIB)

pshow:	p$(SLIB) show.p
	${CC} $(CPPDEFS) ${LDFLAGS} -pg ${CFLAGS} -o pshow show.p p$(SLIB)

dshow:	d$(SLIB) show.d
	${CC} $(CPPDEFS) ${LDFLAGS} ${DFLAGS} -o dshow show.d d$(SLIB)

tshow:	t$(SLIB) show.t
	${CC} $(CPPDEFS) ${LDFLAGS} ${TFLAGS} -o tshow show.t t$(SLIB)

tput:	$(SLIB) tput.o tnames.o tinames.o tifnames.o
	${CC} $(CPPDEFS) ${LDFLAGS} ${CFLAGS} -o tput tput.o $(SLIB)

tterminfo:	$(SLIB) tterminfo.o
	${CC} $(CPPDEFS) ${LDFLAGS} ${CFLAGS} -o tterminfo tterminfo.o $(SLIB)

chgtinfo:	curses.h chgtinfo.c
	${CC} $(CPPDEFS) ${LDFLAGS} ${CFLAGS} -o chgtinfo chgtinfo.c
