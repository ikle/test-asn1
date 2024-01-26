/*
 * ASN.1 BER Long Number helpers
 *
 * Copyright (c) 2016-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef _BER_NUMBER_H
#define _BER_NUMBER_H  1

#include <asn1-mini/ber-input.h>

long ber_get_number (struct ber_input *o);

#endif  /* _BER_NUMBER_H */
