/*
 * ASN.1 DER Tag Peeker
 *
 * Copyright (c) 2016-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef ASN1_DER_TAG_PEEK_H
#define ASN1_DER_TAG_PEEK_H  1

#include <asn1/der/core.h>

/*
 * Peek and return 1-octet tag
 */
static inline uint_fast8_t der_peek_tag_1 (struct der_window *o)
{
	return o->head[0];
}

/*
 * Peek and return N-octet tag as LE unit with in-wire format
 */
#define DER_DEFINE_TAG_PEEK(n, m, type)					\
									\
static inline type der_peek_tag_##n (struct der_window *o)		\
{									\
	return (type) o->head[m] << (m * 8) | der_peek_tag_##m (o);	\
}

DER_DEFINE_TAG_PEEK (2, 1, uint_fast16_t)
DER_DEFINE_TAG_PEEK (3, 2, uint_fast32_t)
DER_DEFINE_TAG_PEEK (4, 3, uint_fast32_t)

#endif  /* ASN1_DER_TAG_PEEK_H */
