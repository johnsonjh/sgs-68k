#ifndef __SIGNAL_H
#ident	"@(#)_signal.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."

extern void (*_sighold(int))(int);
extern void (*_sigignore(int))(int);
extern void (*_sigpause(int))(int);
extern void (*_sigrelse(int))(int);
extern void (*_sigset(int,void (*)(int)))(int);

#define __SIGNAL_H

#endif /* __SIGNAL_H */
