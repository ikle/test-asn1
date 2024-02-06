/*
 * ASN.1 DER Length Fetcher
 *
 * Copyright (c) 2016-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdint.h>

#include <asn1/der/len-pull.h>

#if SIZE_MAX >= 18446744073709551615ull
#	define LEN_MAX  8

#elif SIZE_MAX >= 4294967295ul
#	define LEN_MAX  4
#else
#	define LEN_MAX  2
#endif

/*
 * Generic non-inline length fetcher, returns non-zero on success
 */
int der_pull_len (struct der_window *o, size_t *len)
{
	unsigned char *head = o->head + 1;
	size_t count, x, i;

	if (head > o->tail || o->head[0] == 0x80)  /* reject indefinite */
		return 0;

	if (o->head[0] < 0x80) {
		*len = o->head[0];
		o->head = head;
		return 1;
	}

	if ((count = o->head[0] & 0x7f) > LEN_MAX || (head += count) > o->tail)
		return 0;

	for (x = o->head[0 + 1], i = 1; i < count; ++i)
		x = (x << 8) | o->head[i + 1];

	*len = x;
	o->head = head;
	return 1;
}
