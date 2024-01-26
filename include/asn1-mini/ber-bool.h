/*
 * ASN.1 BER Boolean helpers
 *
 * Copyright (c) 2016-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef _BER_BOOL_H
#define _BER_BOOL_H  1

#include <asn1-mini/ber-input.h>

int ber_get_bool (struct ber_input *o);

#endif  /* _BER_BOOL_H */
