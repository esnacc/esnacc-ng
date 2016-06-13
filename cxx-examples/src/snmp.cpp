#include "asnutil.h"

#include "rfc1155-smi.h"
#include "rfc1157-snmp.h"
#include <fstream>
using namespace SNACC;

/*
 * FUNCTION: test_snmp_file()
 *
 * Attempt to decode the SNMP file and indicate success or failure to std::cout
 */
void test_snmp_file(char *pszfn)
{
   FUNC("test_snmp_file");

   Message snmpObject;

   AsnBuf inputBuf(pszfn);

   std::cout << "SNMP test [" << pszfn << "]\n";

   AsnLen bytesDecoded = 0;

   try
   {
     snmpObject.BDec(inputBuf, bytesDecoded);
     std::cout << "\n";
   }
   catch (std::bad_alloc &)
   {
          std::cout << "Caught bad alloc!\n";
   }
   catch (SnaccException &)
   {
      std::cout << "test_snmp_file() failure " << pszfn << "\n";
   }
   std::cout.flush();
}

/************************************************************************
 FUNCTION: run_snmp_tests()
 
 Description:  This functions traverses through a directory containing
 SNMP V1 objects and uses test_snmp_test() to decode them.  The SNMP objects
 distributed with eSNACC should all decode succesfully.
 ************************************************************************/
void run_snmp_tests(char *path)
{

   char *fn = NULL;
   char fullPath[256];

   GFSI_HANDLE gfsi_handle;

   std::cout << "\n*** SNMP V1 TESTS ***\n\n";

   fn = GFSI_GetFirstFile(&gfsi_handle, path);

   if (fn == NULL)
   {
      std::cout << "ERROR: Bath path or directory is empty\n";
   }

   while (fn != NULL)
   {
      sprintf(fullPath,"%s/%s",path,fn);
      test_snmp_file(fullPath);
      fn = GFSI_GetNextFile(&gfsi_handle);
   }

   GFSI_Close(&gfsi_handle);

   std::cout << "\n*** END OF SNMP V1 TESTS ***\n";

} /* end of traverseDir() */
