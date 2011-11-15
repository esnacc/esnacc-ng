#ifndef _asn_TeletexString_h_
#define _asn_TeletexString_h_

#ifdef __cplusplus
extern "C" {
#endif 

typedef AsnOcts TeletexString; /* [UNIVERSAL 20] IMPLICIT OCTET STRING */

AsnLen BEncTeletexString PROTO ((GenBuf *b, TeletexString *v));

void BDecTeletexString PROTO ((GenBuf *b, TeletexString *result, AsnLen *bytesDecoded, ENV_TYPE env));
AsnLen BEncTeletexStringContent PROTO ((GenBuf *b, AsnOcts *octs));

void BDecTeletexStringContent PROTO ((GenBuf *b, AsnTag tagId, AsnLen len, AsnOcts *result, AsnLen *bytesDecoded, ENV_TYPE env));

#define PrintTeletexString PrintAsnOcts

#define FreeTeletexString FreeAsnOcts 

#ifdef __cplusplus 
}
#endif

#endif
