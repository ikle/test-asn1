/*
 * ASN.1 DER Tag Matcher
 *
 * Copyright (c) 2016-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef ASN1_DER_TAG_MATCH_H
#define ASN1_DER_TAG_MATCH_H  1

#include <asn1/der/tag-peek.h>

/*
 * Match N-octet tag as LE unit with in-wire format and eat data at window
 * head on success
 */
#define DER_DEFINE_TAG_MATCH(n, type)					\
									\
static inline int der_match_tag_##n (struct der_window *o, type tag)	\
{									\
	size_t avail = o->tail - o->head;				\
									\
	if (avail < n || der_peek_tag_##n (o) != tag)			\
		return 0;						\
									\
	o->head += n;							\
	return 1;							\
}

DER_DEFINE_TAG_MATCH (1, uint_fast8_t)
DER_DEFINE_TAG_MATCH (2, uint_fast16_t)
DER_DEFINE_TAG_MATCH (3, uint_fast32_t)
DER_DEFINE_TAG_MATCH (4, uint_fast32_t)

#endif  /* ASN1_DER_TAG_MATCH_H */
