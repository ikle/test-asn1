/*
 * ASN.1 BER Long Number helpers
 *
 * Copyright (c) 2016-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <limits.h>

#include <asn1-mini/ber-number.h>

#define NDIGITS  ((sizeof (long) * CHAR_BIT - 1) / 7)

long ber_get_number (struct ber_input *o)
{
	int a;
	long n;
	int processed;

	for (n = 0, processed = 0; processed < NDIGITS; ++processed) {
		if ((a = ber_get (o)) < 0)
			return a;

		n = (n << 7) | (a & 0x7f);

		if ((a & 0x80) == 0)
			return n;
	}

	return -EPROTONOSUPPORT;
}
