#ident	"@(#)spitcode.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."

#include <stdio.h>
#include "gendefs.h"
extern FILE *fp1;
extern FILE *fp2;
extern char file[];
static short done=0; 	/* we have printed the entire text file */
int myflag=0;
int newcmd=1;
int addbits=0;
int spitnum=0; 		/* the actual number of lines printed */
int spaces=0; 		/* the number of chars printed on this line so far */
int linebits=0;
int cr=0;
int i;
#define MAXSPACE 31
char spitbuf[512];
/* print out a single line of code, given the current pc, and 
 * the line number
 */
spitline(dot,line)
long dot;
long line;
{
/*printf("spitline(dot=[ %d ], line=[ %d ]\n",dot, line);*/
	if(line==(-1))line++; 		/* line to long start another */
	
	/* while the actual number of lines printed 
	 * has not caught up with
	 * the line that needs to be printed print lines
	 */
	while(spitnum < (line-1)){	
		spitnum++;		 
		if(!addbits)		 
			fprintf(fp1,"                                 %6d: ",spitnum);
		else {
			for (i=spaces;i<=MAXSPACE;i++)fputc(' ',fp1);
			fprintf(fp1,"%6d: ",spitnum);
		}

		if(NULL==fgets(spitbuf,510,fp2)){
			fprintf(fp1,"\n");
		}else{
			fprintf(fp1,"%s",spitbuf);
			if(dot>=0) newcmd=1;
			addbits=0;
			linebits=0;
			spaces=0;
			cr=0;
		}
	}
}

/* print out the rest of the file after all the text 
 * has been processed
 */
spitrmd(dot)
long dot;
{
char spitbuf[512];
/*printf("spitrmd(dot=[ %d ]\n",dot);*/
	fprintf(fp1,"\n");
	while(NULL != fgets(spitbuf,510,fp2)){
		spitnum++;
		fprintf(fp1,"                                 %6d: ",spitnum);
		fprintf(fp1,"%s",spitbuf);
	}
	done++; /* signal spitline not to print anymore */
}

/* print out readable object code for this line 
 * given dot= the current pc
 *       nbits= the number of bits to print       
 *      value= the value to be printed
 *      genvalue= no longer used (should be used to print which section(txt/data
 */
spitcode(dot,nbits,value,genval)
unsigned long dot,value ;
unsigned short nbits,genval;
{
unsigned long tmp;
/*printf("spitcode(dot=[ %d ], nbits=[ %d ], value=[ %d ], genval=[ %d ]\n",dot,nbits,value,genval);*/
	if(done)return;
	addbits += nbits;
	linebits += nbits;
	if (addbits>=64){
		spitline(-1,-1);
		fprintf(fp1,"\n%8x: ",dot);
		addbits=nbits;
		spaces=9;
	}
	if(newcmd==1){
		fprintf(fp1,"%8x: ",dot);
		newcmd=0;
		spaces=9;
	};
	switch(nbits){
		case 8: fprintf(fp1,"%02x",0x00ffL&value);
			spaces +=2;
			if(cr){
				fprintf(fp1," ");
				spaces++;
				cr=0;
			}else cr++;
			break;
		case 16:fprintf(fp1,"%04x ",0x0ffffL&value);
			spaces +=5;
			break;
		case 32:tmp= value>>16;
			fprintf(fp1,"%04x ",0x0ffffL&tmp);
			fprintf(fp1,"%04x ",0x0ffffL&value);
			spaces +=10;
			break;
		default:aerror("spitcode: lost");
	}
}
