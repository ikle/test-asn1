/*
 * ASN.1 input helpers
 *
 * Copyright (c) 2016-2021 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef _ASN1_INPUT_H
#define _ASN1_INPUT_H  1

// todo: add possibility to specify user buffer
struct asn1_input *asn1_input_open (
	int (*read) (void *buffer, int size, void *ctx), void *ctx);
// todo: add possibility to fetch buffer on close
void asn1_input_close (struct asn1_input *o);

int asn1_input_peek (struct asn1_input *o);
int asn1_input_get  (struct asn1_input *o);
long asn1_input_gets (struct asn1_input *o, void *to, long count);

#endif  /* _ASN1_INPUT_H */
