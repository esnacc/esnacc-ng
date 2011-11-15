#ifndef _asn_UTF8String_h_
#define _asn_UTF8String_h_


#ifdef __cplusplus
extern "C" {
#endif

typedef AsnOcts UTF8String; /* [UNIVERSAL 12] IMPLICIT OCTET STRING */

AsnLen BEncUTF8String PROTO ((GenBuf *b, UTF8String *v));
AsnLen BEncUTF8StringContent PROTO ((GenBuf *b, UTF8String *octs));

void BDecUTF8String PROTO ((GenBuf *b, UTF8String *result, AsnLen *bytesDecoded, ENV_TYPE env));
void BDecUTF8StringContent PROTO ((GenBuf *b, AsnTag tagId, AsnLen len, UTF8String *result, AsnLen *bytesDecoded, ENV_TYPE env));

#define PrintUTF8String PrintAsnOcts

#define FreeUTF8String FreeAsnOcts 

int CvtUTF8String2wchar(UTF8String *inOcts, wchar_t **outStr);
int CvtUTF8towchar(char *utf8Str, wchar_t **outStr);
int CvtWchar2UTF8(wchar_t *inStr, char **utf8Str);


#ifdef __cplusplus
}
#endif

#endif
