/*
    1.4	89/08/14 15:22:56 hossein
	corrected the fix on void functions. see README.ses.

    1.3	89/08/02 09:39:52 hossein
	fix bug in void functions appearing as expressions.

    1.2	89/06/26 15:24:48 hossein
	changes related to "const" type qualifier for POSIX.

    1.1	89/06/08 00:38:19 root
	date and time created 89/06/08 00:38:19 by root

*/
%{
static	char	SCCSID[] = "@(#) cgram.y:	2.1 83/07/08";
#ident	"@(#)cgram.y	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
%}
%term NAME  2
%term STRING  3
%term ICON  4
%term FCON  5
%term PLUS   6
%term PLUSASG   7
%term MINUS   9
%term MINUSASG   10
%term MUL   12
%term MULASG   13
%term AND   15
%term ANDASG   16
%term OR   18
%term ORASG   19
%term ER   20
%term ERASG   21
%term QUEST  22
%term COLON  23
%term ANDAND  24
%term OROR  25

/*	special interfaces for yacc alone */
/*	These serve as abbreviations of 2 or more ops:
	ASOP	=, = ops
	RELOP	LE,LT,GE,GT
	EQUOP	EQ,NE
	DIVOP	DIV,MOD
	SHIFTOP	LS,RS
	ICOP	ICR,DECR
	UNOP	NOT,COMPL
	STROP	DOT,STREF

	*/
%term ASOP  26
%term RELOP  27
%term EQUOP  28
%term DIVOP  29
%term DIVOPASG  30
%term SHIFTOP  31
%term SHIFTOPASG  32
%term INCOP  33
%term UNOP  34
%term STROP  35

/*	reserved words, etc */
%term TYPE  36
%term TYPEQ  37
%term CLASS  38
%term STRUCT  39
%term RETURN 40
%term GOTO  41
%term IF  42
%term ELSE  43
%term SWITCH  44
%term BREAK  45
%term CONTINUE  46
%term WHILE  47
%term DO  48
%term FOR  49
%term DEFAULT  50
%term CASE  51
%term SIZEOF  52
%term ENUM 53

/*	little symbols, etc. */
/*	namely,

	LP	(
	RP	)

	LC	{
	RC	}

	LB	[
	RB	]

	CM	,
	SM	;

	*/

%term LP  54
%term RP  55
%term LC  56
%term RC  57
%term LB  58
%term RB  59
%term CM  60
%term SM  61
%term ASSIGN  62
%term ELLIPSIS 63
%term ASM 64
%term ARGTYPE 65

/* at last count, there were 7 shift/reduce, 1 reduce/reduce conflicts
/* these involved:
	if/else
	recognizing functions in various contexts, including declarations
	error recovery
	*/

%left CM
%right ASOP ASSIGN PLUSASG MINUSASG MULASG ANDASG ORASG ERASG DIVOPASG SHIFTOPASG 
%right QUEST COLON
%left OROR
%left ANDAND
%left OR
%left ER
%left AND
%left EQUOP
%left RELOP
%left SHIFTOP
%left PLUS MINUS
%left MUL DIVOP
%right UNOP
%right INCOP SIZEOF ARGTYPE
%left LB LP STROP
%{
# include "mfile1.h"
# include <limits.h>				/* bk: for INT_MAX */
# include "modes.h"
unsigned short int leftcons = 1;
static int optid = -1;
int ftn_call;			/* bk */
int ftnsave;
int argspace;
int noilist;
int tentative_flag;		/* to help manage ANSI rules, 3.7.2 */
int xnfdecl;			/* helps prevents printing sdb info prematurely */
/*extern int pstk_indx;	/ helps disentagle cast from prototype grammar /
int save_stkx;			/ (same) */
int maybe_cast;         /* flag to mark possible start 
of a cast type, used by rstruct() */
int saveclass;			/* remember curclass before declspecs */
						/* in parmtype productions clobber it. */
						/* This doubles as an inside-proto flag. */
int incit;				/* related to "null_decl-> NAME LP opt_plist RP" */
extern int seen_typespec;
extern int enum_chk;
extern int vaflag;		/* LINT VARARGS comment from scanner */
/******* declaration flags *********/
int newstyle;
int tqshift;
NODE *str_type, *type_def, *arg_type;
#define DECL_RESET	typequal = curclass = indcl = seen_typespec = \
					maybe_cast = tqshift = struc_qual[sulvl] = 0
#define DECL_RESET2	typequal = curclass = seen_typespec = tqshift = 0;
#define DECL_RESET3	typequal = seen_typespec = tqshift = 0;
#define DECL_RESET4	seen_typespec = 0;
/***********************************/
char chktype[SIGNED];
%}

	/* define types */
%start ext_def_list

%type <intval> con_e ifelprefix ifprefix whprefix forprefix doprefix switchpart
		enum_head str_head name_lp optellipse optname bldspecs type obldspecs
		typequal_list specqual ospecqual 
%type <nodep> e .e term declspecs odeclspecs structspecs enum_dcl struct_dcl
		cast_type  null_decl funct_idn declarator fdeclarator
		nfdeclarator elist /* type_spec parm_decl */ 
		parmtype_list opt_parmtype_list parmtypes fdef_pref 
		fdef_newpref new_style argtype

%token <intval> CLASS NAME STRUCT RELOP CM DIVOP PLUS MINUS SHIFTOP MUL AND OR
		ER ANDAND OROR ASSIGN STROP INCOP UNOP ICON FCON ELLIPSIS PLUSASG 
		MINUSASG MULASG ANDASG ORASG ERASG DIVOPASG SHIFTOPASG TYPEQ SIZEOF
%token <nodep> TYPE ARGTYPE

%%

