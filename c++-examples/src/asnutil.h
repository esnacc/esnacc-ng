#include "asn-incl.h"

#ifdef WIN32
#pragma warning(disable: 4305 4309 4101)
#endif

/* prototypes */
SNACC::AsnLen DecTagLen (unsigned char *bytes);
void bittests(void);
void inttests(void);
void vdaTestThreadLocks();
void run_snmp_tests(char *path);
void TestSetSorting(void);
void hex2str(const char *pszHex, std::string &str);
void fillTestBuffer(unsigned char *test, long length);
bool checkTestBuffer(const unsigned char *test, long length);


typedef void * GFSI_HANDLE; /* Generic File System Interface (GFSI) */
char *         GFSI_GetFirstFile(GFSI_HANDLE *gfsi_handle, char *path,
                        char *extension=NULL);
char *         GFSI_GetNextFile(GFSI_HANDLE *gfsi_handle, char *extension=NULL);
void           GFSI_Close(GFSI_HANDLE *gfsi_handle);
