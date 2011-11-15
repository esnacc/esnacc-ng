
#include "asn-incl.h"

_BEGIN_SNACC_NAMESPACE

AsnLen AsnExtension::BEnc(AsnBuf& b) const
{
	AsnLen l = 0;
	std::list<AsnAny>::const_reverse_iterator i;
    for (i = extList.rbegin(); i != extList.rend(); ++i)
		l += i->BEnc(b);
	
	return l;
}



AsnLen AsnExtension::PEnc(AsnBufBits& b) const
{
	AsnLen l = 0;
	
    std::list<AsnAny>::const_iterator i;
	for (i = extList.begin(); i != extList.end(); ++i)
		l += i->PEnc(b);
	
    return l;
}


void AsnExtension::Print(std::ostream& os, unsigned short indent) const
{
    std::list<AsnAny>::const_iterator i;
	for (i = extList.begin(); i != extList.end(); ++i)
    {
	    i->Print(os, indent);
        os<<"\n";
        Indent(os, indent);

    }
}

_END_SNACC_NAMESPACE