%{
	extern NODE *ProtoDeclare(), *ProtoId(), *do_argty();
	int sulvl=0; /* counts depth of nesting of structure and
			     ** union declarations.  Used to index into
			     ** "is_struc". */
	TWORD struc_qual[16];	/* remember structure type qualifiers */
	static char is_struc[16]; /* nonzero if current STRUCT is 'struct'
				 ** and not 'union'.  Allows 15 deep nesting */
	static int fake = 0;
	static char fakename[NCHNAM+1];
%}

ext_def_list:	   ext_def_list external_def
			{
				extern int null_input;
				null_input = 0;
			}
		|	/* empty */
			{
#ifndef LINT
				bg_file();
#endif
				ftnend();
			}
		;

external_def:	   data_def
			{ blevel = 0; DECL_RESET; vaflag = 0; argno = 0;}
		|  error
			{ blevel = 0; DECL_RESET; vaflag = 0; argno = 0;}
		|  ASM SM
			{ asmout();  blevel = 0; DECL_RESET; argno = 0;} 
		;
data_def:	   declspecs  SM		/* bk: all new declaration grammar */
			{  
				$1->in.op = FREE; DECL_RESET; 
			}
		|	odeclspecs SM
			{  
				$1->in.op = FREE; DECL_RESET; 
				if ( indcl != -1 )
					werror( W_STRICT, 
					"empty external declaration" );
			}
		|  declspecs init_dcl_list  SM
			{  
				$1->in.op = FREE; DECL_RESET; 
			}
		|  odeclspecs init_dcl_list  SM
			{  
				$1->in.op = FREE; DECL_RESET; 
				if ( indcl != -1 )
					werror( W_STRICT, 
					"non-function external must have specifiers" );
			}
		|	fdef_pref function_body
			{  
			    if( blevel ) cerror( "function level error" );
			    if( reached ) retstat |= NRETVAL; 
			    $1->in.op = FREE;
			    ftnend();
		    }
		|	fdef_newpref compoundstmt		/* no arg_dcl_list */
			{	
			newdef:
				regvar = typequal = 0;   /* clear out arguments */
			    if( blevel ) cerror( "function level error" );
			    if( reached ) retstat |= NRETVAL; 
			    $1->in.op = FREE;
			    ftnend();
		    }
		;

fdef_newpref: odeclspecs new_style
					{ 
						typequal = 0; seen_typespec = 0; 
						newstyle = 1;
						goto fcode; 
					}
			|  declspecs new_style
					{ 
						typequal = 0; seen_typespec = 0; 
						newstyle = 1;
						goto fcode; 
					}
		;

fdef_pref:	odeclspecs fdeclarator
			{		goto fcode; }
		|	declspecs fdeclarator
			{ 
			fcode:
				indcl = seen_typespec = 0; 
#ifdef IN_LINE
	/* got a function, is it an asm inline function? */
	/* squirelmacro will purge stdin up to closing '}', then it'll */
	/* try to get the parser to se '}' by unget('}') to believe */
	/* the "function body" part (it's already seen the '{') */

			    if (curclass == INLINE)
			    {
				defid(tymerge($1, $2), INLINE);
				squirrelmacro(stab[$2->tn.rval].sname);
			    }    
			    else
#endif
				{
					ProtoDefine($2);		/* got identifiers for all types? */
					switch ( curclass ) 		/* bk */
					{ 
						case SNULL:
						case EXTERN:
						case STATIC:
						case FORTRAN:
							break;
						default:
							uerror( "function has illegal storage class" );
					}
					defid( tymerge($1,$2),
						curclass==STATIC?STATIC:EXTDEF );
					if ( newstyle ) {
						blevel = 1;
						newstyle = 0;
						curclass = SNULL;
					}
				}
			}
		;

function_body:	 arg_dcl_list compoundstmt
		{	regvar = 0; typequal = 0; }  /* clear out arguments */
		;

arg_dcl_list:	   arg_dcl_list declspecs declarator_list SM
			{ $2->in.op = FREE; DECL_RESET; };
		|  arg_dcl_list declspecs SM
			/* to permit structs in decl. lists */
			{ $2->in.op = FREE; DECL_RESET; };
		| 	/* EMPTY */  {  blevel = 1; DECL_RESET; }
		;

stmt_list:	   stmt_list statement
			{ constid = 0; leftcons = 1;}
		|  /* EMPTY */
			{
			   if (curclass != INLINE)
			   {  bccode();
			       locctr(PROG);
			   }
			}
		;

dcl_stat_list:	dcl_stat_list declspecs SM
			{  
				int t = BTYPE($2->in.type); 
				if ( t != ENUMTY && t != STRTY && t != UNIONTY )
					uerror( "missing declarator" );
				$2->in.op = FREE;  DECL_RESET;
			}
		|	dcl_stat_list declspecs init_dcl_list SM
			{  $2->in.op = FREE;  DECL_RESET; }
		|  /* EMPTY */
		;

odeclspecs: /* empty */
			{  $$ = mkty(INT,0,INT,0);  
				curclass = SNULL; }
		;

declspecs:	decl_init bldspecs
			{	
				if ( $2 != -1 ) {		/* -1 means struct_dcl or type_def */
					int bldtype = types();			/* synthesize chktype */	
					yyval.nodep = mkty( (TWORD) bldtype, 0, bldtype, typequal );
				} else {
					if ( type_def ) {
						$$ = type_def;
						type_def = 0;
						if ( str_type ) {
							str_type->in.op = FREE;
							str_type = 0;
						}
					} else {
						if ( arg_type ) {
							$$ = arg_type;
							arg_type = 0;
						} else {
							$$ = str_type;
							str_type = 0;
						}
					}
				}
			}
		;

