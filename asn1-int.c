/*
 * ASN.1 Integer
 *
 * Copyright (c) 2016-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <errno.h>
#include <stdlib.h>

#include <asn1-mini/asn1-int.h>

int asn1_int_init (struct asn1_int *i, int count)
{
	i->count = count;

	errno = 0;

	if ((i->n = malloc (count * sizeof (*i->n))) == NULL)
		return -errno;

	return 0;
}

void asn1_int_fini (struct asn1_int *i)
{
	free (i->n);
}

void asn1_int_normalize (struct asn1_int *i)
{
	for (; i->count > 0 && i->n[i->count - 1] == 0; --i->count) {}
}
