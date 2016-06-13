// constraints_test.cpp : Defines the entry point for the console application.
//
#ifdef WIN32
    #include <crtdbg.h>
#endif

#include <iostream>
#include "asn-incl.h"

int constraintTest_main(int argc, char *argv[]);
int constraintTest_mainRWC(int argc, char *argv[]);

int main(int argc, char* argv[])
{
#ifdef WIN32
    long memAllocNum = 0;
    
	// Set the debug flags for memory leak checking
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);

	int debugFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	debugFlag = _CrtSetDbgFlag(debugFlag | _CRTDBG_LEAK_CHECK_DF );

//	debugFlag = _CrtSetDbgFlag(debugFlag |
//		_CRTDBG_DELAY_FREE_MEM_DF |  _CRTDBG_CHECK_ALWAYS_DF);

	// Set a breakpoint on the allocation request number
	if (memAllocNum > 0)
		_CrtSetBreakAlloc(memAllocNum);
#endif // WIN32

    
    int iStatus = constraintTest_mainRWC(argc, argv);
    iStatus = constraintTest_main(argc, argv);

    return iStatus;
    
}
