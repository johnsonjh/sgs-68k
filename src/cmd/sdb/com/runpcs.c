/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) runpcs.c: version 23.2 created on 12/11/90 at 15:58:15	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)runpcs.c	23.2	12/11/90 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*		Copyright (c) 1985 AT&T		*/
/*		All Rights Reserved		*/
/*		OLD MOTrunpcs.c	6.5		*/

#ident	"@(#)sdb/com:runpcs.c	23.2"


/*
 *	UNIX debugger
 */

#include "head.h"


/* breakpoint instruction */
#define BPT	0x4e410000	/* TRAP 1 instruction */
#define MASK	0x0000FFFF      /* trap instruction must be in hiword of long */

#define exect	execve		/* exect non-standard system call */
extern int fpuinithh; 	/* hh2 */

extern MSG		NOFORK;
extern MSG		ENDPCS;
extern MSG		BADWAIT;
extern MSG		NOPCS;

/* breakpoints */
extern BKPTR		bkpthead;

extern CHAR		lastc;
extern int		errno;
extern INT		pid;
extern L_INT		loopcnt;

extern ADDR	userpc;
extern L_INT		entrypt;



getsig(sig)
{	return(sig);
}

/* runpcs - control the user process, running it in single step or
 *	continuous mode, depending on runmode, and with signal execsig.
 *
 *	It has the side effect of deleting all breakpoints (calls delbp()).
 */
runpcs(runmode,execsig)
{
	REG BKPTR	bkpt;
	int cmdval;

	/* for 'g' (goto) command, set userpc to address to which to jump */
	if (adrflg) userpc = dot;

	/* execute breakpoints (or single step) loopcnt number of times */
	while (loopcnt-- > 0) {
#if DEBUG > 1
		if(debugflag)
			fprintf(FPRT2, "runpcs(runmode=%d, execsig=%d);\n",
				runmode, execsig);
#endif
#if DEBUG
		if (debug) fprintf(FPRT2, "\tcontinue %#x %d\n",userpc,execsig);
#endif
		errno = 0;
		if (runmode == SINGLE) {
			/* hardware handles single-stepping */
			delbp();
if (debug) fprintf(FPRT2, "SINGLE STEP userpc=%#x USERPC=%#x\n",userpc,USERPC);
			ptrace(SINGLE, pid, userpc, execsig);
			bpwait(); chkerr(); execsig=0; readregs();

		}

		else {		/* continuing from a breakpoint is hard */
#if DEBUG > 1
			if(debugflag) fprintf(FPRT2,
			     " : userpc=%#x, dot=%#x; adrflg=%d; excuting=%d\n",
					userpc, dot, adrflg, executing);
#endif

			/* if truly continuing from bkpt, execute instruction
			 * that should be where breakpoint was set	    */
			if   ((bkpt=scanbkpt(userpc))
			  && !(adrflg && args[0]=='\0') /*  'g' cmd */
/* ?? 0 bkpt */		  && !(userpc==entrypt && (cmd=='r'||cmd=='R')))  
			{
if(debug)fprintf(FPRT2, "EXECBKPT userpc=%#x USERPC=%#x\n",userpc,USERPC);
				execbkpt(bkpt, execsig);
				execsig = 0;
			}
			setbp();
			ptrace(runmode, pid, userpc, execsig);
			bpwait(); chkerr(); execsig=0;
			delbp();
			readregs();
			/* hanna backup if breakpoint */
			if(!signo) dot = SDBREG(PC) = userpc -= 2;
		}

		/* stopped by BPT instruction */

		if ((signo==0) && (bkpt=scanbkpt(userpc))) {
	loop1:

#if DEBUG
			if (debug) fprintf(FPRT2, "\n BPT code; '%s'%#x'%#x'%d",
				bkpt->comm,bkpt->comm[0],'\n',bkpt->flag);
#endif
			dot=bkpt->loc;
			if (bkpt->comm[0] != '\n') {
				cmdval = acommand(bkpt->comm);
				/* if not "really" breakpoint, i.e.
				 * user did not give k command, then
				 * cmdval == 1, and should execute
				 * breakpoint instruction, not count
				 * as a breakpoint in the loop, and
				 * continue to the next breakpoint
				 */
				if (cmdval) {
					execbkpt(bkpt,execsig);
					execsig=0;
					/* if at another breakpoint,
					 * handle as before (check for
					 * user command, by going to
					 * loop1).
					 */
					if ((signo == 0) &&
					    (bkpt=scanbkpt(userpc)))
						goto loop1;
					loopcnt++;  /* Don't count iteration */
					continue;
				}
			}
		}

		/* else not stopped by a BPT instruction */
		else {
			execsig=signo;
			if (execsig) break;
		}
	}	/* end of while loop */
#if DEBUG
 		if (debug) fprintf(FPRT2, "Returning from runpcs\n");
#endif
}


