// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MTHTTPREQEXT_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MTHTTPREQEXT_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef MTHTTPREQEXT_EXPORTS
#define MTHTTPREQEXT_API __declspec(dllexport)
#else
#define MTHTTPREQEXT_API __declspec(dllimport)
#endif

// This class is exported from the MTHttpReqExt.dll
class MTHTTPREQEXT_API CMTHttpReqExt {
public:
	CMTHttpReqExt(void);
	// TODO: add your methods here.
};

extern MTHTTPREQEXT_API int nMTHttpReqExt;

MTHTTPREQEXT_API int fnMTHttpReqExt(void);
