/*
 * ASN.1 integer
 */

#include <errno.h>
#include <stdlib.h>

#include "asn1-int.h"

int asn1_int_init (struct asn1_int *i, int count)
{
	if ((i->n = malloc (count * sizeof (*i->n))) == NULL)
		return -errno;

	i->count = count;
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
