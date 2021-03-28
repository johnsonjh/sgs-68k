#ident	"@(#)rpc.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#
#	Copyright (c) 1984 Sun Microsystems, Inc.  
#
#
#	Copyright 1986 Lachman Associates Inc. and The Instruction Set Ltd.
#
#      @(#)makefile	2.4 LAI KNFS for ARETE  source
#      @(#)makefile	2.8 System V NFS  source
#

include $(IROOT)/macdefs
include $(IROOT)/sgsdefs

FILES = pmap_clnt.o pmap_rmt.o clnt_udp.o sysv42.o au_none.o au_unix.o au_ux_prot.o \
	clnt_perr.o clnt_raw.o clnt_simp.o clnt_tcp.o get_time.o getdom.o \
	pmap_gtmps.o pmap_gtprt.o pmap_prot.o rename.o rhost.o experror.o\
	rpc_prot.o svc.o svc_au_ux.o svc_auth.o svc_raw.o svc_simp.o\
	svc_tcp.o svc_udp.o xdr.o xdr_array.o xdr_float.o xdr_mem.o sysvexos.o\
	xdr_rec.o xdr_refnce.o xdr_stdio.o getfh_i.o nfs_svc_i.o asyncd_i.o

SOURCE =pmap_clnt.c pmap_rmt.c clnt_udp.c sysv42.c au_none.c au_unix.c au_ux_prot.c \
	clnt_perr.c clnt_raw.c clnt_simp.c clnt_tcp.c get_time.c getdom.c \
	pmap_gtmps.c pmap_gtprt.c pmap_prot.c rename.c rhost.c experror.c\
	rpc_prot.c svc.c svc_au_ux.c svc_auth.c svc_raw.c svc_simp.c\
	svc_tcp.c svc_udp.c xdr.c xdr_array.c xdr_float.c xdr_mem.c sysvexos.c\
	xdr_rec.c xdr_refnce.c xdr_stdio.c getfh_i.c nfs_svc_i.c asyncd_i.c 

all: $(FILES) 

experror.o:	 experror.c 
	$(CC) $(CFLAGS) -c experror.c
rhost.o:	 rhost.c 
	$(CC) $(CFLAGS) -c rhost.c
pmap_clnt.o:	 pmap_clnt.c 
	$(CC) $(CFLAGS) -c pmap_clnt.c
pmap_rmt.o:	 pmap_rmt.c 
	$(CC) $(CFLAGS) -c pmap_rmt.c 
clnt_udp.o:	 clnt_udp.c 
	$(CC) $(CFLAGS) -c clnt_udp.c 
sysv42.o:	 sysv42.c 
	$(CC) $(CFLAGS) -c sysv42.c
au_none.o:	 au_none.c 
	$(CC) $(CFLAGS) -c au_none.c 
au_unix.o:	 au_unix.c 
	$(CC) $(CFLAGS) -c au_unix.c 
au_ux_prot.o:	 au_ux_prot.c 
	$(CC) $(CFLAGS) -c au_ux_prot.c
clnt_perr.o:	 clnt_perr.c 
	$(CC) $(CFLAGS) -c clnt_perr.c 
clnt_raw.o:	 clnt_raw.c 
	$(CC) $(CFLAGS) -c clnt_raw.c 
clnt_simp.o:	 clnt_simp.c 
	$(CC) $(CFLAGS) -c clnt_simp.c 
clnt_tcp.o:	 clnt_tcp.c 
	$(CC) $(CFLAGS) -c clnt_tcp.c 
get_time.o:	 get_time.c 
	$(CC) $(CFLAGS) -c get_time.c 
getdom.o:	 getdom.c 
	$(CC) $(CFLAGS) -c getdom.c 
pmap_gtmps.o:	 pmap_gtmps.c 
	$(CC) $(CFLAGS) -c pmap_gtmps.c 
pmap_gtprt.o:	 pmap_gtprt.c 
	$(CC) $(CFLAGS) -c pmap_gtprt.c 
pmap_prot.o:	 pmap_prot.c 
	$(CC) $(CFLAGS) -c pmap_prot.c 
rename.o:	 rename.c 
	$(CC) $(CFLAGS) -c rename.c 
rpc_prot.o:	 rpc_prot.c 
	$(CC) $(CFLAGS) -c rpc_prot.c 
sysvexos.o:	 sysvexos.c 
	$(CC) $(CFLAGS) -c sysvexos.c 
svc.o:	 svc.c 
	$(CC) $(CFLAGS) -c svc.c 
svc_au_ux.o:	 svc_au_ux.c 
	$(CC) $(CFLAGS) -c svc_au_ux.c 
svc_auth.o:	 svc_auth.c 
	$(CC) $(CFLAGS) -c svc_auth.c 
svc_raw.o:	 svc_raw.c 
	$(CC) $(CFLAGS) -c svc_raw.c 
svc_simp.o:	svc_simp.c
	$(CC) $(CFLAGS) -c svc_simp.c
svc_tcp.o:	 svc_tcp.c 
	$(CC) $(CFLAGS) -c svc_tcp.c 
svc_udp.o:	 svc_udp.c 
	$(CC) $(CFLAGS) -c svc_udp.c 
xdr.o:	 xdr.c 
	$(CC) $(CFLAGS) -c xdr.c 
xdr_array.o:	 xdr_array.c 
	$(CC) $(CFLAGS) -c xdr_array.c 
xdr_float.o:	 xdr_float.c 
	$(CC) $(CFLAGS) -c xdr_float.c 
xdr_mem.o:	xdr_mem.c
	$(CC) $(CFLAGS) -c xdr_mem.c
xdr_rec.o:	 xdr_rec.c 
	$(CC) $(CFLAGS) -c xdr_rec.c 
xdr_refnce.o:	 xdr_refnce.c 
	$(CC) $(CFLAGS) -c xdr_refnce.c 
xdr_stdio.o:	 xdr_stdio.c 
	$(CC) $(CFLAGS) -c xdr_stdio.c 
getfh_i.o:	 getfh_i.c 
	$(CC) $(CFLAGS) -c getfh_i.c 
nfs_svc_i.o:	 nfs_svc_i.c 
	$(CC) $(CFLAGS) -c nfs_svc_i.c 
asyncd_i.o:	 asyncd_i.c 
	$(CC) $(CFLAGS) -c asyncd_i.c 

install: all 

clean:
	-rm -f *.o

clobber:	clean
