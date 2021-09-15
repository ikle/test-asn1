/*
 * ASN.1 BER integer helpers
 *
 * Copyright (c) 2016-2021 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "ber-int.h"

#define ROUND_UP(n, to)  ((n + ((to) - 1)) / (to))

int ber_get_int (struct ber_input *o, struct asn1_int *i)
{
	int a, sign;
	long len = ROUND_UP (o->len, sizeof (*i->n));
	asn1_limb_t n, *p;

	if ((a = asn1_int_init (i, len)) < 0)
		return a;

	if ((a = ber_peek (o)) < 0)
		return a;

	sign = (a & 0x80) != 0;

	for (
		p = i->n + len - 1, n = sign ? ~(asn1_limb_t) 0 : 0;
		o->len > 0;
	) {
		if ((a = ber_get (o)) < 0)
			return a;

		n = (n << 8) | a;

		if (o->len % ASN1_LIMB_SIZE == 0) {
			*p = n;
			--p;
		}
	}

	return 0;
}
