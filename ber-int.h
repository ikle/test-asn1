/*
 * ASN.1 BER Integer helpers
 *
 * Copyright (c) 2016-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef _BER_INT_H
#define _BER_INT_H  1

#include <asn1-mini/asn1-int.h>
#include <asn1-mini/ber-input.h>

int ber_get_int (struct ber_input *o, struct asn1_int *i);

#endif  /* _BER_INT_H */
