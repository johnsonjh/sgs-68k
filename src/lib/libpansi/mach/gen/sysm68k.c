#ident	"@(#)sysm68k.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*		Copyright (c) 1985 AT&T		*/
/*		All Rights Reserved		*/
/*	MOTOROLA:sysm68k.c	6.2		*/
/*	MOTOROLA:sysm68k.c	6.1		*/
/*
 * sysm68k - Motorola specific system call
 */
#ifndef M68881
#define M68881 1
#endif
sysm68k( cmd, arg1, arg2 )
{
	extern _sysm68k();

	switch( cmd )
	{
 	case 1:		/* grow/shrink stack */
		{
		register x;
#ifdef DEBUG
		printf("\n\tcmd = %x\targ1 = %x\targ2 = %x\t*arg2 = %x\n",
			cmd, arg1, arg2, *( (int *)arg2 ) );

#endif
		if( ( x = _sysm68k( 1, arg1 ) ) == -1 )
			return( -1 );
#ifdef DEBUG
		printf("\n\tx = %x\n", x);
#endif
		*(int *)arg2 = x;
		return( 0 );
		}
#ifdef M68881
 	case 3:		/* Fetch offending MC68881 exception operand */
		{
		register x;
#ifdef DEBUG
		printf("\n\tcmd = %x\targ1 = %x\n", cmd, arg1);

#endif
		if( ( x = _sysm68k( 3, arg1 ) ) == -1 )
				return( -1 );
#ifdef DEBUG
		printf("\n\tx = %x\n", x);
#endif
		return( 0 );
		}
#endif
	default:
		return( -1 );
	}
}
