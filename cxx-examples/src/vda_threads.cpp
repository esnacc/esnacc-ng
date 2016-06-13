
//
// vda_threads.cpp
#include "vda_threads.h"
#ifdef _BEGIN_SNACC_NAMESPACE
using namespace SNACC;
#endif

// for WIN32 threaded functions are void functions.
// for pthread threaded functions return void pointers.
//

#ifdef WIN32
long vdaTestThreadsKickoff2(void( __cdecl *start_address )( void * ), 
						int lThreadCount, struct ThreadNumberDef *&plThread);
void vdaTestThreads_Perform(void *f);
#else
long vdaTestThreadsKickoff2( void * (*start_address )( void * ), 
						int lThreadCount, struct ThreadNumberDef *&plThread);
void * vdaTestThreads_Perform(void *f);
#endif

bool vdaTestThreads_CheckStatus(
    struct ThreadNumberDef *plThreadNumber, long lThreadCount);
void AsnIntTest_NOPRINT();

// NOT THREAD SAFE.  DO NOT CALL FROM A THREAD!!
//
void vdaSleep(long lSleep)
{
#if defined (WIN32)
		Sleep (lSleep);
#else
      usleep(lSleep*100);
#endif
}



//
//  This routine will generate a given number of threads and execute various
//  SNACC operations to test threads.
//
long vdaTestThreadsKickoff(int lThreadCount, struct ThreadNumberDef *&plThread)
{		
   long status=vdaTestThreadsKickoff2(vdaTestThreads_Perform, lThreadCount, plThread);
   return(status);
}

#ifdef WIN32
long vdaTestThreadsKickoff2(void( __cdecl *start_address )( void * ), 
                            int lThreadCount, struct ThreadNumberDef *&plThread)
#else
long vdaTestThreadsKickoff2(void * (*start_address )( void * ), 
                            int lThreadCount, struct ThreadNumberDef *&plThread)
#endif

{	
    FUNC("vdaTestThreadsKickoff2()");
    
    long status=0;
#ifndef NO_THREADS
    long count=0;
    plThread=(struct ThreadNumberDef *)calloc(lThreadCount, 
                sizeof(struct ThreadNumberDef));
  	 bool bThreadRunning = true;
    int i;

    try
    {
       //
       for (i=0; i < lThreadCount; i++)
       {       // Perform initialization.
           plThread[i].lThreadNumber = i;
           plThread[i].lThreadStatus = VDATHREAD_STATUS_NOT_DEFINED;
       }

       //
       for (i=0; i < lThreadCount; i++)
       {       // Create threads.
   #if defined (WIN32)
    	   _beginthread(start_address, 0, &plThread[i]);
   #else
	   pthread_create (&plThread[i].thread, NULL, start_address, (void *)&plThread[i]);
   #endif
         vdaSleep(3);
       }       // END for lThreadCount.
    }
    catch (SnaccException &e)
    {
       e.push(STACK_ENTRY);
       throw;
    }

#endif
    return(status);
}

//
//
long vdaTestThreadsFinish(int lThreadCount, struct ThreadNumberDef *plThread)
{
#ifndef NO_THREADS
   
   FUNC("vdaTestThreadsFinish()");
   
   long status=0;
   long count=0;
	bool bThreadRunning = true;

   try
   {
      while (bThreadRunning)
      {   
         // Wait until all threads have finished.
         bThreadRunning = vdaTestThreads_CheckStatus(plThread, lThreadCount);
      }

      if (plThread)
         free(plThread);

   } catch (SnaccException &e)
   {
      e.push(STACK_ENTRY);
      throw;
   }


   return(status);
#else
   return (0);
#endif
}

//
//
long vdaTestThreads(int lThreadCount)
{
   
   FUNC("vdaTestThreads()");
   
   long status=0;
   long count=0;
	bool bThreadRunning = true;
   struct ThreadNumberDef *plThread=NULL;

 
   try
   {

      status = vdaTestThreadsKickoff(lThreadCount, plThread);
      if (status == 0)
      {
         // give threads a chance to start
         status = vdaTestThreadsFinish(lThreadCount, plThread);
      }
   }
   catch (SnaccException &e)
   {
      e.push(STACK_ENTRY);
      throw;
   }

   return(status);
}


// return true if there are threads still running
//
bool vdaTestThreads_CheckStatus(
    struct ThreadNumberDef *plThreadNumber, long lThreadCount)
{
    bool bResult=false;
#ifndef NO_THREADS
    for (int i=0; i < lThreadCount; i++)
    {
	    if (plThreadNumber[i].lThreadStatus != VDATHREAD_STATUS_DONE)
            bResult = true;
    }       // END for lThreadCount
    if (!bResult)
    {
       std::cout << "ALL ThreadNumber Tests Passed, DONE!\n";
       std::cout.flush();
    }
#endif // No Threads

    return(bResult);
}


void vdaTestThreadsLock_Perform(void *f_INPUT)
{
    vdaTestThreads(*(long *)f_INPUT);
}

//
//
#ifdef WIN32
void vdaTestThreads_Perform(void *f_INPUT)
#else
void * vdaTestThreads_Perform(void *f_INPUT)
#endif
{
#ifndef NO_THREADS
    int err = 0;
    struct ThreadNumberDef *f=(struct ThreadNumberDef *)f_INPUT;
    f->lThreadStatus = VDATHREAD_STATUS_STARTED;
    int VDATEST_COUNT=5;

    // PERFORM thread test operation(s).
    for (int i=0; i < VDATEST_COUNT; i++)
    {

       threadLock();
       std::cout << "Thread #" <<  f->lThreadNumber << "\n";
       threadUnlock();
    }

    threadLock();

    std::cout << "Thread #" <<  f->lThreadNumber << " DONE!\n";
    std::cout.flush();
    
    threadUnlock();
	
    f->lThreadStatus = VDATHREAD_STATUS_DONE;
#endif // NO_THREADS
}

// EOF vda_threads.cpp
