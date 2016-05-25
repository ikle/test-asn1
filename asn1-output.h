/*
 * ASN.1 output helpers
 */

#ifndef _ASN1_OUTPUT_H
#define _ASN1_OUTPUT_H  1

struct asn1_output *asn1_output_open (
	int (*write) (const void *buffer, int size, void *ctx), void *ctx);
int asn1_output_flush (struct asn1_output *o);
int asn1_output_close (struct asn1_output *o);

int asn1_output_put (struct asn1_output *o, int a);

#endif  /* _ASN1_OUTPUT_H */