#define BPOUT 0
#define BPIN 1
INT bpstate = BPOUT;

/* endpcs() - kill child (user) process */
endpcs()
{
	REG BKPTR	bkptr;
#if DEBUG
 		if (debug) fprintf(FPRT2, "Entering endpcs with pid=%d\n", pid);
#endif
	if (pid) {
		errno = 0;
   		ptrace(EXIT,pid,0,0); pid=0;
		chkerr();

		/*  cleanup of breakpoint states not necessary.
		*	Only two modes being used are BKPTSET and 0.
		*/
/*		for (bkptr=bkpthead; bkptr; bkptr=bkptr->nxtbkpt) {
			if (bkptr->flag) bkptr->flag = BKPTSET;
		}
*/
	}
	bpstate=BPOUT;
}

/* setup() -initializes ptrace, forks the child (user) process, and
 *		waits for child.
 */
setup()
{
	close(fsym); fsym = -1;
	if ((pid = fork()) == 0)
	{
		ptrace(SETTRC,0,0,0);

		/* restore user interrupt handling mode */
		signal(SIGINT,sigint); signal(SIGQUIT,sigqit);
#if DEBUG
 		if (debug) fprintf(FPRT2, "About to doexec  pid=%d\n",pid);
#endif
		doexec(); exit(0);
	}
	else if (pid == -1) {
		error(NOFORK);
		longjmp(env, 1);
	}
	else {
		bpwait();

		readregs();
#if DEBUG > 1
		if(debugflag)
			fprintf(FPRT2, "setup(): userpc=%#x;\n", userpc);
#endif
#if DEBUG
		if (debug) fprintf(FPRT2, "About to open symfil = %s\n",symfil);
#endif
		fsym=open(symfil,0);
		if (errflg) {
			fprintf(FPRT1, "%s: cannot execute\n",symfil);
#if DEBUG
			 if (debug) fprintf(FPRT2, "%d %s\n", errflg, errflg);
#endif
			endpcs();
		}
	}
	bpstate=BPOUT;
}

/* execbkpt(bkptr, execsig) - execute the one instruction that was replaced
 *		by a preakpoint instruction.  It has the side effect of
 *		removing all breakpoints, by calling delbp().
 *
 *		In the case of the 3B, a NULL bkptr means that single
 *		stepping is requested, and that a breakpoint instruction
 *		has not just been encountered.
 */

static
execbkpt(bkptr, execsig)
BKPTR bkptr;
{
#if DEBUG
	if (debug) fprintf(FPRT2,"exbkpt: count=%d, loc=%#x; pid=%d\n",
			bkptr->count, bkptr->loc, pid);
#endif
#if DEBUG > 1
	if (debugflag)
		fprintf(FPRT2,"execbkpt: execsig=%d; userpc=%#x;\n",
			execsig,userpc);
#endif
	ptrace(SINGLE, pid, bkptr->loc, execsig);
	bkptr->flag=BKPTSET;
	bpwait(); chkerr(); readregs();
}

/* doexec() - performs exec call, after parsing arguments on sdb "run" command
 *		line.
 */
extern STRING environ;

