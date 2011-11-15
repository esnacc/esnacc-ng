#include <stdio.h>
#include <stdlib.h>
#include "asn-incl.h"
#include "gen-buf.h"
#include "exp-buf.h"
#include "rfc1155-smi.h"
#include "rfc1157-snmp.h"
#include "gfsi.h"
#ifdef WIN32
#include <windows.h>
#include <winbase.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#endif

char* LoadFile (char *fileName, size_t *size);

/*
 * FUNCTION: test_snmp_file()
 *
 * Attempt to decode the SNMP file and indicate success or failure to std::cout
 */
void test_snmp_file(char *pszfn)
{
    Message a;
    int status = -1;
    AsnLen bytesDecoded = 0;
    ENV_TYPE env;
    size_t fsize;
    char *fileData;
    int val;
    GenBuf *pGenBuf = (GenBuf *) calloc(1, sizeof(GenBuf));
    ExpBuf *pExpBuf = (ExpBuf *) calloc(1, sizeof(ExpBuf));

    fileData = LoadFile (pszfn, &fsize);
    if (fileData == NULL)
    {
       printf("Failed to load file: %s\n", pszfn);
       return;
    }

    ExpBufInstallDataInBuf(pExpBuf, fileData, fsize);
    ExpBufResetInReadMode(&pExpBuf);
	 PutExpBufInGenBuf(pExpBuf, pGenBuf);

    if ((val = setjmp (env)) == 0)
        DDecMessage(pGenBuf, &a, &bytesDecoded, env);
    else
       printf("test_snmp_file() failure %s\n", pszfn);
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

   printf("\n*** SNMP V1 TESTS ***\n\n");

   fn = GFSI_GetFirstFile(&gfsi_handle, path, NULL);

   if (fn == NULL)
   {
      printf("ERROR: Bath path or directory is empty\n");
   }

   while (fn != NULL)
   {
      sprintf(fullPath,"%s/%s",path,fn);
      test_snmp_file(fullPath);
      fn = GFSI_GetNextFile(&gfsi_handle, NULL);
   }

   GFSI_Close(&gfsi_handle);

   printf("\n*** END OF SNMP V1 TESTS ***\n");

} 

/*
 * opens given filename, determines its size, allocs a block
 * of that size and reads the file into it. returns a pointer
 * to this block.  Prints an err msgs is something screwed up
 * and returns NULL.  Sets the size param to the size of the file.
 */
char* LoadFile (char *fileName, size_t *size)
{
    FILE *f;
    unsigned long int fsize;
    char *fileData;

    f = fopen (fileName, "rb");

    if (f == NULL)
    {
        Asn1Error("Could not open file for reading.\n");
        return NULL;
    }

    fseek (f, 0, 2);    /* seek to end */
    fsize = ftell (f);  /* get size of file */
    fseek (f, 0, 0);    /* seek to beginning */

    *size = fsize;
    fileData = (char *) malloc (fsize);

    if (fileData == NULL)
    {
        Asn1Error("Not enough memory to read in file.\n");
        return NULL;
    }

    if (fread (fileData, sizeof (char), fsize, f) != fsize)
    {
        free (fileData);
        fileData = NULL;
        Asn1Error("Trouble reading file.\n");
    }

    fclose (f);
    return fileData;
}  /* LoadFile */
