/*
 * ASN.1 Time
 *
 * Copyright (c) 2016-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef _ASN1_TIME_H
#define _ASN1_TIME_H  1

#include <stddef.h>

struct asn1_time {
	int year, mon, day, hour, min, sec, shift;
};

int asn1_show_time (char *buf, size_t len, struct asn1_time *t);

#endif  /* _ASN1_TIME_H */