doexec()
{
	char *argl[MAXARG], args[LINSIZ];
	register char c, redchar, *argsp, **arglp, *filnam;

	arglp = argl;
	argsp = args;
	*arglp++ = symfil;
	c = ' ';

	do {
		while (eqany(c, " \t")) {
			c = rdc();	/* get char from extern args[] */
		} 
		if (eqany(c, "<>")) {	/* redirecting I/O argument */
			redchar = c;
			do {
				c = rdc();
			} while (eqany(c, " \t"));
			filnam = argsp;
			do {
				*argsp++ = c;
				c = rdc();
			} while (!eqany(c, " <>\t\n"));
			*argsp++ = '\0';
			if (redchar == '<') {
				close(0);
				if (open(filnam,0) < 0) {
					fprintf(FPRT1,
						"%s: cannot open\n",filnam);
					 exit(0);
				}
			} else {
				close(1);
				if (creat(filnam,0666) < 0) {
					fprintf(FPRT1,
						"%s: cannot create\n",filnam);
					 exit(0);
				}
			}
		} else if (c == '"' || c == '\'') {  /*  quoted argument */
			char sc = c;
			*arglp++ = argsp;
			for (c = rdc(); c != sc; c = rdc()) {
				if (c == '\\')
					c = bsconv(rdc());
				*argsp++ = c;
			}
			c = rdc();
			*argsp++ = '\0';
		} else if (c != '\n') {		/* a regular argument */
			*arglp++ = argsp;
			do {
				*argsp++ = c;
				c = rdc();
			} while(!eqany(c, " <>\t\n"));
			*argsp++ = '\0';
		}
	} while (c != '\n');
	*arglp = (char *) 0;
#if DEBUG
	if (debug) {
		char **dap;
		fprintf(FPRT2,
			"About to exect(%s, %d, %d)\n",symfil,argl,environ);
		for (dap = argl; *dap; dap++) {
			fprintf(FPRT2, "%s, ", *dap);
		}
	}
#endif
	exect(symfil, argl, environ);
	perror("Returned from execve");
}

/* scanpkpt() - returns a pointer to the entry in the breakpoint list
 *		corresponding to the address given, if that is a
 *		breakpoint address.  Otherwise returns NULL pointer.
 */
BKPTR
scanbkpt(adr)
ADDR adr;
{
	REG BKPTR	bkptr;
	for(bkptr=bkpthead; bkptr; bkptr=bkptr->nxtbkpt) {
		if (bkptr->flag && bkptr->loc==adr) break;
	}
	return(bkptr);
}

/* delbp(). Remove breakpoints [delbp1() puts original instruction back] */

delbp()
{
	REG BKPTR	bkptr;

#if DEBUG > 1
	if(debugflag)
		fprintf(FPRT2, "delbp():\n");
#endif
	if (bpstate != BPOUT) {
		for (bkptr=bkpthead; bkptr; bkptr=bkptr->nxtbkpt) {
			if (bkptr->flag) {

				delbp1(bkptr);	/*  separate subr */
			}
		}
		bpstate=BPOUT;
	}
}

/* delbp1(bkptr) - deletes breakpoint given by bkptr */

static
delbp1(bkptr)
BKPTR	bkptr;
{
	register long adr;
	register int ptv;
	struct proct *procp;

	adr = bkptr->loc;
	ptv = ptrace(RIUSER, pid, adr, 0);
	if (!(errno && ptv == -1)) {
#if DEBUG > 1
		if (debugflag)
			fprintf(FPRT2, "delbp1(): adr=%#x; ptv=%#x;\n",
				adr,ptv);
#endif
		ptv = ptrace(WIUSER, pid, adr,
				(bkptr->ins & ~MASK) | (ptv & MASK));
		if (!(errno && ptv == -1)) return;
	}
	procp = adrtoprocp(adr);
	fprintf(FPRT1, "Cannot delete breakpoint:  %s:%d @ %d\n",
			procp->pname, adrtolineno(adr, procp), dot);
}

