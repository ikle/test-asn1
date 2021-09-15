/*
 * ASN.1 output helpers
 *
 * Copyright (c) 2018-2021 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <assert.h>
#include <stdlib.h>

#include "asn1-output.h"

struct asn1_output {
	unsigned char *buffer;
	int (*write) (const void *buffer, int size, void *ctx);
	void *ctx;
	unsigned char *head, *tail;
};

#define ASN1_BUFFER_SIZE  4096

struct asn1_output *asn1_output_open (
	int (*write) (const void *buffer, int size, void *ctx), void *ctx)
{
	struct asn1_output *o;

	assert (write != NULL);		// todo: EINVAL

	if ((o = malloc (sizeof (*o))) == NULL)
		return NULL;

	if ((o->buffer = malloc (ASN1_BUFFER_SIZE)) == NULL)
		goto no_buffer;

	o->write = write;
	o->ctx   = ctx;
	o->tail = o->head = o->buffer;

	return o;
no_buffer:
	free (o);
	return NULL;
}

static int asn1_output_write (struct asn1_output *o)
{
	int ret;

	assert (o != NULL);
	assert (o->head < o->tail);

	do
		ret = o->write (o->head, o->tail - o->head, o->ctx);
	while (ret == 0);

	if (ret < 0)
		return ret;

	// todo: check res < size of buffer or ERANGE/EINVAL
	o->head += ret;

	assert (o->head <= o->tail);

	if (o->head == o->tail)
		o->tail = o->head = o->buffer;

	return ret;
}

int asn1_output_flush (struct asn1_output *o)
{
	assert (o != NULL);

	if (o->head == o->tail)
		return 0;

	return asn1_output_write (o);
}

int asn1_output_close (struct asn1_output *o)
{
	int ret;

	assert (o != NULL);

	ret = asn1_output_flush (o);

	free (o->buffer);
	free (o);
	return ret;
}

int asn1_output_put (struct asn1_output *o, int a)
{
	int ret;

	assert (o != NULL);
	assert (o->tail < (o->buffer + ASN1_BUFFER_SIZE));

	*o->tail++ = a;

	if (o->tail < (o->buffer + ASN1_BUFFER_SIZE))
		return a;

	if ((ret = asn1_output_write (o)) < 0)
		return ret;

	return a;
}

long asn1_output_puts (struct asn1_input *o, const void *from, long count)
{
	// todo: implement it
}
