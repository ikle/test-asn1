/*
 * ASN.1 BER Input helpers
 *
 * Copyright (c) 2018-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <errno.h>

#include <asn1-mini/ber-input.h>

/*
 * Returns zero and skips input on EOC (End-of-Content), negative value on
 * error, or tag > 0 and child input on success (and reduce input length by
 * child length).
 */
long ber_get_head (struct ber_input *o, struct ber_input *child)
{
	long tag, len;

	if ((tag = ber_get_tag (o)) < 0)
		return tag;

	if ((len = ber_get_len (o)) < 0)
		return len;

	if (len > o->len)
		return -ENODATA;

	if (tag == 0)  /* EOC, End-of-Content */
		return ber_skip (o);

	child->in  = o->in;
	child->len = len;

	if (o->len != BER_LEN_INDEFINITE)
		o->len -= len;

	return tag;
}