structspecs:	decl_init specqual
			{	
				if ( $2 != -1 ) {		/* -1 means struct_dcl or type_def */
					int bldtype = types();			/* synthesize chktype */	
					yyval.nodep = mkty( (TWORD) bldtype, 0, bldtype, 0 );
				} else {
					if ( type_def ) {
						$$ = type_def;
						type_def = 0;
						if ( str_type ) {
							str_type->in.op = FREE;
							str_type = 0;
						}
					} else {
						if ( arg_type ) {
							$$ = arg_type;
							arg_type = 0;
						} else {
							$$ = str_type;
						}
					}
				}
			}
		;

decl_init:	/* empty */
		{ indcl = -1; }
		;

bldspecs:	class obldspecs
			{ $$ = ( $2 == -1 ) ? -1 : 0 ; }
		|	type obldspecs
			{ 
				if ( $2 == -1 ) {		/* e.g. short struct foo { ... }; */
					if ( str_type )
						uerror( "illegal type specifiers" );
				}
			}
		|	typequal_list obldspecs
			{ $$ = ( $2 == -1 ) ? -1 : 0 ; }
		;

obldspecs:	/* empty */
			{ $$ = 0; }
		| 	bldspecs
		;

specqual:	type ospecqual
		|	typequal_list ospecqual
			{ $$ = ( $2 == -1 ) ? -1 : 0 ; }
		;

ospecqual:	/* empty */
			{ $$ = 0; }
		| 	specqual
		;


class:		  CLASS
			{  
				if ( curclass != 0 )
					uerror( "extra storage class specifier" );
				curclass = $1;
			}
		;

otypequal_list:	/* EMPTY */
		|	tq_shift typequal_list
		;

tq_shift:		/* empty */
		{ ++tqshift; }
		;

typequal_list:	TYPEQ
			{ typequal = type_quals( typequal, $1, tqshift ); }
		|	TYPEQ typequal_list 
			{	typequal = type_quals( typequal, $1, tqshift ); }
		;

type:	   TYPE
			{ 
					if ( !type_def ) {
						if (chktype[ $1->in.type - 1 ] )
							uerror( "illegal type combination" );
						chktype[ $1->in.type - 1 ] = 1;
						$1->in.op = FREE;
						$$=0;
					}
					else {
					 if ( $1 != type_def ) /* e.g.:  "TYPEDEF_NAME int x;" */
						uerror( "illegal combination of type and typedef" );
					 if ( decomp_type($1) )
					   $$ = -1;
					 else {
					   $$ = 0;
					   $1->in.op = FREE;
					   type_def = 0 ; 
					 }
					}
			}
		|  struct_dcl
			{
			str_chk:
				$$ = -1;
				str_type = $1;
			}
		|  enum_dcl
			{ goto str_chk; }
		|  argtype
		   { 
				arg_type = $1;
				$$ = -1; 
		   }
		;

argtype:    ARGTYPE LP cast_type RP  %prec ARGTYPE
			{	
				if (optid>0) { 
					uerror( "declarator not allowed in __argtype" ); 
					optid = -1; 
				}
				$$ = do_argty( $3 );
				ProtoMirage();
			}
		|   ARGTYPE term
			{  $$ = do_argty( $2 ); }
		;

pointer:	MUL otypequal_list
		|	MUL otypequal_list pointer
		;

enum_dcl:	   enum_head LC moe_list optcomma RC
			{ $$ = dclstruct($1); }
		|  ENUM NAME
			{  $$ = rstruct($2,0);  stwart = instruct; }
		;

enum_head:	   ENUM
			{  $$ = bstruct(-1,0); stwart = SEENAME; }
		|  ENUM NAME
			{  $$ = bstruct($2,0); stwart = SEENAME; }
		;

moe_list:	   moe
		|  moe_list CM moe
		;

moe:		   NAME
			{  moedef( $1 ); }
		|  NAME ASSIGN con_e
			{	strucoff = $3;  enum_chk = 0; moedef( $1 ); }
		;

struct_dcl:	   str_head LC type_dcl_list optsemi RC
			{ $$ = dclstruct($1);  
			  if (is_struc[sulvl]) strucdef--; /*finished struc def
						    ** ... may be nested. */
			  sulvl--;
			  typequal = 0;
		        }
		|  STRUCT NAME
			{  $$ = rstruct($2,$1); }
		;

optsemi		:	/* empty */
		{ werror( W_STRICT, "missing semi-colon in structure member list" ); }
		|  SM
		;

str_head:	   STRUCT
			{  
				struc_qual[sulvl] = typequal;
				typequal = 0;
			   sulvl++;
			   is_struc[sulvl] = ($<intval>1 == INSTRUCT);
			   if (is_struc[sulvl])
			      /* struct def, not union def */
			      strucdef++;
			   $$ = bstruct(-1,$1);  stwart=0; }
		|  STRUCT NAME
			{  
				struc_qual[sulvl] = typequal;
				typequal = 0;
			   sulvl++;
			   is_struc[sulvl] = ($<intval>1 == INSTRUCT);
			   if (is_struc[sulvl])
			      /* struct def, not union def */
			      strucdef++;
			   $$ = bstruct($2,$1);  stwart=0;  }
		;

type_dcl_list:	   type_declaration
		|  type_dcl_list SM type_declaration
		;

type_declaration:  structspecs declarator_list
			{ 
				stwart=0; $1->in.op = FREE; 
				DECL_RESET2;
			}
		|  structspecs
			{	
				seen_typespec = 0;
				if( curclass != MOU ) {
					curclass = SNULL;
				} else {
					sprintf( fakename, "$%dFAKE", fake++ );
					defid( tymerge($1, bdty(NAME,NIL,
						lookup( fakename, SMOS ))), curclass );
					werror( W_DFLT, "union member must be named" );
				}
			    stwart = 0;
			    $1->in.op = FREE;
				DECL_RESET2;
		    }
		;


declarator_list:   declarator
			{ defid( tymerge($<nodep>0,$1), curclass);
				stwart = instruct; }
		|  declarator_list  CM {$<nodep>$=$<nodep>0;}  declarator
			{ defid( tymerge($<nodep>0,$4), curclass);
				stwart = instruct; }
		;
