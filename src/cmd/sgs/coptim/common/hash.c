#ident	"@(#)hash.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	hash.c	7.1		*/
/*	Copyright 1985 Motorola Inc.			*/
/* derived from 	hsearch.c	4.1 	*/
/* Compile time switches:

   DRIVER - compile in a main program to drive the tests.
*/

#include <stdio.h>
#include "declar.h"

/* Prototypes for static functions */
static struct entry * build _PARMS((struct hnode **, struct hnode *, struct entry ));
static unsigned  crunch _PARMS((char *));

#define SUCCEED		0
#define FAIL		1
#define FALSE		0
#define repeat		for(;;)
#define until(A)	if(A) break;

typedef HNODE *TABELEM; /* What the table contains (TABle ELEMents) */
static ENTRY *build();
static unsigned int prcnt;	/* Number of probes this item */
static unsigned int crunch();


#ifdef DRIVER
static void hdump();
static HASHTAB testtable;	/* Address of the test hast table */

/*F*********************** FUNCTION HEADER *******************************

FUNCTION NAME:

	main

ACTION:

	Used for testing the hash functions.  The rest of the routines
	have undergone changes so this may no longer work

PARAMETERS:

	Receives:

	Returns:

EXTERNAL DATA REFERENCES:


COMPLEX LOCALS:


FUNCTIONS CALLED:


CALLED BY:


METHOD:

	self explanatory

NOTES:

	none

*FE********************************************************************/

main()
{
    char line[80];	/* Room for the input line */
    int i = 0;		/* Data generator */
    ENTRY *res;		/* Result of hsearch */
    ENTRY *new;		/* Test entry */
    char action;

    if(hcreat(&testtable, 5))
	printf("Length = %u\n", testtable.length);
    else {
	fprintf(stderr, "Out of core\n");
	exit(FAIL);
    }

    repeat {
	hdump(&testtable);
	do
	  {
	  printf("Remove or Add the key? (r,a): ");
	  scanf("%c",&action);
	  while( getchar() != '\n' );
	  action = toupper(action);
	  }
	while ( action != 'R' && action != 'A');
	printf("Enter the key: ");
	until (EOF == scanf("%s", line));
	while( getchar() != '\n' );
	new = (ENTRY *) malloc(sizeof(ENTRY));
	if(new == NULL) {
	    fprintf(stderr, "Out of core \n");
	    exit(FAIL);
	}
	else {
	    new->key = malloc((unsigned) strlen(line) + 1);
	    if(new->key == NULL) {
		fprintf(stderr, "Out of core \n");
		exit(FAIL);
	    }
	    strcpy(new->key, line);
	    new->data = malloc(sizeof(int));
	    if(new->data == NULL) {
		fprintf(stderr, "Out of core \n");
		exit(FAIL);
	    }
	    *new->data = i++;
	}
	if (action == 'A')
		{
		res = hsearch(&testtable, hashd, strcmp, *new, ENTER);
		printf("The number of probes required was %d\n", prcnt);
		if(res == (ENTRY *) 0)
		    printf("Table is full\n");
		else {
		    printf("Success: ");
		    printf("Key = %s, Value = %d\n", res->key, *res->data);
		   } 
		}
	else
		{
		res = hsearch(&testtable, hashd, strcmp, *new, FIND);
		if (res)
		  {
		  res = (ENTRY *) removekey(&testtable, hashd, res);
		  if (res)
			printf("Key succesfully removed\n");
		  else
			printf("ERROR - Key was not found\n");
		  }
		else
		  printf("Key does not exist\n");
		}
    }
    exit(SUCCEED);
}
#endif

/*F*********************** FUNCTION HEADER *******************************

FUNCTION NAME:

	hcreat

ACTION:

	Create a hash table no smaller than "size" number of elements.

PARAMETERS:

	Receives:

		ht: pointer to structure describing hash table
		size: number of elements in table

	Returns:

		returns TRUE if table created

EXTERNAL DATA REFERENCES:

	none

COMPLEX LOCALS:

	none

FUNCTIONS CALLED:

	calloc

CALLED BY:

	main

METHOD:

	self explanatory

NOTES:

	none

*FE********************************************************************/

