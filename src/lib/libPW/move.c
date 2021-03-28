/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) move.c: version 25.1 created on 12/2/91 at 18:44:43	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)move.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	OLD ARETE :move.c	1.2	*/

#ident	"@(#)move.c:move.c	25.1"


/*
	Copies `n' characters from string `a' to string `b'.
*/

char *move(a,b,n)
char *a,*b;
unsigned n;
{
	register char *x, *y;
	register short *x1, *y1;
	register int m;

	/*
		Test for non-zero number of characters to move
	*/
	if (m=n) {
		x = a;
		y = b;
		/*
			Compare the low order bits of the two pointers
			If both are equal (both even or both odd) then
			move words instead of bytes
		*/
		if ((((int)x^(int)y)&1) == 0) {
			/*
				If both odd move one byte to make both even
			*/
			if ((int)x&1) {
				*y++ = *x++;
				n--;
			}
			/*
				Add one to n/2 so that we can use --m instead
				of m--; --m is more efficient
			*/
			x1 = (short *)x;
			y1 = (short *)y;
			for (m=n/2+1; --m; )
				*y1++ = *x1++;
			m = n&1;
			x = (char *)x1;
			y = (char *)y1;
		}
		/*
			Add one to m so that we can use --m instead of m--;
			--m is more eficient.
		*/
		for (++m; --m; ) *y++ = *x++;
	}
}