declarator:	   nfdeclarator
		|  nfdeclarator COLON con_e
			%prec CM
			{  if( !(instruct & (INSTRUCT|INUNION)) )	/*bk*/
				uerror( "field outside of structure" );
			    if( $3<0 || $3 >= FIELD ){
				uerror( "illegal field size" );
				$3 = 1;
				}
			    defid( tymerge($<nodep>0,$1), FIELD|$3 );
			    $$ = NIL;
			    }
		|  COLON con_e
			%prec CM
			{   if( !(instruct & (INSTRUCT|INUNION)) )
				uerror( "field outside of structure" );
			    /* alignment or hole */
			    falloc( stab, $2, -1, $<nodep>0 );
			    $$ = NIL;
			    }
		|  error
			{  $$ = NIL; }
		;

		/* int (a)();   is not a function --- sorry! */
nfdeclarator:	   pointer nfdeclarator		
			{  umul:
				$$ = bdty( UNARY MUL, $2, 0 ); }
		|  nfdeclarator  LP   RP		
			{  uftn:
				$$ = bdty( UNARY CALL, $1, 0 ); }
		|  nfdeclarator  LP parmtype_list  RP
			{ 
				$$ = bdty( UNARY CALL, $1, 0 );
				stwart = 0;
				ProtoLink( $1, 0 );		 /* tie protolist to ftn-ptr id */
				ProtoDeclare($1);		/* just an id, not a function def'n */
				seen_typespec = 0;
			}
		|  nfdeclarator LB RB		
			{  
				seen_typespec = 0;
			uary:
				$$ = bdty( LB, $1, 0 ); }
		|  nfdeclarator LB con_e RB	
			{  
				seen_typespec = 0;
			bary:
				if( (int)$3 <= 0 )
					werror( W_DFLT, "zero or negative subscript" );
				$$ = bdty( LB, $1, $3 ); 
				}
		|  NAME  		
			{  seen_typespec = 0; $$ = bdty( NAME, NIL, $1 ); }
		|   LP  nfdeclarator  RP 		
			{ $$=$2; seen_typespec = 0; }
		;

new_style:	fdeclarator  LP parmtype_list  RP
			{		/* e.g. "int (*foo(int x))(char *y)" */
				$$ = bdty( UNARY CALL, $1, 0 );
				stwart = 0;
				ProtoPush();	/* we don't know name of pointed-to ftn */
				ProtoLink($1,1);
			}
		|  name_lp  parmtype_list  RP
			{
				NODE *idn;
				argspace = 0;
				$$ = bdty( UNARY CALL, bdty(NAME,NIL,$1), 0 );
				stwart = 0;
				ProtoLink(idn=bdty(NAME,NIL,$1),0); /*link ptypes to ftn id*/
				idn->in.op = FREE;
				}
		|	pointer new_style
			{  goto umul; }
		|  new_style  LP opt_parmtype_list  RP
			{  
				$$ = bdty( UNARY CALL, $1, 0 );
				stwart = 0;
				if ( $3 ) {
					ProtoPush();
					ProtoLink($1,1);
				}
			} 
		|  new_style LB RB
			{  goto uary; }
		|  new_style LB con_e RB
			{  goto bary; }
		|   LP  new_style  RP
			{ $$ = $2; }
		;

fdeclarator:	   pointer fdeclarator
			{  goto umul; }
		|  fdeclarator  LP   RP
			{  goto uftn; }
		|  fdeclarator LB RB
			{  goto uary; }
		|  fdeclarator LB con_e RB
			{  goto bary; }
		|   LP  fdeclarator  RP
			{ $$ = $2; }
		|  name_lp  name_list  RP
			{
				argspace = 0;
				if( blevel!=0 )
					uerror(
					"function declaration in bad context");
				$$ = bdty( UNARY CALL, bdty(NAME,NIL,$1), 0 );
				stwart = 0;
				}
		|  name_lp RP
			{
				argspace = 0;
				$$ = bdty( UNARY CALL, bdty(NAME,NIL,$1), 0 );
				stwart = 0;
				}
		;

optellipse:		CM ELLIPSIS			/* "int foo(...)" is illegit */
		| 	/* empty */
			{ $$ = 0; }			/* must return a value, since LHS has a type */
		;

name_lp:	  NAME LP
			{
				/* turn off typedefs for argument names */
/*				stwart = SEENAME; / bk: to allow typedef prototype */
				if( stab[$1].sclass == SNULL )
				    stab[$1].stype = FTN;
				argspace = 1;			/*bk*/
				seen_typespec = 0;
				}
		;

parmtype_list:	proto_init parmtypes optellipse
			{		
				ProtoEnd($3);			/* mark end of prototype list */
				tfree($2);				/* done with proto list */
				--blevel;				/* back down from protolist scope */
				$$ = $2;
			}
		;

parmtypes:		cast_type
			{ 
				if ( curclass && curclass != REGISTER )
					uerror( 
				"\"register\" is only valid storage class in a parameter list");
				if (optid >= 0) optid = ftnarg( optid ); 
				ProtoType($1,optid);
				curclass = saveclass;
				DECL_RESET3;
				optid = -1;
			}
		|	parmtypes CM cast_type
			{ 
				if ( curclass && curclass != REGISTER )
					uerror( 
				"\"register\" is only valid storage class in a parameter list");
				if (optid >= 0) optid = ftnarg( optid ); 
				ProtoType($3,optid);
				curclass = saveclass;
				DECL_RESET3;
				optid = -1;
				goto bop;
			}
		;

proto_init:		/* empty */	
			{ ++blevel; }
		;

name_list:	   NAME			
			{ ftnarg( $1 );  stwart = SEENAME; }
		|  name_list  CM  NAME 
			{ ftnarg( $3 );  stwart = SEENAME; }
		| error
		;
		/* always preceeded by declspecs: thus the $<nodep>0's */
