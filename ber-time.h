/*
 * ASN.1 BER Time helpers
 *
 * Copyright (c) 2016-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef _BER_TIME_H
#define _BER_TIME_H  1

#include <asn1-mini/asn1-time.h>

#include "ber-input.h"

int ber_get_utctime (struct ber_input *o, struct asn1_time *t);
int ber_get_gentime (struct ber_input *o, struct asn1_time *t);

#endif  /* _BER_TIME_H */
