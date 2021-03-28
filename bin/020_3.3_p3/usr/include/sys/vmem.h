#ifndef SYS_VMEM_H
#define SYS_VMEM_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) vmem.h: version 24.1 created on 10/28/91 at 18:41:03	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)vmem.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ifndef	SYS_IMMU_H
#include <sys/immu.h>
#endif	/* SYS_IMMU_H */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* vmem.h	rev 0
* a few terms:
*	supervisor = a program is executing with supervisor bit on.
*	user = the supervisor bit is off
*	touch = read, write, or modify a storage location
*	GB, KB, Kb = gigabyte, kilobyte, kilobit
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*	Virtual Spaces
* The MMU or the io map must be used to access 
* other modules on the CSS bus including main memory.
*
*/
#define KMEM_START ((unsigned)0x80000000)
#define KMEM_SIZE 0x40000000	/* 1 GB Supervisor space
					under MMU control	*/
#define KIO_START ((unsigned)0xc0000000)
#define KIO_SIZE 0x40000000	/* 1 GB I/O space */

#define UMEM_START 0x00000000
#define UMEM_SIZE 0x80000000	/* 2 GB User space controlled by MMU	*/

#define KUMEM_START 0x00000000	
#define KUMEM_SIZE 0x80000000	/* 2 GB User space controlled by MMU
					as seen by Supervisor	*/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*	Memory Management Unit
* enforces permissions, translates addresses, keeps page statistics
*
*
*/
#define PAGE_SIZE NBPP		/* the granularity of memory management */

/*	The "root pointer" registers determine the physical address
*	where the two active page pointer tables begin.
*--->caution: these must be accessed as aligned long operands.		*/
#ifdef M68020
#define URP_REG 0xffe41000	/* Active user root pointer		*/
#define KRP_REG 0xffe41004	/* Supervisor root pointer		*/
#endif

/* * * *
*	A typical Page Pointer Table entry
* +----------------------------------------------+----------+--------+
* |  35-- physical pointer to a Page Table  --11 |xxxxxxxxxx| unused |
* +----------------------------------------------+----------+--------+
*  31                 three bytes               7      6     5      0
*
* An entry is selected in the Page Table by virtual address bits 21-12.
*
*
* * * * * *
#define PPTE_BASE   0xffffff00	/* & mask for phys addr part		*/
#define PPTE_VBITOK 0x00000080	/* | value to mark table ok		*/
#define PPTE_VBITER 0xffffff7f	/* & mask to mark table BUS ERROR	*/

/* * * *
*	A typical Page Table entry
* +----------------------------------------------+-------------------+
* |  35-- physical pointer to a memory page --12 |     flag byte     |
* +----------------------------------------------+-------------------+
*                                               8!7                 0!
*  ............................................./                    !
* /                                                                  !
* !   7       6        5        4       3        2       1       0   !
* +-------+-------+---------+-------+-------+--------+-------+-------+
* | spare | valid | touched | dirty | cache | readok | wrtok | xqtok |
* +-------+-------+---------+-------+-------+--------+-------+-------+
* A byte is selected in the memory page by virtual address bits 11-0.
*
* "valid" tells the MMU whether to give a BUS ERROR EXCEPTION
* if the process touches the page.
*
* "touched" tells the page has been read, written, or modified
* "dirty" says the page was written
* Touched and dirty are kept current by the MMU.
*
* "cache" says the page is cacheable
* 
* "readok, wrtok, and xqtok" are permission bits
*
* "spare" is not yet defined.
* * * * * */
#define PTE_BASE   0xffffff00	/* & mask for phys addr part		*/
#define PTE_ATTR   0x000000ff	/* & mask for attributes part		*/
#define PTE_SPARE  0x80		/* Spare attribute bit			*/
#define PTE_FAULT  0x40		/* Fault attribute bit			*/
#define PTE_TOUCH  0x20		/* Touched attribute bit		*/
#define PTE_DIRTY  0x10		/* Dirty attribute bit			*/
#define PTE_CACHE  0x08		/* Cacheable attribute bit		*/
#define PTE_RPERM  0x04		/* Read permission attribute bit	*/
#define PTE_WPERM  0x02		/* Write permission attribute bit	*/
#define PTE_XPERM  0x01		/* Execute permission attribute bit	*/
#define PTE_RWX    0x07		/* Read, write and execute perms        */

#endif /* SYS_VMEM_H */
