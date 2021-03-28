/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) wordlist.c: version 25.1 created on 12/2/91 at 17:44:25	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)wordlist.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)sgs/comp2/mach:wordlist.c	1.1"

#include <stdio.h>
#include <search.h>

extern int Wflag;
static int allshort;

mkwtable(filename)
char * filename;
{
char buf[256],*ptr,*malloc();
int len;
unsigned int hash;
extern ENTRY *hsearch();
ENTRY item,*retitem;
FILE *fp;
register int numentry=0;
register int errflag=0;
	if(( fp=fopen(filename,"r"))==NULL){
		werror("Can't open word address name file: %s. Assume all names short.\n",filename);
		allshort++;
		return;
	}
	if(!	hcreate(1000)){
		fprintf(stderr,"Cannot create word address hashtable. Assume all long.\n");
		Wflag=0;
		return;
	}
	while(numentry<1000 && fscanf(fp,"%s",buf) != EOF){
		if( (item.key=malloc(strlen(buf)+1))==NULL){
			werror("Out of Short table space. Entry %s treated long.\n",buf);
			errflag++;
			break;
		}
		strcpy(item.key,buf);
		if(!hsearch(item,ENTER)){
			fprintf(stderr,"Error on entry of %s. Treated as long.\n");
			errflag++;
		}else	numentry++;
	}
	if(!numentry && !errflag)
		allshort++;
}

int
findwname(name)
char *name;
{
ENTRY item;
	if(allshort)
		return 1;
	item.key=name;
/*
	fprintf(stderr,"looking for [ %s ],return[ %d ]\n",
		name,(int)hsearch(item,FIND));
*/
	return((int)hsearch(item,FIND));
}
	
