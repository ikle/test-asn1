/*
 * ASN.1 BER Blob Input helper
 *
 * Copyright (c) 2016-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <asn1-mini/ber-input.h>

long ber_get_blob (struct ber_input *o, void *data, long count)
{
	unsigned char *dst = data;
	long i;
	int a;

	for (i = 0; i < count; ++i)
		if ((a = ber_get (o)) < 0)
			return a;
		else
			dst[i] = a;
	return i;
}
