/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) bio.c: version 23.1 created on 11/15/90 at 13:17:58	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)bio.c	23.1	11/15/90 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)sdb/com:bio.c	23.1"

/*		Copyright (c) 1985 AT&T		*/
/*		All Rights Reserved		*/
	/*	OLD MOT:bio.c	6.1		*/

#include "head.h"

/*
 * NAMES:  bread(), blseek(), binit()
 *
 * DESCRIPTION:
 *	 This is a buffered read package.
 *
 *       Bread may be called with a negative nbytes which causes it to
 *      read backwards.  In this case, buffer should point to the first
 *      byte following the buffer.  If only a partial read is possible
 *      (due to beginning of file), only the last bytes of the buffer
 *      will be filled.
 */


bread(brs, buff, nbytes)
struct brbuf *brs; char *buff; {
	register int k, nb;

	if (nbytes > 0) {
		for (nb=nbytes; nb>0; nb--) {
			if (brs->nr == 0) {
				brs->nr = read(brs->fd,brs->next=brs->b,BRSIZ);
				brs->nl = 0;
				if (brs->nr < 0) return(-1);
				if (brs->nr == 0) return(nbytes-nb);
				}
			*buff++ = *brs->next++;
			brs->nr--;
			brs->nl++;
			}
		}
	else {
		nbytes = -nbytes;
		for (nb=nbytes; nb>0; nb--) {
			if (brs->nl == 0) {
				if ((k=tell(brs->fd)) >= BRSIZ + brs->nr) {
					lseek(brs->fd,(long)-(BRSIZ+brs->nr),1);
					brs->nl = read(brs->fd, brs->b, BRSIZ);
				} else {
					lseek(brs->fd, 0L, 0);
					k = k - brs->nr;
					if (k < 0) k = 0;
					brs->nl = read(brs->fd, brs->b, k);
				}
				if (brs->nl == 0) return(nbytes-nb);
				brs->next = brs->b + brs->nl;
				brs->nr = 0;
				}
			*--buff = *--brs->next;
			brs->nr++;
			brs->nl--;
			}
		}
	return(nbytes);
	}

blseek(brs, offset, flag) 
struct brbuf *brs; long offset; {
	brs->nl = 0;
	brs->nr = 0;
	return(lseek(brs->fd,offset,flag));
	}

binit(brs)
struct brbuf *brs; {
	brs->nl = brs->nr = 0;
}

long
tell(fildes) {
	return(lseek(fildes, 0L, 1));
}
