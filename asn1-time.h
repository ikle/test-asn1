/*
 * ASN.1 time
 *
 * Copyright (c) 2016-2021 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef _ASN1_TIME_H
#define _ASN1_TIME_H  1

struct asn1_time {
	int year, mon, day, hour, min, sec, shift;
};

#endif  /* _ASN1_TIME_H */
