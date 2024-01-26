/*
 * ASN.1 Time Show helper
 *
 * Copyright (c) 2016-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdio.h>
#include <stdlib.h>

#include <asn1-mini/asn1-time.h>

int asn1_show_time (char *buf, size_t len, struct asn1_time *t)
{
	return snprintf (buf, len, "%4u-%02u-%02u %02u:%02u:%02u %+03d%02d",
			 t->year, t->mon, t->day,
			 t->hour, t->min, t->sec,
			 t->shift / 60, abs (t->shift) % 60);
}
