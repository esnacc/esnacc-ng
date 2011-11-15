#include "asn-incl.h"

using namespace SNACC;


EXTERNALChoice::EXTERNALChoice(const EXTERNALChoice &that)
{
   Init();
   *this = that;
}
void EXTERNALChoice::Init(void)
{
   // initialize choice to no choice
   choiceId = NoChoiceCid;
   NoChoice = NULL;
};
void EXTERNALChoice::Clear()
{
  switch (choiceId)
  {
    case single_ASN1_typeCid:
      delete single_ASN1_type;
      single_ASN1_type = NULL;
      break;
    case octet_alignedCid:
      delete octet_aligned;
      octet_aligned = NULL;
      break;
    case arbitraryCid:
      delete arbitrary;
      arbitrary = NULL;
      break;
    default:
      arbitrary = NULL;
  } // end of switch
} // end of Clear

EXTERNALChoice &EXTERNALChoice::operator = (const EXTERNALChoice &that)
{
  if (this != &that)
  {
    Clear();

    switch (choiceId = that.choiceId)
    {
      case single_ASN1_typeCid:
        single_ASN1_type = new AsnOcts;
        *single_ASN1_type = *that.single_ASN1_type;
        break;
      case octet_alignedCid:
        octet_aligned = new AsnOcts;
        *octet_aligned = *that.octet_aligned;
        break;
      case arbitraryCid:
        arbitrary = new AsnBits;
        *arbitrary = *that.arbitrary;
        break;
      default:
	arbitrary = NULL;
    }
  }

  return *this;
}

AsnLen
EXTERNALChoice::BEncContent (AsnBuf & b) const
{
  FUNC("EXTERNALChoice::BEncContent");

  AsnLen l=0;
  switch (choiceId)
  {
    case single_ASN1_typeCid:
      l = single_ASN1_type->BEncContent (b);
      l += BEncDefLen (b, l);
      l += BEncTag1 (b, UNIV, PRIM, OCTETSTRING_TAG_CODE);
      l += BEncConsLen (b, l);
      l += BEncTag1 (b, CNTX, CONS, 0);
      break;

    case octet_alignedCid:
      l = octet_aligned->BEncContent (b);
      l += BEncDefLen (b, l);
      l += BEncTag1 (b, CNTX, PRIM, 1);
      break;

    case arbitraryCid:
      l = arbitrary->BEncContent (b);
      l += BEncDefLen (b, l);
      l += BEncTag1 (b, CNTX, PRIM, 2);
      break;

    default:
      throw EXCEPT("Can not encode non optional empty CHOICE", ENCODE_ERROR);

  } // end switch
  return l;
} // EXTERNALChoice::BEncContent


void EXTERNALChoice::BDecContent (const AsnBuf & b, AsnTag tag, AsnLen elmtLen0, AsnLen &bytesDecoded/*, s env*/)
{
   FUNC("EXTERNALChoice::BDecContent()");

   Clear();
   AsnLen elmtLen1;
   switch (tag)
   {
      case MAKE_TAG_ID (CNTX, CONS, 0):
         tag = BDecTag (b, bytesDecoded);
         if ((tag != MAKE_TAG_ID (UNIV, PRIM, OCTETSTRING_TAG_CODE))
             && (tag != MAKE_TAG_ID (UNIV, CONS, OCTETSTRING_TAG_CODE)))
         {
            throw InvalidTagException(typeName(), tag, STACK_ENTRY);
         }

         elmtLen1 = BDecLen (b, bytesDecoded);
         choiceId = single_ASN1_typeCid;
         single_ASN1_type = new AsnOcts;
         single_ASN1_type->BDecContent (b, tag, elmtLen1, bytesDecoded);
         if (elmtLen0 == INDEFINITE_LEN)
           BDecEoc (b, bytesDecoded);
         break;

      case MAKE_TAG_ID (CNTX, PRIM, 1):
      case MAKE_TAG_ID (CNTX, CONS, 1):
         choiceId = octet_alignedCid;
         octet_aligned = new AsnOcts;
         octet_aligned->BDecContent (b, tag, elmtLen0, bytesDecoded);
         break;

      case MAKE_TAG_ID (CNTX, PRIM, 2):
      case MAKE_TAG_ID (CNTX, CONS, 2):
         choiceId = arbitraryCid;
         arbitrary = new AsnBits;
         arbitrary->BDecContent (b, tag, elmtLen0, bytesDecoded);
         break;

      default:
         throw InvalidTagException(typeName(), tag, STACK_ENTRY);
      break;
      } // end switch
} // EXTERNALChoice::BDecContent