init_dcl_list:	   init_declarator
			%prec CM
		|  init_dcl_list  CM {$<nodep>$=$<nodep>0;}  init_declarator
		;
		/* always preceeded by declspecs */
xnfdeclarator:	   nfdeclarator
			{ 	/* bk: was previous decl tentative? (ANSI 3.7.2) */
				$1 = tymerge($<nodep>0,$1);
				tentative_flag = 1;	/* be on guard for tentative def */
				xnfdecl = 1;	/* some type info may come via initializer */
				defid( $1, curclass );
			    beginit($1->tn.rval);
				xnfdecl = 0;
				tentative_flag = 0;
				DECL_RESET4;
			    }
		|  error
		;
		/* always preceeded by declspecs */
init_declarator:   nfdeclarator
			{  
				struct symtab *p;

				$1 = tymerge( $<nodep>0,$1 ); 
				tentative_flag = 1;	/* be on guard for tentative def */
				nidcl( $1 );

				p = &stab[$1->tn.rval];
				if ( ( CONST_QUALIFIED( $1->tn.type, p->stypeq ) ) &&
						p->sclass != TYPEDEF )
					werror( W_DFLT, "unitialized const-qualified object" );
				tentative_flag = 0;
			}
		|  new_style
			{ defid( 
				ProtoDeclare(tymerge($<nodep>0,$1)), 
							uclass(curclass) );
			}
		|  fdeclarator
			{ ProtoMix($1); defid( tymerge($<nodep>0,$1), uclass(curclass) ); } 
		|  xnfdeclarator ASSIGN e
			%prec CM
			{  
				/*bk:  struct { ... } x = expr; */
				/* char p[] = "hi", e above == NIL */
				if ( $3 && $3->in.type == STRTY ) 
					noilist = 1;

					doinit( $3 );
					endinit();
					noilist = 0;			/*bk*/
			}
		|  xnfdeclarator ASSIGN LC init_list optcomma RC
			{  endinit(); }
		| error
		;

init_list:	   initializer
			%prec CM
		|  init_list  CM  initializer
		;
initializer:	   e
			%prec CM
			{  doinit( $1 ); }
		|  ibrace init_list optcomma RC
			{  irbrace(); }
		;

optcomma	:	/* empty */
		|  CM
		;

ibrace		: LC
			{  ilbrace(); }
		;

/*	STATEMENTS	*/

compoundstmt:	   begin dcl_stat_list stmt_list RC
			{
			    clearst(blevel);
			    if (--blevel == 1)
			    {
				clearst(blevel);
				blevel = 0;
			    }
			    checkst(blevel);
			    autooff = *--psavbc;
			    regvar = *--psavbc;
#ifdef MC68K
			    eccode(blevel);	/* called at end of block */
#endif
			}
		;

begin:		  LC
			{  if( blevel == 1 ) dclargs();
			    ++blevel;
			    if( psavbc > &asavbc[BCSZ-2] )
					cerror( "nesting too deep" );
			    *psavbc++ = regvar;
			    *psavbc++ = autooff;
			    }
		;

statement:	   e SM
			{ ecomp( $1 ); }
		|  ASM SM
			{ asmout(); }
		|  compoundstmt
		|  ifprefix statement
			{  
				deflab($1);
			   	reached = 1;
			   }
		|  ifelprefix statement
			{ 	
				if( $1 != NOLAB ){
				deflab( $1 );
				reached = 1;
				}
			    }
		|  whprefix statement
			{   
		            branch(  contlab );
			    deflab( brklab );
			    if( (flostat&FBRK) || !(flostat&FLOOP)) reached = 1;
			    else reached = 0;
			    resetbc(0);
			    }
		|  doprefix statement WHILE  LP  e  RP   SM
			{ 	
			    deflab( contlab );
			    if( flostat & FCONT ) reached = 1;
			    ecomp( buildtree( CBRANCH,
			    buildtree( NOT, $5, NIL ), bcon( $1 ) ) );
			    deflab( brklab );
			    reached = 1;
			    resetbc(0);
			    }
		|  forprefix .e RP statement
			{   
			    deflab( contlab );
			    if( flostat&FCONT ) reached = 1;
			    if( $2 ) ecomp( $2 );
			    branch( $1 );
			    deflab( brklab );
			    if( (flostat&FBRK) || !(flostat&FLOOP) ) reached=1;
			    else reached=0;
			    resetbc(0);
			    }
		| switchpart statement
			{   
			    if( reached ) branch( brklab );
			    deflab( $1 );
			    swend();
			    deflab(brklab);
			    if( (flostat&FBRK) || !(flostat&FDEF) ) reached=1;
			    resetbc(FCONT);
			    }
		|  BREAK  SM
			{  if( brklab == NOLAB ) uerror( "illegal break");
			    else if(reached) branch( brklab );
			    flostat |= FBRK;
			    if( brkflag ) goto rch;
			    reached = 0;
			    }
		|  CONTINUE  SM
			{   if( contlab == NOLAB ) uerror( "illegal continue");
			    else branch( contlab );
			    flostat |= FCONT;
			    goto rch;
			    }
		|  RETURN  SM
			{   retstat |= NRETVAL;
			    branch( retlab );
			rch:
			    if( !reached ) werror( W_LINT, "statement not reached" );
			    reached = 0;
			    }
		|  RETURN e  SM
			{   register NODE *temp;
			    TWORD indtype();
			    idname = curftn;
			    temp = buildtree( NAME, NIL, NIL );
			    if(temp->in.type == (FTN|VOID))
				uerror(
				"void function %s cannot return value",
					stab[idname].sname);
			    temp->in.type = DECREF( temp->in.type );
			    temp->in.type = indtype( temp->in.type );
			    temp->tn.op = RNODE;  /* node for return */
			    temp = buildtree( ASSIGN, temp, $2 );
			    ecomp( temp );
			    retstat |= RETVAL;
			    branch( retlab );
			    reached = 0;
			    }
		|  GOTO NAME SM
			{  register NODE *q;
			    q = block( FREE, NIL, NIL, INT|ARY, 0, INT, 0 );
			    q->tn.rval = idname = $2;
			    defid( q, ULABEL );
			    stab[idname].suse = -lineno;
			    branch( stab[idname].offset );
			    goto rch;
			    }
		|   SM
		|  error  SM
		|  error RC
		|  label statement
		;
