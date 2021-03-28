#ident	"@(#)hash.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	@(#)hash.h	7.1		*/
/*	Copyright 1985 Motorola Inc.			*/

#ifndef HASH_H
#define HASH_H

typedef struct entry { char *key, *data; } ENTRY;
typedef enum { FIND, ENTER } ACTION;

typedef struct hnode 
	{ 			/* node in hash bucket */
	ENTRY item;		/* hash key */
	struct hnode *next;	/* pointer to next sibling in bucket */
	} HNODE;

typedef struct hashtab
	{			/* hash table */
	HNODE **table;		/* address of hash table */
	int length;		/* length of the hash table */
	} HASHTAB;

#endif	/* HASH_H */