int
hcreat(ht, size)
HASHTAB *ht;	/* Pointer to structure describing hash table */
int size;		/* Minimum size for hash table */
{
    unsigned int unsize;	/* Holds the shifted size */
    int m;			/* Log base 2 of length */

    if(size <= 0)
	return(FALSE);

    unsize = size;	/* +1 for empty table slot; -1 for ceiling */
    ht->length = 1;		/* Maximum entries in tabbe */
    m = 0;		/* Log2 length */
    while(unsize) {
	unsize >>= 1;
	ht->length <<= 1;
	m++;
    }

    ht->table = (TABELEM *) calloc(ht->length, sizeof(TABELEM));
    return(ht->table != NULL);
}
#    define COMPARE(A, B) (*cmp)((A), (B))
#    ifdef SORTUP
#        define CMPFUNC(A, B) (COMPARE((A), (B)) > 0)
#    else
#    ifdef SORTDOWN
#        define CMPFUNC(A, B) (COMPARE((A), (B)) < 0)
#    else
#        define CMPFUNC(A, B) (COMPARE((A), (B)) != 0)
#    endif
#    endif


/*F*********************** FUNCTION HEADER *******************************

FUNCTION NAME:

	hsearch

ACTION:

	Function ENTERs or FINDs "item" in hashtable "ht", and returns
	a pointer to "item"s location.

PARAMETERS:

	Receives:

		ht: hash table -- a table of pointers to sorted items 
				   which are chained together
	        hashfunc: function which performs the hashing on item 
			  to determine which bucket in "ht" to begin the search.
		cmp: function which compares two items and returns a
		     value indicating one of the following states: less than,
		     equality,
	       
		item: item to be inserted or sought

		action: either ENTER or FIND

	Returns:

		a pointer to the "item"s location

EXTERNAL DATA REFERENCES:

	hash table indicated by "ht"

COMPLEX LOCALS:

	none

FUNCTIONS CALLED:

	(*hashfunc)
	(*cmp)
	build

CALLED BY:

	leafvalue
	enterht
	announcelabel
	resolvebr
	driver (debug only)

METHOD:

	self explanatory

NOTES:

	none

*FE********************************************************************/

ENTRY *
hsearch(ht, hashfunc, cmp, item, action)

HASHTAB *ht;		/* Hash Table to be searched */
int (*hashfunc)();	/* hash function to be used to determine index */
int (*cmp)();		/* compare function which compares two entries */	
ENTRY item;		/* Item to be inserted or found */
ACTION action;		/* FIND or ENTER */
{
    HNODE *p;		/* Searches through the linked list */
    HNODE **q;		/* Where to store the pointer to a new NODE */
    unsigned int i;	/* Result of hash */
    int res;		/* Result of comparison */

    prcnt = 1;

    i = (*hashfunc)(ht, item.key);	/* Table[i] contains list head */

    if(ht->table[i] == (HNODE*)NULL) { /* List has not yet been begun */
	if(action == FIND)
	    return((ENTRY *) NULL);
	else
	    return(build(&ht->table[i], (HNODE *) NULL, item));
    }
    else {			/* List is not empty */
	q = &ht->table[i];
	p = ht->table[i];
	while(p != NULL && (res = CMPFUNC(item.key, p->item.key))) {
	    prcnt++;
	    q = &(p->next);
	    p = p->next;
	}

	if(p != NULL && res == 0)	/* Item has been found */
	    return(&(p->item));
	else {			/* Item is not yet on list */
	    if(action == FIND)
		return((ENTRY *) NULL);
	    else
#ifdef START
		return(build(&ht->table[i], ht->table[i], item));
#else
		return(build(q, p, item));
#endif
	}
    }
}


/*F*********************** FUNCTION HEADER *******************************

FUNCTION NAME:

	build

ACTION:

	Build an ENTRY in the hash table. ENTRY will be placed between
	"last" and "next"

PARAMETERS:

	Receives:

		last: previous entry in list

		next: next entry in list

	Returns:

		address of new item in list

EXTERNAL DATA REFERENCES:

	none

COMPLEX LOCALS:

	none

FUNCTIONS CALLED:

	malloc

CALLED BY:

	hsearch

METHOD:

	self explanatory

NOTES:

	none

*FE********************************************************************/

