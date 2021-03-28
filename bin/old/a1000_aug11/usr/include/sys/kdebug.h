#ifndef SYS_KDEBUG_H
#define SYS_KDEBUG_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) kdebug.h: version 2.1 created on 5/22/89 at 19:08:10	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)kdebug.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#include "sys/systm.h"

#ifndef	SYMNMLEN
#define	SYMNMLEN	8	/* from head/syms.h	*/
#endif
#ifndef	FILNMLEN
#define	FILNMLEN	14	/* from head/syms.h	*/
#endif
#define	KDEB_INTC	03	/* CTRL-C is our default interrupt char */
#define	LOADAVG		1
#define	PCHISTSZ	256
#define	PCHISTBACKCNT	5
#define	PCHISTFORWCNT	10
#define	XBUFSIZE	256
#define	TSTR_LEN	64
#define	putchar		prf_putchar
#define	getchar		prf_getchar
#define	chkstdin	prf_chkstdin
#define	cons_intc	prf_cons_intc
#define	cons_ttyint	prf_cons_ttyint
#define	printf		kd_printf
#define	sprintf		kd_sprintf

#define n_nptr0		_n._n_nptr[0]
#define n_nptr1		_n._n_nptr[1]
#ifndef	n_name
#define n_name		_n._n_name
#endif
#ifndef	n_nptr
#define n_nptr		_n._n_nptr[1]
#endif
#ifndef	n_zeroes
#define n_zeroes	_n._n_n._n_zeroes
#endif
#ifndef	n_offset
#define n_offset	_n._n_n._n_offset
#endif

/*
**	Optimum sizes with enough reserve.
*/
#define	NKSYMFILE	300
#define	NKSYMTEXT	2000
#define	NKSYMDATA	500
#define	NKSYMBSS	500
#define	KSYM_SYSNAME	"/syst"

typedef	struct	ksymfile
	{
		int		textorg;
		int		dataorg;
		int		bssorg;
		unsigned long	n_value;
		char		fname[FILNMLEN+1];
	} ksymfile_t;

typedef	struct	ksyment
	{
		union	/* must be identical to union in 'struct syment' */
		{
			char		_n_name[SYMNMLEN];/* old COFF version */
			struct
			{
				long	_n_zeroes;/* new == 0 */
				long	_n_offset;/* offset into string table */
			} _n_n;
			char		*_n_nptr[2];/* allows for overlaying */
		} _n;
		int		n_terminator;
		unsigned long	n_value;
		int		n_line;
		int		e_line;
	} ksyment_t;

extern	char	*strcat();
extern	char	*strcpy();
extern	char	*getarg();
extern	char	xinbuf[];
extern	int	atox(), atoi();
extern	int	kerneloffset, etext[], edata[], end[];
extern	int	prf_cons_intc;		/* if set, its our interrupt	*/
extern	int	prf_cons_ttyint;	/* got a tty interrupt char	*/
extern	int	Debug_cc;		/* default ^T: in os/sysarix.c	*/
extern	int	kd_longjmp;
extern	int	kd_exception;
extern	int	*kd_jmpptr;
extern	int	kd_dosync;
extern	int	kd_prtsyms;
extern	int	kd_prtlnum;
extern	int	kd_prtfile;
extern	int	kd_prtargs;
extern	int	kd_intc;		/* default interrupt character	*/
extern	int	ksym_tab_present;
extern	int	ksym_str_present;
extern	char	atext[];
extern	char	acode[];
extern	ushort	mem[];
extern	ushort	*currpc;

extern	ksyment_t	*kd_addrtosym();
extern	ksyment_t	*kd_texttosym(),  *kd_datatosym(),  *kd_bsstosym();
extern	ksyment_t	*kd_nametotext(), *kd_nametodata(), *kd_nametobss();
extern	char		*kd_symtoname(),  *kd_symtoline(),  *kd_symalloc();
extern	char		*kd_texttofile(), *kd_datatofile(), *kd_bsstofile();
extern	char		*kd_addrtofile(), *kd_compile();

#endif /* SYS_KDEBUG_H */
