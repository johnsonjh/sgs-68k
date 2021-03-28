#ident	"@(#)clnt_perr.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#ident	"@(#)clnt_perr.c	3.1 LAI System V NFS Release 3.0/V3 source"
#ifndef lint
#ident	"@(#)clnt_perror.c 1.1 85/02/08 Copyr 1984 Sun Micro"
#endif

/*
 * clnt_perror.c
 *
 * Copyright (C) 1984, Sun Microsystems, Inc.
 *
 */
#include <stdio.h>
#ifndef EXOS
#include <sys/types.h>
#endif
#include <rpc/types.h>
#include <rpc/xdr.h>
#include <rpc/auth.h>
#include <rpc/clnt.h>
#include <rpc/rpc_msg.h>
extern char *sys_errlist[];
extern int sys_nerr;
static char rpc_syserr_buff[32];

/* #define rpc_syserr(e) ( (e >= 0 && e < sys_nerr) ? \
			sys_errlist[e] : \
		(sprintf(rpc_syserr_buff, "%d", e), rpc_syserr_buff) ) */

/*
 * Print reply error info
 */
void
clnt_perror(rpch, s)
	CLIENT *rpch;
	char *s;
{
	struct rpc_err e;
	void clnt_perrno();

	CLNT_GETERR(rpch, &e);
	fprintf(stderr, "%s: ", s);
	switch (e.re_status) {
		case RPC_SUCCESS:
		case RPC_CANTENCODEARGS:
		case RPC_CANTDECODERES:
		case RPC_TIMEDOUT:
		case RPC_PROGUNAVAIL:
		case RPC_PROCUNAVAIL:
		case RPC_CANTDECODEARGS:
			clnt_perrno(e.re_status);
			break;
		case RPC_CANTSEND:
			clnt_perrno(e.re_status);
/*		fprintf(stderr, "; errno = %s", rpc_syserr(e.re_errno)); */
		if  (e.re_errno >= 0 && e.re_errno < sys_nerr) 
	       	  fprintf(stderr, "; errno = %s", sys_errlist[e.re_errno]); 
		else
fprintf(stderr, "; errno = %s", (sprintf(rpc_syserr_buff, "%d", e.re_errno), rpc_syserr_buff) );
			break;
	
		case RPC_CANTRECV:
			clnt_perrno(e.re_status);
/*		fprintf(stderr, "; errno = %s", rpc_syserr(e.re_errno)); */
		if  (e.re_errno >= 0 && e.re_errno < sys_nerr) 
	       	  fprintf(stderr, "; errno = %s", sys_errlist[e.re_errno]); 
		else
fprintf(stderr, "; errno = %s", (sprintf(rpc_syserr_buff, "%d", e.re_errno), rpc_syserr_buff) );
			break;
	
		case RPC_VERSMISMATCH:
			clnt_perrno(e.re_status);
			fprintf(stderr, "; low version = %lu, high version = %lu", e.re_vers.low, e.re_vers.high);
			break;
	
		case RPC_AUTHERROR:
			clnt_perrno(e.re_status);
			fprintf(stderr, "; why = ");
			switch (e.re_why) {
			case AUTH_OK:
				fprintf(stderr, "AUTH_OK");
				break;
	
			case AUTH_BADCRED:
				fprintf(stderr, "AUTH_BOGUS_CREDENTIAL");
				break;
	
			case AUTH_REJECTEDCRED:
				fprintf(stderr, "AUTH_REJECTED_CREDENTIAL");
				break;
	
			case AUTH_BADVERF:
				fprintf(stderr, "AUTH_BOGUS_VERIFIER");
				break;
	
			case AUTH_REJECTEDVERF:
				fprintf(stderr, "AUTH_REJECTED_VERIFIER");
				break;
	
			case AUTH_TOOWEAK:
				fprintf(stderr, "AUTH_TOO_WEAK (remote error)");
				break;
	
			case AUTH_INVALIDRESP:
				fprintf(stderr, "AUTH_INVALID_RESPONSE");
				break;
	
			default:
				fprintf(stderr, "AUTH_UNKNOWN_FAILURE");
				break;
			}
			break;
	
		case RPC_PROGVERSMISMATCH:
			clnt_perrno(e.re_status);
			fprintf(stderr, "; low version = %lu, high version = %lu", e.re_vers.low, e.re_vers.high);
			break;
	
		default:
			fprintf(stderr, "RPC_UNKNOWN_FAILURE; s1 = %lu, s2 = %lu", e.re_lb.s1, e.re_lb.s2);
			break;
	}
	fprintf(stderr, "\n");
}

