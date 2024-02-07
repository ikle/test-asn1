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

/*
 * Note, this function should be used with compile-constant tag value
 */
static inline int der_match_tag (struct der_window *o, uint_fast32_t tag)
{
	return	tag <= 255u       ? der_match_tag_1 (o, tag) :
		tag <= 65535u     ? der_match_tag_2 (o, tag) :
		tag <= 16777215ul ? der_match_tag_3 (o, tag) :
		/* otherwise */     der_match_tag_4 (o, tag) ;
}

#endif  /* ASN1_DER_TAG_MATCH_H */