AsnLen EXTERNALChoice::PEnc (AsnBufBits &b) const
{
	AsnLen len=0;


	return len;
}

AsnLen EXTERNALChoice::BEnc (AsnBuf & b) const
{
    AsnLen l=0;
    l = BEncContent (b);
    return l;
}

void EXTERNALChoice::BDec (const AsnBuf & b, AsnLen &bytesDecoded/*, s env*/)
{
    AsnLen elmtLen;
    AsnTag tag;

    /*  CHOICEs are a special case - grab identifying tag */
    /*  this allows easier handling of nested CHOICEs */
    tag = BDecTag (b, bytesDecoded);
    elmtLen = BDecLen (b, bytesDecoded);
    BDecContent (b, tag, elmtLen, bytesDecoded);
}


void EXTERNALChoice::Print(std::ostream& os, unsigned short indent) const
{
	FUNC("EXTERNALChoice::Print");
	
	switch (choiceId)
	{
    case single_ASN1_typeCid:
		os << "single-ASN1-type ";
		if (single_ASN1_type)
			single_ASN1_type->Print(os, indent);
		else
			os << "-- void3 --\n";
		break;
		
    case octet_alignedCid:
		os << "octet-aligned ";
		if (octet_aligned)
			octet_aligned->Print(os, indent);
		else
			os << "-- void3 --\n";
		break;
		
    case arbitraryCid:
		os << "arbitrary ";
		if (arbitrary)
			arbitrary->Print(os, indent);
		else
			os << "-- void3 --\n";
		break;
		
    default:
		throw EXCEPT("Can not encode non optional empty CHOICE", ENCODE_ERROR);
	} // end of switch
} // EXTERNALChoice::Print

void EXTERNALChoice::PrintXML (std::ostream &os, const char *lpszTitle) const
{
  os << "<EXTERNALChoice-CHOICE>";
  if (lpszTitle)
    os << lpszTitle;
  os << "-" << std::endl;
  switch (choiceId)
  {
    case single_ASN1_typeCid:
      if (single_ASN1_type)
        single_ASN1_type->PrintXML(os,"single-ASN1-type");
      else
      {
        os << "<single-ASN1-type>";
        os << "-- void3 --</single-ASN1-type>" << std::endl;
      }
      break;

    case octet_alignedCid:
      if (octet_aligned)
        octet_aligned->PrintXML(os,"octet-aligned");
      else
      {
        os << "<octet-aligned>";
        os << "-- void3 --</octet-aligned>" << std::endl;
      }
      break;

    case arbitraryCid:
      if (arbitrary)
        arbitrary->PrintXML(os,"arbitrary");
      else
      {
        os << "<arbitrary>";
        os << "-- void3 --</arbitrary>" << std::endl;
      }
      break;
  default:
    break;
  } // end of switch
  os << "</EXTERNALChoice-CHOICE>" << std::endl;
} // EXTERNALChoice::PrintXML

void EXTERNAL::Init(void)
{
  direct_reference = NULL;
  indirect_reference = NULL;
  data_value_descriptor = NULL;
#if TCL
  encoding = new EXTERNALChoice;
#else
  encoding = NULL; // incomplete initialization of mandatory element!
#endif // TCL
}

void EXTERNAL::Clear()
{
  delete direct_reference;
  direct_reference = NULL;
  delete indirect_reference;
  indirect_reference = NULL;
  delete data_value_descriptor;
  data_value_descriptor = NULL;
  delete encoding;
  encoding = NULL;
}

EXTERNAL::EXTERNAL(const EXTERNAL &that)
{
   Init();
   *this = that;
}

