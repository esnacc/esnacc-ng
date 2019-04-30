#ifdef WIN32
// snaccDLL.cpp : Defines the entry point for the DLL application.
//

#include <windows.h>
#include "snaccdll.h"

extern "C" SNACCDLL_API BOOL WINAPI DllMain( HANDLE , 
                                               DWORD  ul_reason_for_call, 
                                               LPVOID
                                              )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}


// This is an example of an exported variable
SNACCDLL_API int nSnaccDLL=0;

// This is an example of an exported function.
SNACCDLL_API int fnSnaccDLL(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see snaccDLL.h for the class definition
CSnaccDLL::CSnaccDLL()
{ 
	return; 
}
#endif
