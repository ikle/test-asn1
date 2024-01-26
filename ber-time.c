/*
 * ASN.1 Time helpers
 *
 * Copyright (c) 2016-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <errno.h>

#include <asn1-mini/ber-time.h>

static int ber_is_digit (int a)
{
	return a >= 0x30 && a <= 0x39;
}

static int ber_get_ascii_number (struct ber_input *o, unsigned count)
{
	int a, n;

	for (n = 0; count > 0; --count)
		if ((a = ber_get (o)) < 0)
			return a;
		else if (ber_is_digit (a))
			n = n * 10 + (a - 0x30);
		else
			return -EINVAL;

	return n;
}

static int ber_get_timezone (struct ber_input *o, struct asn1_time *t)
{
	int a, h, m;

	if ((a = ber_peek (o)) < 0)
		return a;

	switch (a) {
	case 0x5a:  /* Z */
		t->shift = 0;
		break;
	case 0x2b:  /* + */
		t->shift = 1;
		break;
	case 0x2d:  /* - */
		t->shift = -1;
		break;
	default:
		return 0;
	}

	(void) ber_get (o);

	if (t->shift == 0)
		return 1;

	if ((h = ber_get_ascii_number (o, 2)) < 0)
		return h;

	if ((m = ber_get_ascii_number (o, 2)) < 0)
		return m;

	t->shift *= h * 60 + m;
	return 1;
}

static int ber_get_time (struct ber_input *o, struct asn1_time *t)
{
	int ret;

	if ((t->year = ber_get_ascii_number (o, 2)) < 0)
		return t->year;

	if ((t->mon  = ber_get_ascii_number (o, 2)) < 0)
		return t->mon;

	if ((t->day  = ber_get_ascii_number (o, 2)) < 0)
		return t->day;

	if ((t->hour = ber_get_ascii_number (o, 2)) < 0)
		return t->hour;

	if ((t->min  = ber_get_ascii_number (o, 2)) < 0)
		return t->min;

	t->sec = 0;

	if ((ret = ber_get_timezone (o, t)) != 0)
		return ret;

	if ((t->sec = ber_get_ascii_number (o, 2)) < 0)
		return t->sec;

	return ber_get_timezone (o, t);
}

int ber_get_utctime (struct ber_input *o, struct asn1_time *t)
{
	int ret;

	if ((ret = ber_get_time (o, t)) < 0)
		return ret;

	t->year += (t->year > 50) ? 1900 : 2000;

	return 0;
}

int ber_get_gentime (struct ber_input *o, struct asn1_time *t)
{
	int century, ret;

	if ((century = ber_get_ascii_number (o, 2)) < 0)
		return century;

	if ((ret = ber_get_time (o, t)) < 0)
		return ret;

	t->year += century * 100;

	return 0;
}
