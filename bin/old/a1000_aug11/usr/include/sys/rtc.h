#ifndef SYS_RTC_H
#define SYS_RTC_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) rtc.h: version 2.1 created on 5/22/89 at 19:10:14	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)rtc.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)uts/head/sys:rtc.h	2.1"

#define RTC_WRT		0x01		/* Output bits */
#define RTC_READ	0x02
#define RTC_ADDR	0x04
#define RTC_SEL		0x08

#define RTC_BUSY	0x10		/* Input bits */
#define RTC_ZERO	0x20

#define RTC_DMASK	0x0F		/* Data bits mask */

struct rtc_dat {
	unsigned char sec_1;
	unsigned char sec_10;
	unsigned char min_1;
	unsigned char min_10;
	unsigned char hour_1;
	unsigned char hour_10;
	unsigned char weekday;
	unsigned char day_1;
	unsigned char day_10;
	unsigned char month_1;
	unsigned char month_10;
	unsigned char year_1;
	unsigned char year_10;
	unsigned char null;
    };

extern struct rtc_dat rtc_dat;

#define RTC_SEC_1	0
#define RTC_SEC_10	1
#define RTC_MIN_1	2
#define RTC_MIN_10	3
#define RTC_HOUR_1	4
#define RTC_HOUR_10	5
#define RTC_WEEKDAY	6
#define RTC_DAY_1	7
#define RTC_DAY_10	8
#define RTC_MONTH_1	9
#define RTC_MONTH_10	10
#define RTC_YEAR_1	11
#define RTC_YEAR_10	12

#endif /* SYS_RTC_H */