label:		   NAME COLON
			{  register NODE *q;
			    q = block( FREE, NIL, NIL, INT|ARY, 0, LABEL, 0 );
			    q->tn.rval = $1;
			    defid( q, LABEL );
			    reached = 1;
			    }
		|  CASE e COLON
			{  addcase($2);
			    reached = 1;
			    }
		|  DEFAULT COLON
			{  reached = 1;
			    adddef();
			    flostat |= FDEF;
			    }
		;
doprefix:	DO
			{  savebc();
			    if( !reached ) werror( W_LINT, "loop not entered at top");
			    brklab = getlab();
			    contlab = getlab();
			    deflab( $$ = getlab() );
			    reached = 1;
			    }
		;
ifprefix:	IF LP e RP
			{
			 ecomp( buildtree( CBRANCH, $3, bcon( $$=getlab()) ) ) ;
			    reached = 1;
			    }
		;
ifelprefix:	  ifprefix statement ELSE
			{
			 	if( reached ) branch( $$ = getlab() );
			    else $$ = NOLAB;
			    deflab( $1 );
			    reached = 1;
			    }
		;

whprefix:	  WHILE  LP  e  RP
			{
			 	savebc();
			    if( !reached ) werror( W_LINT, "loop not entered at top");
			    if( $3->in.op == ICON && $3->tn.lval != 0 ) flostat = FLOOP;
			    deflab( contlab = getlab() );
			    reached = 1;
			    brklab = getlab();
			    if( flostat == FLOOP ) tfree( $3 );
			    else ecomp( buildtree( CBRANCH, $3, bcon( brklab) ) );
			    }
		;
forprefix:	  FOR  LP  .e  SM .e  SM 
			{
			 	if( $3 ) ecomp( $3 );
			    else if( !reached ) werror( W_LINT, "loop not entered at top");
			    savebc();
			    contlab = getlab();
			    brklab = getlab();
			    deflab( $$ = getlab() );
			    reached = 1;
			    if( $5 )
				ecomp( buildtree( CBRANCH, $5, bcon( brklab) ));
			    else flostat |= FLOOP;
			    }
		;
switchpart:	   SWITCH  LP  e  RP
			{
			   register NODE *temp;
			    savebc();
			    temp = block( SNODE, NIL, NIL, INT, 0, INT, 0 );
			    temp = buildtree( ASSIGN, temp, $3 );
			    brklab = getlab();
			    ecomp( temp );
			    branch( $$ = getlab() );
			    swstart();
			    reached = 0;
			    }
		;
/*	EXPRESSIONS	*/
con_e:		   { $<intval>$=instruct; stwart=instruct=0; } e
			%prec CM
			{	
				icon_overflow( $2, 1 );		/* bk */
				$$ = icons( $2 );  
				instruct=$<intval>1; 
			}
		;
.e:		   e
		|
			{ $$=0; }
		;
elist:		   e
			%prec CM
		|  elist  CM  e
			{  goto bop; }
		;

e:		   e RELOP e
			{
			preconf:
			    if( yychar==RELOP || yychar==EQUOP || yychar==AND
					|| yychar==OR || yychar==ER ){
			precplaint:
				werror( W_LINT,
				"precedence confusion possible: parenthesize!"
					);
				}
			bop:
			    $$ = buildtree( $2, $1, $3 );
			    }
		|  e CM e
			{  $2 = COMOP;
			    goto bop;
			    }
		|  e DIVOP e
			{  goto bop; }
		|  e PLUS e
			{  if(yychar==SHIFTOP) goto precplaint; else goto bop; }
		|  e MINUS e
			{  if(yychar==SHIFTOP ) goto precplaint; else goto bop; }
		|  e SHIFTOP e
			{  if(yychar==PLUS||yychar==MINUS) goto precplaint; else goto bop; }
		|  e MUL e
			{  goto bop; }
		|  e EQUOP  e
			{  goto preconf; }
		|  e AND e
			{  if( yychar==RELOP||yychar==EQUOP ) goto preconf;  else goto bop; }
		|  e OR e
			{  if(yychar==RELOP||yychar==EQUOP) goto preconf; else goto bop; }
		|  e ER e
			{  if(yychar==RELOP||yychar==EQUOP) goto preconf; else goto bop; }
		|  e ANDAND e
			{  goto bop; }
		|  e OROR e
			{  goto bop; }
		|  e MULASG e
			{  abop:
				$$ = buildtree( $2, $1, $3 );
				}
		|  e DIVOPASG e
			{   $$ = buildtree( ASG $2, $1, $3 ); }
		|  e PLUSASG e
			{  goto abop; }
		|  e MINUSASG e
			{  goto abop; }
		|  e SHIFTOPASG e
			{  $$ = buildtree( ASG $2, $1, $3 ); }
		|  e ANDASG e
			{  goto abop; }
		|  e ORASG e
			{  goto abop; }
		|  e ERASG e
			{  goto abop; }
		|  e QUEST e COLON e
			{  $$=buildtree(QUEST, $1, buildtree( COLON, $3, $5 ) );
			    }
		|  e ASOP e			/* bk: let's remove this!! */
			{  werror( W_LINT, "old-fashioned assignment operator" );
				goto bop; }
		|  e ASSIGN e
			{	goto bop; }
		|  term
		;

