
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SNACCDLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SNACCDLL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef SNACCDLL_EXPORTS
#define SNACCDLL_API __declspec(dllexport)
#else
#define SNACCDLL_API __declspec(dllimport)
#endif

// This class is exported from the snaccDLL.dll
class SNACCDLL_API CSnaccDLL {
public:
	CSnaccDLL(void);
	// TODO: add your methods here.
};

extern SNACCDLL_API int nSnaccDLL;

SNACCDLL_API int fnSnaccDLL(void);

