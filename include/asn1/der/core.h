/*
 * ASN.1 DER Core
 *
 * Copyright (c) 2016-2024 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef ASN1_DER_CORE_H
#define ASN1_DER_CORE_H  1

#include <stddef.h>
#include <stdint.h>

struct der_window {
	unsigned char *head, *tail;
};

#endif  /* ASN1_DER_CORE_H */
