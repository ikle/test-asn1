/*
 * ASN.1 DER Length Peeker
 *
 * Copyright (c) 2016-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef ASN1_DER_LEN_PEEK_H
#define ASN1_DER_LEN_PEEK_H  1

#include <asn1/der/core.h>

/*
 * Peek and return 1-octet length (for length in range [128, 255))
 */
static inline size_t der_peek_len_1 (struct der_window *o)
{
	return o->head[1];
}

/*
 * Peek and return N-octet length (BE number)
 */
#define DER_DEFINE_LEN_PEEK(n, m)					\
									\
static inline size_t der_peek_len_##n (struct der_window *o)		\
{									\
	return der_peek_len_##m (o) << 8 | o->head[m];			\
}

DER_DEFINE_LEN_PEEK (2, 1)
DER_DEFINE_LEN_PEEK (3, 2)
DER_DEFINE_LEN_PEEK (4, 3)

#endif  /* ASN1_DER_LEN_PEEK_H */
