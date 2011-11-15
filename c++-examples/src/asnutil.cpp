#include "asnutil.h"

using namespace SNACC;

AsnLen DecTagLen (unsigned char *bytes)
{
    FUNC("BDecLen()");
    AsnLen  len;
    int  lenBytes;

    if (bytes[0] < 128)   /* short length */
        return bytes[0];

    else if (bytes[0] == (unsigned char) 0x080)  /* indef len indicator */
        return INDEFINITE_LEN;

    else  /* long len form */
    {
        /*
         * strip high bit to get # bytes left in len
         */
        lenBytes = bytes[0] & (unsigned char) 0x7f;

        if (lenBytes > sizeof (long int))
        {
           throw BoundsException("length overflow", STACK_ENTRY);
        }

        for (len = 0; lenBytes > 0; lenBytes--)
            len = (len << 8) | (unsigned long int) bytes[1];

        return len;
    }
    /* not reached */
}

void hex2str(const char *pszHex, std::string &str)
{
   FUNC("hex2str()");

   bool error = false;
   int  nInt; // int. which has an ascii val. of a character
   unsigned char *puchIn; // temp pointer to hex

    // if this is hex it should be divisible by two.  if not return an error.
    if (strlen(pszHex) % 2 != 0)
       error = true;
   
    if (!error)
    {
       puchIn = (unsigned char *)pszHex;
       while (*puchIn != '\0' && !error)
       {
          if (sscanf((char*)puchIn, "%2x", &nInt) > 0)
          {
             str += (char) nInt;
             puchIn += 2;
          }
          else
             error = true;
       }
    }
    if (error)
       throw EXCEPT("invalid hex string",-1);
}

void fillTestBuffer(unsigned char *test, long length)
{
   int n = 0;
   for (int i = 0; i < length; i++)
   {
      if (n == 219)
         n = 0;
      test[i] = n;
      n++;
   }
}

bool checkTestBuffer(const unsigned char *test, long length)
{
   int n = 0;
   for (int x=0; x < length; x++)
   {
      if (n == 219)
         n = 0;
      if (test[x] != n)
      {
         std::cout << "Died at " << x << std::endl;
         return false;
      }
      n++;
   }
   return true;
}

