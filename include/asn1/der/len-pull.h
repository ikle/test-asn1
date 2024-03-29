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

/*
 * Generic non-inline length fetcher, returns non-zero on success
 */
int der_pull_len (struct der_window *o, size_t *len);

/*
 * Inline length fetchers
 */
static inline int der_pull_len_0_7 (struct der_window *o, size_t *len)
{
	size_t avail = o->tail - o->head;

	if (avail < 1 || o->head[0] >= 0x80)
		return 0;

	*len = o->head[0];
	o->head += 1;
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
	size_t avail = o->tail - o->head;				\
									\
	if (o->head[0] != (0x80 + n) || avail < (1 + n))		\
		return 0;						\
									\
	*len = der_peek_len_##n (o);					\
	o->head += 1 + n;						\
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
DER_DEFINE_LEN_PULL_COMP (0, 24, 32)

#endif  /* ASN1_DER_LEN_PULL_H */
