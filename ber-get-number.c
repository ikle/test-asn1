/*
 * ASN.1 BER Long Number helpers
 *
 * Copyright (c) 2016-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <asn1-mini/ber-number.h>

long ber_get_number (struct ber_input *o)
{
	int a;
	long n;
	int processed;

	for (n = 0, processed = 0; processed < 4; ++processed) {
		if ((a = ber_get (o)) < 0)
			return a;

		n = (n << 7) | (a & 0x7f);

		if ((a & 0x80) == 0)
			return n;
	}

	return -EPROTONOSUPPORT;
}
