/*
    1.1	89/06/08 00:41:19 root
	date and time created 89/06/08 00:41:19 by root

*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

%{
/*	ATT:#ident	"cpp:common/cpy.y	1.5"	*/
#ident	"@(#)cpy.y	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
%}
%{
#include "sgs.h"			/* for sgs-global warning info */
%}
%term number stop DEFINED
%term EQ NE LE GE LS RS
%term ANDAND OROR
%left ','
%right '='
%right '?' ':'
%left OROR
%left ANDAND
%left '|' '^'
%left '&'
%binary EQ NE
%binary '<' '>' LE GE
%left LS RS
%left '+' '-'
%left '*' '/' '%'
%right '!' '~' UMINUS
%left '(' '.'
%type <numb> e term
%token <numb> number
%token <op> stop '>' '<' '*' LE GE EQ
%union {				/*bk*/
	struct {
		long val;
		int unsign;
	} numb;
	int op;
}
%%
S:	e stop	={return($1.val);}


e:	  e '*' e
		={$$.val = $1.val * $3.val;}
	| e '/' e
		={
			if ($3.val == 0) {
				ppwarn( W_DFLT, "division by zero");
				$$.val = 0;
			}
			else
				$$.val = $1.val / $3.val;
		}
	| e '%' e
		={
			if ($3.val == 0) {
				ppwarn( W_DFLT, "remainder by zero");
				$$.val = 0;
			}
			else
				$$.val = $1.val % $3.val;
		}
	| e '+' e
		={$$.val = $1.val + $3.val;}
	| e '-' e
		={$$.val = $1.val - $3.val;}
	| e LS e
		={$$.val = $1.val << $3.val;}
	| e RS e
		={$$.val = $1.val >> $3.val;}
	| e '<' e
		{
			if ( $1.unsign || $3.unsign )
				$$.val = (unsigned) $1.val < (unsigned) $3.val;
			else
				$$.val = $1.val < $3.val;
		}
	| e '>' e
		{
			if ( $1.unsign || $3.unsign )
				$$.val = (unsigned) $1.val > (unsigned) $3.val;
			else
				$$.val = $1.val > $3.val;
		}
	| e LE e
		{
			if ( $1.unsign || $3.unsign )
				$$.val = (unsigned) $1.val <= (unsigned) $3.val;
			else
				$$.val = $1.val <= $3.val;
		}
	| e GE e
		={$$.val = $1.val >= $3.val;}
	| e EQ e
		={$$.val = $1.val == $3.val;}
	| e NE e
		={$$.val = $1.val != $3.val;}
	| e '&' e
		={$$.val = $1.val & $3.val;}
	| e '^' e
		={$$.val = $1.val ^ $3.val;}
	| e '|' e
		={$$.val = $1.val | $3.val;}
	| e ANDAND e
		={$$.val = $1.val && $3.val;}
	| e OROR e
		={$$.val = $1.val || $3.val;}
	| e '?' e ':' e
		={$$.val = $1.val ? $3.val : $5.val;}
	| e ',' e
		={$$.val = $3.val;}
	| term
		={$$ = $1;}
term:
	  '-' term %prec UMINUS
		={$$.val = -$2.val;}
	| '!' term
		={$$.val = !$2.val;}
	| '~' term
		={$$.val = ~$2.val;}
	| '(' e ')'
		={$$.val = $2.val;}
	| DEFINED '(' number ')'
		={$$.val= $3.val;}
	| DEFINED number
		={$$.val = $2.val;}
	| number
		={$$ = $1; }
%%
