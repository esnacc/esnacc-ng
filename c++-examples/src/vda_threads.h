// vda_threads.h
//
#include <memory.h>
#include <malloc.h>
#if defined (WIN32)
#include <windows.h>
#include "process.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#else
#define _REENTRANT
#ifndef NO_THREADS
#include "pthread.h"
#endif
#include "unistd.h"
#endif /* WIN32 */

#include "asn-incl.h"

#ifdef _BEGIN_SNACC_NAMESPACE
using namespace SNACC;
#endif

#ifndef NO_THREADS
long vdaTestThreads(int lThreadCount);

bool vdaTestThreads_CheckStatus(
    struct ThreadNumberDef *plThreadNumber, long lThreadCount);
void vdaSleep(long lSleep);

struct ThreadNumberDef {
      long lThreadNumber;
      long lThreadStatus;
#ifndef WIN32
      pthread_t thread;
#endif
};
#define VDATHREAD_STATUS_NOT_DEFINED 1
#define VDATHREAD_STATUS_STARTED 2
#define VDATHREAD_STATUS_DONE 3

long vdaTestThreadsFinish(int lThreadCount, struct ThreadNumberDef *plThread);

#endif //NO_THREADS
// EOF vda_threads.h