EXTERNAL &EXTERNAL::operator = (const EXTERNAL &that)
{
  if (this != &that)
  {
    Clear();
    if (that.direct_reference)
    {
      if (!direct_reference)
        direct_reference = new AsnOid;
      *direct_reference = *that.direct_reference;
    }
    else
    {
      delete direct_reference;
      direct_reference = NULL;
    }
    if (that.indirect_reference)
    {
      if (!indirect_reference)
        indirect_reference = new AsnInt;
      *indirect_reference = *that.indirect_reference;
    }
    else
    {
      delete indirect_reference;
      indirect_reference = NULL;
    }
    if (that.data_value_descriptor)
    {
      if (!data_value_descriptor)
        data_value_descriptor = new ObjectDescriptor;
      *data_value_descriptor = *that.data_value_descriptor;
    }
    else
    {
      delete data_value_descriptor;
      data_value_descriptor = NULL;
    }
    if (that.encoding)
    {
      if (!encoding)
        encoding = new EXTERNALChoice;
      *encoding = *that.encoding;
    }
    else
    {
      delete encoding;
      encoding = NULL;
    }
  }

  return *this;
}

AsnLen
EXTERNAL::BEncContent (AsnBuf & b) const
{
  AsnLen totalLen = 0;
  AsnLen l=0;

    l = encoding->BEncContent (b);
    totalLen += l;

  if (NOT_NULL (data_value_descriptor))
  {
    l = data_value_descriptor->BEncContent (b);
    l += BEncDefLen (b, l);

    l += BEncTag1 (b, UNIV, PRIM, OD_TAG_CODE);
    totalLen += l;
  }

  if (NOT_NULL (indirect_reference))
  {
    l = indirect_reference->BEncContent (b);
    l += BEncDefLen (b, l);

    l += BEncTag1 (b, UNIV, PRIM, INTEGER_TAG_CODE);
    totalLen += l;
  }

  if (NOT_NULL (direct_reference))
  {
    l = direct_reference->BEncContent (b);
    l += BEncDefLen (b, l);

    l += BEncTag1 (b, UNIV, PRIM, OID_TAG_CODE);
    totalLen += l;
  }

  return totalLen;
} // EXTERNAL::BEncContent


void EXTERNAL::BDecContent (const AsnBuf & b, AsnTag /*tag0*/, AsnLen elmtLen0, AsnLen &bytesDecoded/*, s env*/)
{
   FUNC("EXTERNAL::BDecContent()");

   Clear();
   AsnTag tag1;
   AsnLen seqBytesDecoded = 0;
   AsnLen elmtLen1;
   tag1 = BDecTag (b, seqBytesDecoded);

   if ((tag1 == MAKE_TAG_ID (UNIV, PRIM, OID_TAG_CODE)))
   {
      elmtLen1 = BDecLen (b, seqBytesDecoded);
      direct_reference = new AsnOid;
      direct_reference->BDecContent (b, tag1, elmtLen1, seqBytesDecoded);
      tag1 = BDecTag (b, seqBytesDecoded);
   }

   if ((tag1 == MAKE_TAG_ID (UNIV, PRIM, INTEGER_TAG_CODE)))
   {
      elmtLen1 = BDecLen (b, seqBytesDecoded);
      indirect_reference = new AsnInt;
      indirect_reference->BDecContent (b, tag1, elmtLen1, seqBytesDecoded);
      tag1 = BDecTag (b, seqBytesDecoded);
   }

   if ((tag1 == MAKE_TAG_ID (UNIV, PRIM, OD_TAG_CODE))
    || (tag1 == MAKE_TAG_ID (UNIV, CONS, OD_TAG_CODE)))
   {
      elmtLen1 = BDecLen (b, seqBytesDecoded);
      data_value_descriptor = new ObjectDescriptor;
      data_value_descriptor->BDecContent (b, tag1, elmtLen1, seqBytesDecoded);
      tag1 = BDecTag (b, seqBytesDecoded);
   }

   if ((tag1 == MAKE_TAG_ID (CNTX, CONS, 0))
    || (tag1 == MAKE_TAG_ID (CNTX, PRIM, 1))
    || (tag1 == MAKE_TAG_ID (CNTX, CONS, 1))
    || (tag1 == MAKE_TAG_ID (CNTX, PRIM, 2))
    || (tag1 == MAKE_TAG_ID (CNTX, CONS, 2)))
   {
      elmtLen1 = BDecLen (b, seqBytesDecoded);
      encoding = new EXTERNALChoice;
      encoding->BDecContent (b, tag1, elmtLen1, seqBytesDecoded);
   }
   else
   {
      throw EXCEPT("SEQUENCE is missing non-optional elmt", DECODE_ERROR);
   }

   bytesDecoded += seqBytesDecoded;
   if (elmtLen0 == INDEFINITE_LEN)
   {
      BDecEoc (b, bytesDecoded);
      return;
   }
   else if (seqBytesDecoded != elmtLen0)
   {
      throw EXCEPT("ERROR - Length discrepancy on sequence", DECODE_ERROR);
   }
   else
      return;
} // EXTERNAL::BDecContent

