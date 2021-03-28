#ifndef __LOCALE_H
#ident	"@(#)_locale.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."

#include "_ctype.h"
#include <locale.h>

#define _P_locale "/lib/locale/"
#define _L_locale (sizeof ( _P_locale ))
#define _MAXLENLOCALE 14
#define _SZ_COLLATE 512
#define _SZ_SUBS    128

extern struct lconv *_monetary_num;
extern char _cur_locale[LC_MAX][LC_NAMELEN];
extern short _colltbl[_SZ_COLLATE/2];
extern unsigned char _1_to_n[_SZ_SUBS];
extern unsigned short  _2_to_1[_SZ_SUBS];
extern unsigned short  _val_2_to_1[_SZ_SUBS];

extern char *_fullocale(char *,char *);
extern char *_nativeloc(int);
extern int _setctype(char *);
extern int _setcollate(char *);
extern int _settime(char *);
extern int _setnumeric(char *);
extern int _setmonetary(char *);

#define __LOCALE_H

#endif /* __LOCALE_H */
