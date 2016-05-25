/*
 * ASN.1 time
 */

#ifndef _ASN1_TIME_H
#define _ASN1_TIME_H  1

struct asn1_time {
	int year, mon, day, hour, min, sec, shift;
};

#endif  /* _ASN1_TIME_H */
