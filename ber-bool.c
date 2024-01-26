/*
 * ASN.1 BER Boolean helpers
 *
 * Copyright (c) 2016-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <asn1-mini/ber-bool.h>

int ber_get_bool (struct ber_input *o)
{
	int a, v;

	for (v = 0; o->len > 0; v |= a)
		if ((a = ber_get (o)) < 0)
			return a;

	return v != 0;
}
