#	START NEW ARIX SCCS HEADER
#
#	@(#) iob.sh: version 25.1 created on 12/2/91 at 19:51:04
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)iob.sh	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
SGS_PATH=`grep '^SGS=' $IROOT/cc`
STDIO=`echo $SGS_PATH | sed -e "s/^SGS=//"`
set `grep '#define FOPEN_MAX' $STDIO/usr/include/stdio.h`
echo " #include <stdio.h>"
echo " #ifdef DYNAMIC"
echo "	extern FILE _iob[];"
echo "	extern unsigned char _smbuf[];"
echo "	FILE _iob[_NFILE] = {
		{ 0, NULL, NULL, _IOREAD|_IOIOB, 0, NULL, stdout, NULL },
		{ 0, NULL, NULL, _IOWRT|_IOIOB, 1, NULL, stderr, stdin },
		{ 0, _smbuf, _smbuf, _IOWRT|_IONBF|_IOIOB, 2,
		  &_smbuf[_SBFSIZ], NULL, stdout }, "
COUNT=4
LOOPS=`expr $3 - 1`

echo "		{ 0, NULL, NULL, _IOIOB, 0, NULL, &_iob[`expr $COUNT`], NULL }, "
while [ $COUNT -lt $LOOPS ]
do
	echo "		{ 0, NULL, NULL, _IOIOB, 0, NULL, &_iob[`expr $COUNT + 1`], &_iob[`expr $COUNT - 1`] }, "
	COUNT=`expr $COUNT + 1`
done

echo "		{ 0, NULL, NULL, _IOIOB, 0, NULL, NULL, &_iob[`expr $COUNT - 1`] } "
echo "	};"
echo "FILE *_firstbuf = _iob;"
echo "FILE *_freeiob = &_iob[ 3 ];"
echo "FILE *_freemalloc = NULL;"
echo "#endif /* DYNAMIC */"
