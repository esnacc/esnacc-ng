#ifndef _asn_IA5String_h_
#define _asn_IA5String_h_

#ifdef __cplusplus
extern "C" {
#endif


typedef AsnOcts IA5String; /* [UNIVERSAL 22] IMPLICIT OCTET STRING */

AsnLen BEncIA5String PROTO ((GenBuf *b, IA5String *v));
AsnLen BEncIA5StringContent PROTO ((GenBuf *b, IA5String *octs));

void BDecIA5String PROTO ((GenBuf *b, IA5String *result, AsnLen *bytesDecoded, ENV_TYPE env));
void BDecIA5StringContent PROTO ((GenBuf *b, AsnTag tagId, AsnLen len, IA5String *result, AsnLen *bytesDecoded, ENV_TYPE env));

#define PrintIA5String PrintAsnOcts

#define FreeIA5String FreeAsnOcts

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif


