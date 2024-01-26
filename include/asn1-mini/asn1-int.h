/*
 * ASN.1 Integer
 *
 * Copyright (c) 2016-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef _ASN1_INT_H
#define _ASN1_INT_H  1

#include <stdint.h>

#ifndef ASN1_LIMB_TYPE
#define ASN1_LIMB_TYPE  unsigned long
#endif

typedef ASN1_LIMB_TYPE asn1_limb_t;

/* limb size in octets */
#define ASN1_LIMB_SIZE  (sizeof (asn1_limb_t) * CHAR_BIT / 8)

struct asn1_int {
	int count;
	asn1_limb_t *n;
};

int  asn1_int_init (struct asn1_int *i, int count);
void asn1_int_fini (struct asn1_int *i);

void asn1_int_normalize (struct asn1_int *i);

#endif  /* _ASN1_INT_H */