/* setbp() -
 * Insert breakpoints [setbp1() overwrites instruction space with bpt instr] *
 */
setbp()
{
	REG BKPTR	bkptr;

#if DEBUG > 1
	if(debugflag)
		fprintf(FPRT2, "setbp():\n");
#endif
	if (bpstate != BPIN) {
		for (bkptr=bkpthead; bkptr; bkptr=bkptr->nxtbkpt) {
			if (bkptr->flag) {
				setbp1(bkptr);	/*  separate subr */
			}
		}
		bpstate=BPIN;
	}
}

/* setpb1(bkptr) - sets breakpoint given by bkptr */

static
setbp1(bkptr)
BKPTR	bkptr;
{
	register long adr;
	register int ptv;
	struct proct *procp;
	adr = bkptr->loc;
	errno = 0;
	ptv = ptrace(RIUSER, pid, adr, 0);

	if (!(errno && ptv == -1)) {
		bkptr->ins = ptv;
#if DEBUG > 1
		if (debugflag)
			fprintf(FPRT2, "setbp1(): adr=%#x; ins=%#x\n", 
				adr, ptv);
#endif
		ptv = ptrace(WIUSER, pid, adr, BPT | ptv & MASK);
		if (!(errno && ptv == -1)) return;
	}

	procp = adrtoprocp(adr);
	fprintf(FPRT1, "Cannot set breakpoint:  %s:%d @ %d\n",
			procp->pname, adrtolineno(adr, procp), dot);
}

bpwait()
{
	REG ADDR w;
	INT stat;

	signal(SIGINT, SIG_IGN );
#if DEBUG
	if (debug) fprintf(FPRT2, "Waiting for pid %d\n", pid);
#endif
	while ((w = wait(&stat)) != pid  &&  w!=-1) ;
#if DEBUG
	if (debug) fprintf(FPRT2, "Ending wait\n");
	if (debug) fprintf(FPRT2, "w = %d; pid = %d; stat = %#x;\n",w,pid,stat);
#endif
/*	dot = userpc = SDBREG(PC);	*/
	signal(SIGINT,sigint);
	if (w == -1) {
		pid = 0;
		errflg=BADWAIT;
	}
	else if ((stat & 0177) != 0177) {
		if (signo = stat&0177) sigprint();
		if (stat&0200) {
			error(" - core dumped");
			close(fcor);
			corfil = "core";
			setcor();	/* get most recent core dump */
		}
		pid=0;
		errflg=ENDPCS;
	}
	else {
		signo = stat>>8;
#if DEBUG
    		if (debug)
		    fprintf(FPRT2, "stat = %#x, signo = %d\n", stat, signo);
#endif
		if (signo != SIGTRAP &&
		    ptrace(RUREGS, pid, SYSREG(PC), 0) != adsubn)
		{
			sigprint();
		}
		else	signo = 0;
	}
}

readregs()
{
	REG i;

	uu.u_ar0 = (int *)ptrace(RUREGS, pid, offsetof(user_t, u_ar0));

	if ((int)uu.u_ar0 == -1)
	{
		perror("Cannot ptrace child process; try again.");
		endpcs();
		longjmp(env, 1);
	}

	if((int)ptrace(RFPUREGS,pid,&proc_fpp,0) == -1)
		fpuinithh = 0;
	else
		fpuinithh = 1;

	for (i = NUMREGLS - 1; i >= 0; --i) 
	{
        	SDBREG(reglist[i].roffs) =
                    ptrace(RUREGS, pid, SYSREG(reglist[i].roffs), 0);
	}

	ptrace(RFPUREGS, pid, &proc_fpp, 0); 

	if( pid == 0 && PC < 0 )
	{
		error( NOPCS );
		return(ERROR);
	}
	else
	{
		dot = userpc = SDBREG(PC);
	}
}

char 
readchar() {
	lastc = *argsp++;
	if (lastc == '\0') lastc = '\n';
	return(lastc);
}

char
rdc()
{
	register char c;

	c = *argsp++;
	return(c == '\0' ? '\n' : c);
}