term:		   term INCOP
			{  $$ = buildtree( $2, $1, bcon(1) ); }
		|  MUL term
			{ ubop:
			    if( constid && leftcons ) 
			    {
			    	$$ = buildtree( UNARY $1, $2, NIL );
				leftcons = 0; 
			    }
			    else
			    	$$ = buildtree( UNARY $1, $2, NIL );
			}
		|  AND term
			{  
				if ( modes[ MODE_ANSIKEYW ].lxmval )
				{
					if ( str_expr( $2 ) ) {
						$$ = $2;
					}
					else {
						goto ubop;
					}
				}
				else
					if ( ISFTN($2->in.type) || 
					   ISARY($2->in.type) ) {
						$$ = $2;
					}
					else
						if ( str_expr ( $2 ) ) {
							$$ = $2;
						}
						else
							goto ubop;
			}
		|  MINUS term
			{  goto ubop; }
		|  PLUS term					/* bk */
			{
					/*would like to do zip, but 
					may need default integral promotions*/
				goto ubop; 
			}
		|  UNOP term
			{
			    $$ = buildtree( $1, $2, NIL );
			    }
		|  INCOP term
			{  $$ = buildtree( $1==INCR ? ASG PLUS : ASG MINUS,
						$2,
						bcon(1)  );
			    }
		|  SIZEOF term
			{  $$ = doszof( $2 ); }
		|  LP cast_type RP term  %prec INCOP
			{
/*				NODE *bfk;
				bfk = $2;				** bk: pardon me?? */
				if (optid > 0) { 
					uerror( "declarator not allowed in cast" ); 
					optid = -1; 
				}
				$$ = buildtree( CAST, $2, $4 );
			    $$->in.left->in.op = FREE;
			    $$->in.op = FREE;
			    $$ = $$->in.right;
				ProtoMirage();
			    }
		|  SIZEOF LP cast_type RP  %prec SIZEOF
			{	
				if (optid>0) { 
					uerror( "declarator not allowed in sizeof" ); 
					optid = -1; 
				}
				$$ = doszof( $3 );
				ProtoMirage();
			}
		|  term LB e RB
			{  $$ = buildtree( LB, $1, $3 ); }
		|  term LB e COLON e RB
			{  $$ = xicolon( $1, $3, $5 ); }
		|  funct_idn  RP
			{  ftn_call = 0; $$=buildtree(UNARY CALL,$1,NIL);  }
		|  funct_idn elist  RP
			{ ftn_call = 0;  $$=buildtree(CALL,$1,$2);}
		|  term STROP NAME
			{  if( $2 == DOT ){
/*				if( notlval( $1 ) )uerror(
				       "structure reference must be addressable"
					); */				/* bk: see str_expr in trees.c */
				$1 = buildtree( UNARY AND, $1, NIL );
				}
			    idname = $3;
			    $$ = buildtree( STREF, $1,
					buildtree( NAME, NIL, NIL ) );
			    }
		|  NAME
			{  idname = $1;
			    /* recognize identifiers in initializations */
			    if( blevel==0 && stab[idname].stype == UNDEF ) {
				register NODE *q;
				werror( W_DFLT, "undeclared initializer name %s",
					stab[idname].sname );
				q = block( FREE, NIL, NIL, INT, 0, INT, 0 );
				q->tn.rval = idname;
				defid( q, EXTERN );
				}

			    if ( constid && $<intval>0 != MUL)  leftcons = 0;

			    $$=buildtree(NAME,NIL,NIL);
			    stab[$1].suse = -lineno;
			}
		|  ICON
			{  $$=bcon(0);
			    $$->tn.lval = lastcon;
			    $$->tn.rval = NONAME;
			    if( $1 ) $$->fn.csiz = $$->in.type = ctype( ULONG );	/*bk */
			    }
		|  FCON
			{  $$=buildtree(FCON,NIL,NIL);
			    $$->fpn.dval = dcon;
				if ( $1 ) $$->fn.csiz = $$->fpn.type = ctype(FLOAT);	/*bk*/
			    }
		|  STRING
			{  $$ = getstr(); /* get string contents */ }
		|   LP  e  RP
			{ $$=$2; }
		;


cast_type:	  SaveClass declspecs null_decl
			{
				$$ = tymerge( $2, $3 );
				$$->in.op = NAME;
				$2->in.op = FREE;
				if (incit) {
					incit = 0;
					$$->in.type = INCREF($$->in.type);
				}
				seen_typespec = 0;
				maybe_cast = 0;
			}
		;


SaveClass:		/* empty */		/* save curclass: "declspecs" clobbers it */
			{ saveclass = curclass; maybe_cast = 1;
			curclass=SNULL; }
		;

opt_parmtype_list:	proto_push 		/* empty, really */
			{	$$ = (NODE *) 0; }
		|	proto_push parmtype_list 
			{ $$ = $2; };
		;

null_decl:   optname
			{ $$ = bdty( NAME, NIL, $1 ); }
		|	LP  parmtype_list RP
			{ $$ = buildtree( UNARY CALL, bdty(NAME,NIL,-1),0); } 
		|	LP optname RP
			{ $$ = bdty( UNARY CALL, bdty(NAME,NIL,$2),0); }
		|  LP null_decl RP LP 
			{ ftnsave = optid; }
			opt_parmtype_list RP
			{ 
				optid = ftnsave;	/* optnames in opt_parm are comments only */
				$2->in.type = FTN|INT;
				$$ = bdty( UNARY CALL, $2, 0 ); 
				if ( $6 )					/* any prototype? */
					ProtoLink($2,0);		/* link prototypes to funct id */
			}
		|  pointer null_decl
			{  goto umul; }
		|  null_decl LB RB
			{  goto uary; }
		|  null_decl LB con_e RB
			{  goto bary;  }
		|  NAME LP 
			{ ftnsave = $1; }
			opt_parmtype_list RP
			{ 
				NODE *wrk;
				optid = ftnsave;	/* optnames in opt_parm are comments only */
				wrk = bdty( NAME, NIL, $1 );
				wrk->in.type = FTN|INT;
				incit = 1;					/* do INCREF once we have TWORD */
				$$ = bdty( UNARY CALL, wrk, 0 ); 
				if ( $4 )					/* any prototype? */
					ProtoLink(wrk,0);		/* link prototypes to funct id */
			}
		|  LP null_decl RP
			{ $$ = $2; }
		;

