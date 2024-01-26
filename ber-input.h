/*
 * ASN.1 BER input helpers
 *
 * Copyright (c) 2018-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef _BER_INPUT_H
#define _BER_INPUT_H  1

#include <limits.h>

#include "asn1-input.h"

struct ber_input {
	struct asn1_input *in;
	long len;
};

#define BER_LEN_INDEFINITE  LONG_MAX

int ber_peek (struct ber_input *o);
int ber_get  (struct ber_input *o);

/* NOTE: we use in-wire format of tag loaded into LE 32-bit unit */
long ber_get_tag (struct ber_input *o);
long ber_get_len (struct ber_input *o);

long ber_get_blob (struct ber_input *o, void *data, long count);

#endif  /* _BER_INPUT_H */
