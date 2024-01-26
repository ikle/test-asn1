/*
 * ASN.1 BER Input helpers
 *
 * Copyright (c) 2016-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <asn1-mini/ber-input.h>

#define BER_GET_TAG_EXTRA(n) 			\
	do {					\
		if ((a = ber_get (o)) < 0)	\
			return a;		\
						\
		tag |= a << ((n) * 8);		\
						\
		if ((a & 0x80) == 0)		\
			return tag;		\
	} while (0)

/* NOTE: we use in-wire format of tag loaded into LE 32-bit unit */
long ber_get_tag (struct ber_input *o)
{
	long tag;
	int a;

	if ((a = ber_get (o)) < 0)
		return a;

	tag = a;

	if ((a & 0x1f) != 0x1f)
		return tag;

	BER_GET_TAG_EXTRA (1);
	BER_GET_TAG_EXTRA (2);
	BER_GET_TAG_EXTRA (3);

	return -EPROTONOSUPPORT;
}

long ber_get_len (struct ber_input *o)
{
	int a, count;
	long len;

	if ((a = ber_get (o)) < 0 ||
	    (a & 0x80) == 0)
		return a;

	if (a == 0x80)
		return BER_LEN_INDEFINITE;

	count = a & 0x7f;

	if (count > 4)
		return -EPROTONOSUPPORT;

	for (len = 0; count > 0; --count) {
		if ((a = ber_get (o)) < 0)
			return a;

		len = (len << 8) | a;
	}

	return len;
}
