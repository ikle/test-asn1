/*
 * ASN.1 input helpers
 */

#ifndef _ASN1_INPUT_H
#define _ASN1_INPUT_H  1

struct asn1_input *asn1_input_open (
	int (*read) (void *buffer, int size, void *ctx), void *ctx);
void asn1_input_close (struct asn1_input *o);

int asn1_input_peek (struct asn1_input *o);
int asn1_input_get  (struct asn1_input *o);
int asn1_input_gets (struct asn1_input *o, void *to, long count);

#endif  /* _ASN1_INPUT_H */