static ENTRY
*build(last, next, item)
HNODE **last;		/* Where to store in last list item */
HNODE *next;		/* Link to next list item */
ENTRY item;		/* Item to be kept in node */
{
    HNODE *p = (HNODE *) malloc(sizeof(HNODE));

    if(p != NULL) {
	p->item = item;
	*last = p;
	p->next = next;
	return(&(p->item));
    }
    else
	return(NULL);
}


/* hashd
	Hash function for keys which are character strings.
*/

unsigned int
hashd(ht, key)		/* Division hashing scheme */
HASHTAB *ht;
char *key;		/* Key to be hashed */
{
    return(crunch(key) % ht->length);
}

static unsigned int
crunch(key)		/* Convert multicharacter key to unsigned int */
char *key;
{
    unsigned int sum = 0;	/* Results */
    int s;			/* Length of the key */

    for(s = 0; *key; s++)	/* Simply add up the bytes */
	sum += *key++;

    return(sum + s);
}

#ifdef DRIVER
static void
hdump(ht)			/* Dumps loc, data, probe count, key */
HASHTAB *ht;
{
    unsigned int i;	/* Counts ht->table slots */
    HNODE *a;		/* Current Node on list */

    for(i = 0; i < ht->length; i++)
	if(ht->table[i] == NULL)
	    printf("%o.\t-,\t-,\t(NULL)\n", i);
	else {
	    printf("%o.", i);
	    for(a = ht->table[i]; a != NULL; a = a->next)
		printf("\t%d,\t%#0.4x,\t%s\n",
		    *a->item.data, a, a->item.key);
	}
}
#endif

#ifdef DRIVER
/*F*********************** FUNCTION HEADER *******************************

FUNCTION NAME:

	removekey

ACTION:

	Removes a key from a hash table.

PARAMETERS:

	Receives:

		ht: hash table of concern

		hashfunc: hash function for this hash table

		node: key to be removed

	Returns:

		pointer to node removed

EXTERNAL DATA REFERENCES:

	none

COMPLEX LOCALS:

	none


FUNCTIONS CALLED:

	(*hashfunc)
	free

CALLED BY:

	driver (debug only)

METHOD:

	self explanatory

NOTES:

	none

*FE********************************************************************/

HNODE *
removekey(ht, hashfunc, node)

int (*hashfunc)();
HASHTAB *ht;
HNODE *node;
{
   unsigned int i;
   HNODE *p;
   HNODE **q;

   i = (*hashfunc)(ht, node->item.key);
   p = ht->table[i];
   q = &ht->table[i];
   while ( p && p != node )
      {		/* find node if it exists */
      q = &p->next;
      p = p->next;
      }
   if (p)
      {		/* node exists - relink remaining nodes */
      *q = p->next;
      free(p);
      }
   return (p);
}
#endif

/*F*********************** FUNCTION HEADER *******************************

FUNCTION NAME:

	cleanupht

ACTION:

	Function traverses the specified hash table and returns
	the space allocated for each of its keys.

PARAMETERS:

	Receives:

		ht: hash table of concern

		f: specialized function for each hash table which
		   cleans up associated data structures for each
		   key

	Returns:

		nothing

EXTERNAL DATA REFERENCES:

	none

COMPLEX LOCALS:

	none

FUNCTIONS CALLED:

	(*f)
	free

CALLED BY:

	lcse
	initfunc
	talloc

METHOD:

	self explanatory

NOTES:

	none

*FE********************************************************************/
/* cleanupht
	This function traverses the hash table removing all the keys.
*/

void
cleanupht( ht, f )

HASHTAB *ht;
void (*f)();

{
   int i;
   HNODE *p;
   char *key;

   for (i = 0; i < ht->length ; ++ i )
      {
      for ( p = ht->table[i]; p ; p = p->next )
	 {
	 free(p->item.key);
         if (f)
            (*f)(p->item.data);  /* free up the data link */
	 free(p);
	 }
      ht->table[i] = NULL;
      }
}
