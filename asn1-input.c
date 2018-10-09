/*
 * ASN.1 input helpers
 */

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "asn1-input.h"

struct asn1_input {
	unsigned char *buffer;
	int (*read) (void *buffer, int size, void *ctx);
	void *ctx;
	const unsigned char *head, *tail;
};

static size_t asn1_input_available (struct asn1_input *o)
{
	assert (o->tail >= o->head);

	return o->tail - o->head;
}

#define ASN1_BUFFER_SIZE  4096

struct asn1_input *asn1_input_open (
	int (*read) (void *buffer, int size, void *ctx), void *ctx)
{
	struct asn1_input *o;

	assert (read != NULL);

	if ((o = malloc (sizeof (*o))) == NULL)
		return NULL;

	if ((o->buffer = malloc (ASN1_BUFFER_SIZE)) == NULL)
		goto no_buffer;

	o->read = read;
	o->ctx  = ctx;
	o->tail = o->head = o->buffer;

	return o;
no_buffer:
	free (o);
	return NULL;
}

void asn1_input_close (struct asn1_input *o)
{
	if (o == NULL)
		return;

	free (o->buffer);
	free (o);
}

static int asn1_input_fetch (struct asn1_input *o)
{
	int ret;

	assert (o != NULL);
	assert (asn1_input_available (o) == 0);

	if ((ret = o->read (o->buffer, ASN1_BUFFER_SIZE, o->ctx)) < 0)
		return ret;

	if (ret == 0)
		return -ENODATA;

	assert (ret <= ASN1_BUFFER_SIZE);

	o->head = o->buffer;
	o->tail = o->buffer + ret;

	return ret;
}

int asn1_input_peek (struct asn1_input *o)
{
	int ret;

	assert (o != NULL);

	if (asn1_input_available (o) == 0 &&
	    (ret = asn1_input_fetch (o)) < 0)
		return ret;

	return *o->head;
}

int asn1_input_get (struct asn1_input *o)
{
	int a;

	assert (o != NULL);

	if ((a = asn1_input_peek (o)) >= 0)
		++o->head;

	return a;
}

static int asn1_input_read (struct asn1_input *o, void *to, size_t count)
{
	int ret;

	if (asn1_input_available (o) == 0 &&
	    (ret = asn1_input_fetch (o)) < 0)
		return ret;

	if (count > asn1_input_available (o))
		count = asn1_input_available (o);

	memcpy (to, o->head, count);
	o->head += count;

	return count;
}

long asn1_input_gets (struct asn1_input *o, void *to, long count)
{
	long total;
	char *p;
	int ret;

	for (total = count, p = to; count > 0; p += ret, count -= ret)
		if ((ret = asn1_input_read (o, p, count)) < 0)
			return ret;

	return total;
}