AsnLen EXTERNAL::PEnc (AsnBufBits &b) const
{
	AsnLen len=0;


	return len;
}

AsnLen EXTERNAL::BEnc (AsnBuf & b) const
{
  AsnLen l=0;
  l = BEncContent (b);
  l += BEncConsLen (b, l);
  l += BEncTag1 (b, UNIV, CONS, EXTERNAL_TAG_CODE);
  return l;
}

void EXTERNAL::BDec (const AsnBuf & b, AsnLen &bytesDecoded/*, s env*/)
{
   FUNC("EXTERNAL::BDec()");

   AsnTag tag;
   AsnLen elmtLen1;

   if ((tag = BDecTag (b, bytesDecoded)) != MAKE_TAG_ID (UNIV, CONS, EXTERNAL_TAG_CODE))
   {
      throw InvalidTagException(typeName(), tag, STACK_ENTRY);
   }
   elmtLen1 = BDecLen (b, bytesDecoded);
   BDecContent (b, tag, elmtLen1, bytesDecoded);
}


void EXTERNAL::Print(std::ostream& os, unsigned short indent) const
{
	os << "{ -- SEQUENCE --" << std::endl;
	++indent;

	if (NOT_NULL (direct_reference))
	{
		Indent(os, indent);
		os << "direct-reference ";
		direct_reference->Print(os, indent);
	}
	else
	{
		Indent(os, indent);
		os << "direct-reference ";
		os << "-- void --";
		os << "," << std::endl;
	}
	
	if (NOT_NULL (indirect_reference))
	{
		Indent(os, indent);
		os << "indirect-reference ";
		indirect_reference->Print(os, indent);
	}
	else
	{
		Indent(os, indent);
		os << "indirect-reference ";
		os << "-- void --";
		os << "," << std::endl;
	}
	
	if (NOT_NULL (data_value_descriptor))
	{
		Indent(os, indent);
		os << "data-value-descriptor ";
		data_value_descriptor->Print(os, indent);
	}
	else
	{
		Indent(os, indent);
		os << "data-value-descriptor ";
		os << "-- void --";
		os << "," << std::endl;
	}
	
	if (NOT_NULL (encoding))
	{
		Indent(os, indent);
		os << "encoding ";
		os << *encoding;
	}
	else
	{
		Indent(os, indent);
		os << "encoding ";
		os << "-- void --";
		os << std::endl;
	}
	
	os << std::endl;
	Indent (os, --indent);
	os << "}";
} // EXTERNAL::Print


void EXTERNAL::PrintXML (std::ostream &os, 
                   const char *lpszTitle) const
{
  if (lpszTitle)
  {
     os << "<" << lpszTitle << ">" << std::endl;
     os << typeName() << " SEQUENCE" << std::endl ;  }
  else
  {
     os << "<" << typeName() << "> SEQUENCE" << std::endl;
  }
  if (NOT_NULL (direct_reference))
  {
    direct_reference->PrintXML(os, "direct-reference");
  }
  else
    os << "<direct-reference>-- void --</direct-reference>" << std::endl;

  if (NOT_NULL (indirect_reference))
  {
    indirect_reference->PrintXML(os, "indirect-reference");
  }
  else
    os << "<indirect-reference>-- void --</indirect-reference>" << std::endl;

  if (NOT_NULL (data_value_descriptor))
  {
    data_value_descriptor->PrintXML(os, "data-value-descriptor");
  }
  else
    os << "<data-value-descriptor>-- void --</data-value-descriptor>" << std::endl;

  if (NOT_NULL (encoding))
  {
    encoding->PrintXML(os, "encoding");
  }
  else
    os << "<encoding>-- void --</encoding>" << std::endl;

  if (lpszTitle)
  {
     os << "</" << lpszTitle << ">" << std::endl;
  }
  else
     os << "</" << typeName() << ">" << std::endl;
} // EXTERNAL::PrintXML

