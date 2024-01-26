/*
 * ASN.1 DER encoding helpers
 *
 * Copyright (c) 2016-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <asn1-mini/ber-bool.h>
#include <asn1-mini/ber-int.h>
#include <asn1-mini/ber-number.h>
#include <asn1-mini/ber-time.h>

enum {
	ASN1_EOC	= 0x00,
	ASN1_BOOL	= 0x01,
	ASN1_INT	= 0x02,
	ASN1_BITS	= 0x03,
	ASN1_BLOB	= 0x04,
	ASN1_NULL	= 0x05,
	ASN1_OID	= 0x06,
	ASN1_UTF8	= 0x0c,
	ASN1_SEQ	= 0x10,
	ASN1_SET	= 0x11,
	ASN1_NUMSTR	= 0x12,
	ASN1_PRINT	= 0x13,
	ASN1_ASCII	= 0x16,
	ASN1_UTCTIME	= 0x17,
	ASN1_GENTIME	= 0x18,
};

static const char *ber_get_type_name (int type)
{
	switch (type & 0x1f) {
	case ASN1_BOOL:		return "boolean";
	case ASN1_INT:		return "integer";
	case ASN1_BITS:		return "bits";
	case ASN1_BLOB:		return "blob";
	case ASN1_NULL:		return "null";
	case ASN1_OID:		return "OID";
	case ASN1_UTF8:		return "UTF8 string";
	case ASN1_SEQ:		return "list";
	case ASN1_SET:		return "set";
	case ASN1_NUMSTR:	return "numeric string";
	case ASN1_PRINT:	return "printable string";
	case ASN1_ASCII:	return "ASCII string";
	case ASN1_UTCTIME:	return "UTC time";
	case ASN1_GENTIME:	return "generic time";
	}

	return NULL;
}

static int ber_dump_bool (struct ber_input *o)
{
	int v;

	if ((v = ber_get_bool (o)) < 0)
		return v;

	printf (v != 0 ? "true\n" : "false\n");
	return 0;
}

static void asn1_dump_int (struct asn1_int *i)
{
	asn1_limb_t *p;

	asn1_int_normalize (i);

	if (i->count == 0) {
		printf ("0");
		return;
	}

	p = i->n + i->count - 1;
	printf ("%llx", (unsigned long long) *p);

	for (--p; p >= i->n; --p)
		printf ("%016llx", (unsigned long long) *p);
}

static int ber_dump_int (struct ber_input *o)
{
	int ret;
	struct asn1_int i;

	if ((ret = ber_get_int (o, &i)) < 0)
		return ret;

	asn1_dump_int (&i);

	asn1_int_fini (&i);
	putchar ('\n');
	return 0;
}

static int ber_dump_blob (struct ber_input *o)
{
	int a;

	while (o->len > 0)
		if ((a = ber_get (o)) < 0)
			return a;
		else
			printf ("%02x ", a);

	putchar ('\n');
	return 0;
}

static int ber_dump_oid (struct ber_input *o)
{
	int a;
	long n;

	if ((a = ber_get (o)) < 0)
		return a;

	if (a < 80)
		printf ("%d.%d", a / 40, a % 40);
	else
		printf ("2.%d", a - 80);

	while (o->len > 0)
		if ((n = ber_get_number (o)) < 0)
			return n;
		else
			printf (".%ld", n);

	putchar ('\n');
	return 0;
}

static int ber_dump_string (struct ber_input *o)
{
	int a;

	while (o->len > 0)
		if ((a = ber_get (o)) < 0)
			return a;
		else
			putchar (a);

	putchar ('\n');
	return 0;
}

static int ber_dump_time (struct ber_input *o, int ext)
{
	int ret;
	struct asn1_time t;

	ret = ext ? ber_get_gentime (o, &t) : ber_get_utctime (o, &t);
	if (ret < 0)
		return ret;

	printf ("%4u-%02u-%02u %02u:%02u:%02u %+03d%02d\n",
		t.year, t.mon, t.day,
		t.hour, t.min, t.sec,
		t.shift / 60, abs (t.shift) % 60);

	return 0;
}

static void indent (int level)
{
	for (; level > 0; --level)
		printf ("    ");
}

static long ber_tag_number (long tag)
{
	int base = tag & 0x1f;
	long lo, hi;

	if (base != 0x1f)
		return base;

	tag >>= 8;

	if ((tag & 0x80) == 0)
		return tag;

	lo = tag &  0x7f;
	hi = tag & ~0xff;

	tag = (hi >> 1) | lo;

	if ((tag & 0x4000) == 0)
		return tag;

	lo = tag &  0x3fff;
	hi = tag & ~0x7fff;

	tag = (hi >> 1) | lo;

	return tag;
}

static void ber_dump_prefix (int level, long tag, long len)
{
	const char *name = NULL;

	indent (level);

	switch (tag & 0xc0) {
	case 0x00:
		if ((name = ber_get_type_name (tag)) != NULL)
			printf ("%s ", name);
		break;
	case 0x40:	printf ("application ");      break;
	case 0x80:	printf ("context-specific "); break;
	case 0xc0:	printf ("private");           break;
	}

	if (name == NULL)
		printf ("%ld ", ber_tag_number (tag));

	if (len > 0)
		printf ("[%ld] ", len);
	else if (len < 0)
		printf ("[] ");
}

static int ber_dump (struct ber_input *o, int level);

static int ber_dump_constructed (struct ber_input *o, int level)
{
	int ret;

	printf ("{\n");

	while (o->len > 0)
		if ((ret = ber_dump (o, level + 1)) < 0)
			return ret;

	indent (level); printf ("}\n");
	return 0;
}

static int ber_dump_primitive (struct ber_input *o, long tag)
{
	if (o->len == 0) {
		putchar ('\n');
		return 0;
	}

	printf ("= ");

	switch (tag) {
	case ASN1_BOOL:		return ber_dump_bool   (o);
	case ASN1_INT:		return ber_dump_int    (o);
	case ASN1_UTF8:
	case ASN1_NUMSTR:
	case ASN1_PRINT:
	case ASN1_ASCII:	return ber_dump_string (o);
	case ASN1_UTCTIME:	return ber_dump_time   (o, 0);
	case ASN1_GENTIME:	return ber_dump_time   (o, 1);
	case ASN1_OID:		return ber_dump_oid    (o);
	}

	return ber_dump_blob (o);
}

static int ber_dump (struct ber_input *o, int level)
{
	long tag, len;
	struct ber_input box;

	if ((tag = ber_get_tag (o)) < 0)
		return tag;

	if ((len = ber_get_len (o)) < 0)
		return len;

	if (len > o->len)
		return -ENODATA;

	if (tag == ASN1_EOC)  /* end of content */
		return ber_skip (o);

	box.in  = o->in;
	box.len = len;

	if (o->len != BER_LEN_INDEFINITE)
		o->len -= len;

	ber_dump_prefix (level, tag, len);

	if ((tag & 0x20) != 0)
		return ber_dump_constructed (&box, level);

	return ber_dump_primitive (&box, tag);
}

#include <stddef.h>
#include <unistd.h>

static int reader (void *buffer, int size, void *ctx)
{
	int fd = ctx - NULL, ret;

	if ((ret = read (fd, buffer, size)) < 0)
		return -errno;

	return ret;
}

int main (int argc, char *argv[])
{
	struct asn1_input *in;
	struct ber_input o;

	if ((in = asn1_input_open (reader, (void *) 0)) == NULL)
		goto no_input;

	o.in  = in;
	o.len = BER_LEN_INDEFINITE;

	errno = -ber_dump (&o, 0);
	asn1_input_close (in);

	if (errno > 0)
		goto no_dump;

	return 0;
no_input:
	perror ("stdin");
	return 1;
no_dump:
	perror ("BER dump");
	return 1;
}
