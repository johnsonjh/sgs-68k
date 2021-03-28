#ident	"@(#)rand.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	rand.c	7.1		*/
/* rand.c */
/* static char SCCSID[] = "rand.c:	2.1 83/07/08"; */

/*	MC68000 Optimizer */

#include "optim.h"



/* Operands are managed in a linked hash table. */

#define	RANDSIZE	511	/* number of table slots */

/* Define table entries */

struct randentry
{
    RAND rand;			/* this is the actual operand */
    struct randentry * next;	/* pointer to next linked entry */
};

typedef struct randentry RANDENTRY;

#define	RENULL	((RANDENTRY *) 0) /* null table entry */

static RANDENTRY * hashtab[RANDSIZE];	/* declare the hash table */
/* Some simple service functions first. */

/* rhash -- hash an operand string
**
** This routine returns a hash number for an operand string.
** The algorithm was arrived at empirically to give good scatter
** on a large set of test data.
*/

unsigned long
rhash(s,n)
register char * s;		/* operand string */
register int n;			/* string length */
{
    register unsigned long hashval = *s++;

    while (--n > 0)
	hashval = (hashval << 3) + *s++;
    
    return (hashval);
}
/* clrhash -- initialize the hash table */

void
clrhash()
{
    register int i;
    register RANDENTRY ** rptr;

    for (i = RANDSIZE, rptr = hashtab; i != 0; *rptr++ = RENULL, i--)
	;
    
    return;
}
/* saverand -- save an operand, return its operand pointer
**
** This routine looks for an existing RAND structure, given a character
** string.  If the structure exists already, a pointer to it is returned.
** Otherwise one of two things happens:
**	If the creation flag is set, a new RAND is built, and its
**	pointer is returned; otherwise, an RNULL pointer is returned.
*/

RAND *
saverand(ptr, len, type, ruses, create)
char * ptr;			/* operand string */
int len;			/* operand string length */
RTYPE type;			/* operand type */
REG ruses;			/* register data */
boolean create;			/* true if okay to create new one */
{
    char * memcpy();
    char * temp;
    int memcmp();
    int slot = rhash(ptr,len) % RANDSIZE;
				/* choose initial hash table slot */
    register RANDENTRY ** rptr = &hashtab[slot];
				/* prepare to search that slot's chain */
    
    /* run down the chain of entries dangling from this slot, looking for
    ** a character string match.
    */

    while (*rptr != RENULL && 
           ((temp = (*rptr)->rand.rname)[len] != '\0' ||  /* check length */
	   memcmp(temp, ptr, len) != 0))
	rptr = &((*rptr)->next);
    
    /* Either we have run out of slots or found an existing equivalent
    ** operand.  Check which.
    */

    if (*rptr != RENULL)
	return(&((*rptr)->rand)); /* an existing operand.  Return its pointer */
    
    if (! create)		/* check for permission to create, since not
				** found
				*/
	return (RNULL);
/* The desired operand is not in the table.  We must add it.  "rptr" points
** at the place to store the pointer to the new table entry.
*/

    {
	char * newname;		/* string in which operand is saved */
	RANDENTRY * newentry;	/* new table entry */

	/* prepare to copy operand string into permanent memory */

	newname = getspace((unsigned)(len + 1));
	(void) memcpy(newname, ptr, len);
	newname[len] = '\0';	/* make null-terminated string */

	newentry = GETSTR(RANDENTRY); /* get new table entry */

	/* fill in the pieces */

	*rptr = newentry;	/* put new entry at end */
	newentry->next = RENULL; /* end of the chain */

	newentry->rand.rname = newname;
	newentry->rand.rtype = type;
	newentry->rand.ruses = ruses;

	/* machine dependent fill out RAND structure */
	fillrand(&newentry->rand);

	return( &newentry->rand ); /* return the completed entry */
    }
}
/* findrand -- find an existing operand string
**
** Findrand finds an existing operand and returns a pointer to its
** operand structure.
*/

RAND *
findrand(ptr, flag)
char * ptr;			/* null terminated string */
boolean flag;			/* true if operand must exist */
{
    int strlen();
    RAND * r =  saverand(ptr, strlen(ptr), (RTYPE) 0, REG_NONE, false);
				/* find operand, don't create new one */

    if (! flag || r != RNULL)	/* if operand needn't exist, return what we
				** got; always return real pointer
				*/
	return(r);

    oerr("Can't find operand");	/* otherwise error */

/*NOTREACHED*/
}

	/* chgregrand() -- change all occurances of a register and it's mask
	** to a another register in the entire operand hash table
    */
void
chgregrand(snew, masknew, sold, maskold)
char	* snew;					/* character id of new register */
REG		masknew;				/* register mask of new */
char	* sold;					/* character id of old register */
REG		maskold;				/* register mask of old */
{
	register char *s1;			/* string compare pointer */
	register char *s2;			/* string compare pointer */
	char * rstr;				/* major string search point */
	register RANDENTRY *reptr;	/* RAND entry link pointer */
	register RANDENTRY **rptr;  /* RAND table slot pointer */
	register int i;				/* RAND table index counter */
	char *ss1;					/* work pointer */
	char *ss2;					/* work pointer */

	if (strlen(snew) != strlen(sold))
		oerr("chgregrand() old and new register id not same size\n");
	/* scan all entries in the hash table */
	for (i=RANDSIZE, rptr=hashtab; i!=0; rptr++, i--)
	  /* run down chain for this slot */
	  for (reptr=*rptr; reptr!=RENULL; reptr=reptr->next)
		/* see if the name matches the one we are to replace */
		for (rstr=reptr->rand.rname; *rstr!=0; rstr++)
		{	for (s1=rstr, s2=sold; *s1==*s2 && *s2 != 0; s1++, s2++);
			if (*s2==0)
			{	/* found an instance. replace it */
				if (tempflag) printf("replacing reg in \"%s\"\n", reptr->rand.rname);
				for (ss1=rstr, ss2=snew; *ss2 != 0; ss1++, ss2++)
						*ss1 = *ss2;
				if ( !( (reptr->rand.ruses | reptr->rand.owuses) & maskold) )
					oerr("chgregrand - missing reg use\n");
				if (reptr->rand.ruses & maskold)	/* in ea field */
				  {	reptr->rand.ruses ^= maskold;	/* turn off old mask */
					reptr->rand.ruses |= masknew;	/* and on new mask */
					}
				if (reptr->rand.rindx & maskold)	/* if index register */
				  {	reptr->rand.rindx ^= maskold;	/* off old mask */
					reptr->rand.rindx |= masknew;	/* on new mask */
					}
#ifdef m68k
				if (reptr->rand.owuses & maskold)
				{	reptr->rand.owuses ^= maskold;		/* off old mask */
					reptr->rand.owuses |= masknew;		/* on new mask */
					}
#endif
			} /* end string replacement */

		} /* end string compare search */

}