/*
 * This interface for use by clntrpc
 */
void
clnt_perrno(num)
	enum clnt_stat num;
{
	switch (num) {
		case RPC_SUCCESS:
			fprintf(stderr, "RPC_SUCCESS");
			break;
	
		case RPC_CANTENCODEARGS:
			fprintf(stderr, "RPC_CANT_ENCODE_ARGS");
			break;
	
		case RPC_CANTDECODERES:
			fprintf(stderr, "RPC_CANT_DECODE_RESULTS");
			break;
	
		case RPC_CANTSEND:
			fprintf(stderr, "RPC_CANT_SEND");
			break;
	
		case RPC_CANTRECV:
			fprintf(stderr, "RPC_CANT_RECV");
			break;
	
		case RPC_TIMEDOUT:
			fprintf(stderr, "RPC_TIMED_OUT");
			break;
	
		case RPC_VERSMISMATCH:
			fprintf(stderr, "RPC_VERSION_MISMATCH");
			break;
	
		case RPC_AUTHERROR:
			fprintf(stderr, "RPC_AUTH_ERROR");
			break;
	
		case RPC_PROGUNAVAIL:
			fprintf(stderr, "RPC_REMOTE_PROGRAM_UNAVAILABLE");
			break;
	
		case RPC_PROGVERSMISMATCH:
			fprintf(stderr, "RPC_PROGRAM_MISMATCH");
			break;
	
		case RPC_PROCUNAVAIL:
			fprintf(stderr, "RPC_UNKNOWN_PROCEDURE");
			break;
	
		case RPC_CANTDECODEARGS:
			fprintf(stderr, "RPC_CANT_DECODE_ARGS");
			break;
		case RPC_UNKNOWNHOST:
			fprintf(stderr, "RPC_UNKNOWNHOST");
			break;
		case RPC_PMAPFAILURE:
			fprintf(stderr, "RPC_PMAP_FAILURE");
			break;
		case RPC_PROGNOTREGISTERED:
			fprintf(stderr, "RPC_PROG_NOT_REGISTERED");
			break;
		case RPC_SYSTEMERROR:
			fprintf(stderr, "RPC_SYSTEM_ERROR");
			break;
		default:
			fprintf(stderr, "Unknown RPC error");
			break;
	}
}

/*
 * A handle on why an rpc creation routine failed (returned NULL.)
 */
struct rpc_createerr rpc_createerr;

clnt_pcreateerror(s)
	char *s;
{

	fprintf(stderr, "%s: ", s);
	clnt_perrno(rpc_createerr.cf_stat);
	switch (rpc_createerr.cf_stat) {
		case RPC_PMAPFAILURE:
			fprintf(stderr, " - ");
			clnt_perrno(rpc_createerr.cf_error.re_status);
			break;

		case RPC_SYSTEMERROR:
if  (rpc_createerr.cf_error.re_errno >= 0 && rpc_createerr.cf_error.re_errno < sys_nerr) 
      fprintf(stderr, " - %s", sys_errlist[rpc_createerr.cf_error.re_errno]); 
		else
fprintf(stderr, " - %s", (sprintf(rpc_syserr_buff, "%d", rpc_createerr.cf_error.re_errno), rpc_syserr_buff) );
	
/*	fprintf(stderr, " - %s", rpc_syserr(rpc_createerr.cf_error.re_errno));*/
			break;

	}
	fprintf(stderr, "\n");
}
