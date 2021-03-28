/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) ofile.h: version 24.1 created on 10/28/91 at 18:36:04	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)ofile.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ifndef SYS_OFILE_CHAIN_H
#define SYS_OFILE_CHAIN_H

#include <sys/immu.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/file.h>

#define CHAIN_SIZE		PTSIZE

#define NOFILES_PER_CHAIN	((CHAIN_SIZE - sizeof(struct ofile_chain *)) / \
				  (sizeof(struct file *) + sizeof(char)))

typedef struct ofile_chain {
	struct ofile_chain	*link;
	struct file		*fp[NOFILES_PER_CHAIN];
	uchar			oflag[NOFILES_PER_CHAIN];
} ofile_chain_t;

#define GET_OFILE(I)		((I) < NOFILES_IN_U ? u.u_fp[I] : \
				get_chained_ofile(I))

#define GET_OFLAG(I)		((I) < NOFILES_IN_U ? u.u_oflag[I] : \
				get_chained_oflag(I))

#ifdef INKERNEL

#define SET_OFILE(I, FP)	{ ASSERT(I >= 0); \
				((I) < NOFILES_IN_U ? u.u_fp[I] = (FP) : \
				set_chained_ofile((I), (FP)));  \
				}

#define SET_OFLAG(I, VAL)	((I) < NOFILES_IN_U ? u.u_oflag[I] = (VAL) : \
				set_chained_oflag((I), (VAL)))

extern struct file 	*get_chained_ofile();
extern struct file 	*set_chained_ofile();
extern uchar 		get_chained_oflag();
extern uchar 		set_chained_oflag();
extern int		allocate_u_ofile();
extern int		dup_ofiles(); 
extern void		close_ofiles();

#endif /* INKERNEL */

#endif /* SYS_OFILE_CHAIN_H */
