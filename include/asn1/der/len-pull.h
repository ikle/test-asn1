/*
 * ASN.1 DER Length Fetcher
 *
 * Copyright (c) 2016-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef ASN1_DER_LEN_PULL_H
#define ASN1_DER_LEN_PULL_H  1

#include <asn1/der/len-peek.h>

static inline int der_pull_len_0_7 (struct der_window *o, size_t *len)
{
	unsigned char *head = o->head + 1;

	if (head > o->tail || o->head[0] >= 0x80)
		return 0;

	*len = o->head[0];
	o->head = head;
	return 1;
}

/*
 * Note, o->head[0] must be accessible on entry
 */
#define DER_DEFINE_LEN_PULL(n, l, h)					\
									\
static inline								\
int der_pull_len_##l##_##h (struct der_window *o, size_t *len)		\
{									\
	unsigned char *head = o->head + 1 + n;				\
									\
	if (o->head[0] != (0x81 + n) || head > o->tail)			\
		return 0;						\
									\
	*len = der_peek_len_##n (o);					\
	o->head = head;							\
	return 1;							\
}

DER_DEFINE_LEN_PULL (1,  7, 8)
DER_DEFINE_LEN_PULL (2,  8, 16)
DER_DEFINE_LEN_PULL (3, 16, 24)
DER_DEFINE_LEN_PULL (4, 24, 32)

#define DER_DEFINE_LEN_PULL_COMP(l, m, h)				\
									\
static inline								\
int der_pull_len_##l##_##h (struct der_window *o, size_t *len)		\
{									\
	return	der_pull_len_##l##_##m (o, len) ||			\
		der_pull_len_##m##_##h (o, len);			\
}

DER_DEFINE_LEN_PULL_COMP (0,  7, 8)
DER_DEFINE_LEN_PULL_COMP (0,  8, 16)
DER_DEFINE_LEN_PULL_COMP (0, 16, 24)

#endif  /* ASN1_DER_LEN_PULL_H */
