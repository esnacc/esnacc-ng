#ifndef _asn_UniversalString_h_
#define _asn_UniversalString_h_


#ifdef __cplusplus
extern "C" {
#endif

typedef AsnOcts UniversalString; /* [UNIVERSAL 28] IMPLICIT OCTET STRING */

AsnLen BEncUniversalString PROTO ((GenBuf *b, UniversalString *v));
AsnLen BEncUniversalStringContent PROTO ((GenBuf *b, UniversalString *octs));

void BDecUniversalString PROTO ((GenBuf *b, UniversalString *result, AsnLen *bytesDecoded, ENV_TYPE env));
void BDecUniversalStringContent PROTO ((GenBuf *b, AsnTag tagId, AsnLen len, UniversalString *result, AsnLen *bytesDecoded, ENV_TYPE env));

#define PrintUniversalString PrintAsnOcts

#define FreeUniversalString FreeAsnOcts 

int CvtUniversalString2wchar(UniversalString *inOcts, wchar_t **outStr);


#ifdef __cplusplus
}
#endif

#endif