optname:	NAME
			{ optid = $1; seen_typespec = 0; }
		| /* EMPTY */
			{ optid = -1; $$ = -1; seen_typespec = 0; }
		;

proto_push:	/* empty */
			{ ProtoPush(); }
			;

funct_idn:	   NAME  LP 
			{  if( stab[$1].stype == UNDEF ){
				register NODE *q;
				q = block( FREE, NIL, NIL, FTN|INT, 0, INT, 0 );
				q->tn.rval = $1;
				defid( q, EXTERN );
				}
			    idname = $1;
			    $$=buildtree(NAME,NIL,NIL);
			    stab[idname].suse = -lineno;
				ftn_call = 1;			/* bk */
			}
		|  term  LP 
			{ 	/* bk: ANSI 3.3.2.2  */
				if ( ($1->in.type & ( FTN<<TSHIFT | PTR )) == 0 )	
					if ( ( $1->in.type & FTN ) == 0 )
						uerror( "illegal function expression" );
				ftn_call = 1; 
			}
		;
%%

NODE *
mkty( t, d, s, tq ) unsigned t; {
	return( block( TYPE, NIL, NIL, t, d, s, tq ) );
	}

NODE *
bdty( op, p, v ) NODE *p; {
	register NODE *q;

	q = block( op, p, NIL, INT, 0, INT, 0 );

	switch( op ){

	case UNARY MUL:
	case UNARY CALL:
		break;

	case LB:
		q->in.right = bcon(v);
		break;

	case NAME:
		q->tn.rval = v;
		break;

	default:
		cerror( "bad bdty" );
		}

	return( q );
	}

dstash( n ){ /* put n into the dimension table */
	if( curdim >= DIMTABSZ-1 ){
		cerror( "dimension table overflow");
		}
	dimtab[ curdim++ ] = n;
	}

savebc() {
	if( psavbc > & asavbc[BCSZ-4 ] ){
		cerror( "whiles, fors, etc. too deeply nested");
		}
	*psavbc++ = brklab;
	*psavbc++ = contlab;
	*psavbc++ = flostat;
	*psavbc++ = swx;
	flostat = 0;
	}

resetbc(mask){

	swx = *--psavbc;
	flostat = *--psavbc | (flostat&mask);
	contlab = *--psavbc;
	brklab = *--psavbc;

	}

addcase(p) NODE *p; { /* add case to switch */

	p = optim( p );  /* change enum to ints */
	if( p->in.op != ICON ){
		uerror( "non-integer constant case expression");
		return;
		}
	if( swp == swtab ){
		uerror( "case not in switch");
		return;
		}
	if( swp >= &swtab[SWITSZ] ){
		cerror( "switch table overflow");
		}
	icon_overflow( p, 1 );				/* bk: check for unsigned > INT_MAX */
	swp->sval = p->tn.lval;
	deflab( swp->slab = getlab() );
	++swp;
	tfree(p);
	}

adddef(){ /* add default case to switch */
	if( swp == swtab ){
		uerror( "default not inside switch");
		return;
		}
	if( swtab[swx].slab >= 0 ){
		uerror( "duplicate default in switch");
		return;
		}
	deflab( swtab[swx].slab = getlab() );
	}

swstart(){
	/* begin a switch block */
	if( swp >= &swtab[SWITSZ] ){
		cerror( "switch table overflow");
		}
	swx = swp - swtab;
	swp->slab = -1;
	++swp;
	}

swend(){ /* end a switch block */

	register struct sw *swbeg, *p, *q, *r, *r1;
	CONSZ temp;
	int tempi;

	swbeg = &swtab[swx+1];

	/* sort */

	r1 = swbeg;
	r = swp-1;

	while( swbeg < r ){
		/* bubble largest to end */
		for( q=swbeg; q<r; ++q ){
			if( q->sval > (q+1)->sval ){
				/* swap */
				r1 = q+1;
				temp = q->sval;
				q->sval = r1->sval;
				r1->sval = temp;
				tempi = q->slab;
				q->slab = r1->slab;
				r1->slab = tempi;
				}
			}
		r = r1;
		r1 = swbeg;
		}

	/* it is now sorted */

	for( p = swbeg+1; p<swp; ++p ){
		if( p->sval == (p-1)->sval ){
			uerror( "duplicate case in switch, %d", tempi=p->sval );
			return;
			}
		}

	reached = 1;
	genswitch( swbeg-1, (int)(swp-swbeg) );
	swp = swbeg-1;
	}



int 
decomp_type(n)
NODE * n;
{
  int ty;

  ty=n->in.type;
  switch ( ty ) {

   case ULONG:
   case UCHAR:
   case USHORT: if ( chktype[UNSIGNED -1 ] || chktype[ ty -UNSIGNED + INT -1] )
			uerror( "illegal type combination" );
		else {
			chktype[UNSIGNED -1 ]  = 1; 
			chktype[ ty - UNSIGNED + INT -1] = 1; 
			return(0);
		}
		break;
   case INT:
   case FLOAT:
   case DOUBLE:
   case LONG:
   case CHAR:
   case SHORT: if ( chktype[ ty -1 ] )
			uerror( "illegal type combination" );
		else {
			chktype[ ty -1 ]  = 1; 
			return(0);
		}
		break;
   default: return(-1);

   }
}


