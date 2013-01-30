// MTHttpReqExt.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "MTHttpReqExt.h"
#include "Wininet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

#define MT4_EXPFUNC __declspec(dllexport)

// The one and only application object

CWinApp theApp;

MT4_EXPFUNC void __stdcall Hello(char* say)
{
	int len = strlen(say);
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &say[0], len, NULL, 0);
    std::wstring wstrTo( size_needed, 0 );
    MultiByteToWideChar (CP_UTF8, 0, &say[0], len, &wstrTo[0], size_needed);
    //return wstrTo;	

	MessageBox(NULL, wstrTo.c_str(), TEXT("demo"),NULL);
}

int OpenConnection(char* Host, int Port)
{
	HINTERNET hi = InternetOpen();
	HINTERNET hi = InternetConnect();
	return 0;

}

bool SentRequest(int ConnectionId, char* RequestMethod, char* RequestPath, char* RequestData)
{
	HINTERNET HttpOpenRequest();
	return true;

}

char*  GetResponseData(char* Key)
{
	return NULL;
}

bool CloseConnection(int ConnectionId)
{
	return true;
}



int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: code your application's behavior here.
		}
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
