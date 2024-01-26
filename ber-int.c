/*
 * ASN.1 BER Integer helpers
 *
 * Copyright (c) 2016-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <errno.h>
#include <limits.h>

#include <asn1-mini/ber-int.h>

#define ROUND_UP(n, to)  ((n + ((to) - 1)) / (to))

int ber_get_int (struct ber_input *o, struct asn1_int *i)
{
	int a, sign;
	long len = ROUND_UP (o->len, sizeof (*i->n));
	long size = len * sizeof (*i->n);
	asn1_limb_t n, *p;

	if (size < o->len || len >= INT_MAX)  /* len overflow */
		return -ENOMEM;

	if ((a = asn1_int_init (i, len)) < 0 || len == 0)
		return a;

	if ((a = ber_get (o)) < 0)
		return a;

	sign = (a & 0x80) != 0;
	p = i->n + len - 1, n = sign ? ~(asn1_limb_t) 0 : 0;

	do {
		n = (n << 8) | a;

		if (o->len % ASN1_LIMB_SIZE == 0) {
			*p = n;
			--p;
		}
	}
	while ((a = ber_get (o)) >= 0);

	return o->len == 0 ? 0 : a;
}
