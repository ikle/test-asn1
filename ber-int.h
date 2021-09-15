/*
 * ASN.1 BER integer helpers
 *
 * Copyright (c) 2016-2021 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef _BER_INT_H
#define _BER_INT_H  1

#include "asn1-int.h"
#include "ber-input.h"

int ber_get_int (struct ber_input *o, struct asn1_int *i);

#endif  /* _BER_INT_H */
