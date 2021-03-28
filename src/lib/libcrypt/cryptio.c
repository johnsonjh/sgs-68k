/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) cryptio.c: version 25.1 created on 12/2/91 at 19:07:36	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)cryptio.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libcrypt_x:cryptio.c	1.7"		*/

#ident	"@(#)lib/libcrypt:cryptio.c	25.1"

#include <stdio.h>
#include <signal.h>
#define	READER		0
#define	WRITER		1
#define KSIZE 	8
static char key[KSIZE+1];
/*  Global Variables  */
static struct header {
	long offset;
	unsigned int count;
};
static int crypt_pid[_NFILE];
static FILE *fp[_NFILE];  

int run_setkey(p, keyparam)
int   p[2];
char *keyparam;
{
	if(cryptopen(p) == -1)
		return -1;
	strncpy(key, keyparam, KSIZE);
	if(*key == 0) {
		crypt_close(p);
		return(0);
	}
	if(writekey(p,key) == -1)
		return -1;
	return(1);
}

static char cmd[] = "exec /bin/crypt -p 2>/dev/null";
static int
cryptopen(p)
int	p[2];
{
	int pid;
	int n;
	FILE *fpin, *fpout;
	char c;	

	if(p[0] != 0 || p[1] != 0) { /* shutdown old crypt process which isn't needed */
		crypt_close(p); 
	}
	if(!(pid = p2open(cmd, &fpin, &fpout)))  {
		return(-1);
	}
	p[0] = fileno(fpin);
	p[1] = fileno(fpout);
	crypt_pid[p[WRITER]] = pid;
	crypt_pid[p[READER]] = pid; 
	fp[p[0]] = fpin;
	fp[p[1]] = fpout;
	if((n = read(p[WRITER], &c, 1))!= 1) { /* check that crypt is working on
					   other end */
		crypt_close(p); /* remove defunct process */
		return(-1); 
	}
	return(1);
}

static int writekey(p,keyarg)
int p[2];
char *keyarg;
{
	void (*pstat) ();
	pstat = signal(SIGPIPE, SIG_IGN); /* don't want pipe errors to cause
					     death */
	if(write(p[READER], keyarg, KSIZE) != KSIZE) {
		crypt_close(p); /* remove defunct process */
		signal(SIGPIPE,pstat);
		return(-1);
	}
	(void)signal(SIGPIPE,pstat);
	return(1);
}


int
run_crypt(offset, buffer, count, p)
long offset;
char	*buffer;
unsigned int count;
int p[2];
{
	int	n;
	int string_size;
	struct header header;
	void (*pstat) ();

	header.count = count;
	header.offset = offset;
	pstat = signal(SIGPIPE, SIG_IGN);
	if(write(p[READER], (char *)&header, sizeof(header))!=sizeof(header)) {
		crypt_close(p);
		signal(SIGPIPE, pstat);
		return -1;
	}
	if((n = write(p[READER], buffer, count)) < count) {
		crypt_close(p);
		signal(SIGPIPE, pstat);
		return(-1);
	}
	if((n = read(p[WRITER], buffer,  count))< count) {
		crypt_close(p);
		signal(SIGPIPE, pstat);
		return(-1);
	}
	signal(SIGPIPE, pstat);
	return(0);
}

makekey(b)
int b[2];
{
	register int i;
	long gorp;
	char tempbuf[KSIZE], *a, *temp;

	a = key;
	temp = tempbuf;
	for(i = 0; i < KSIZE; i++)
		temp[i] = *a++; 
	gorp = getuid() + getgid();

	for(i = 0; i < 4; i++)
		temp[i] ^= (char)((gorp>>(8*i))&0377); 

	if (cryptopen(b) == -1)
		return(-1);
	if (writekey(b,temp) == -1)
		return(-1);
	return(0);
}


crypt_close(p)
int p[2];
{
	int pid;
	pid = crypt_pid[p[0]];
	if(pid != crypt_pid[p[1]]) {
		fprintf(stderr,"p[0] not from same run_setkey as p[1]\n");
		exit(1);
	}
	(void) kill(pid, 9);
	(void) p2close(fp[p[0]], fp[p[1]]);
	p[0] = p[1] = 0;
}
