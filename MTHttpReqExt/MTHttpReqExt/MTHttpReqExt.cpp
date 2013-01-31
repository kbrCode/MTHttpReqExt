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
HINTERNET hConn = NULL;

inline wstring ConvertFromChar(char* cSrc)
{
	int len = strlen(cSrc);
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &cSrc[0], len, NULL, 0);
    std::wstring wstrTo( size_needed, 0 );
    MultiByteToWideChar (CP_UTF8, 0, &cSrc[0], len, &wstrTo[0], size_needed);
	return wstrTo;
}


MT4_EXPFUNC void __stdcall Hello(char* say)
{
    std::wstring wstrTo = ConvertFromChar(say);
	MessageBox(NULL, wstrTo.c_str(), TEXT("demo"),NULL);
}

MT4_EXPFUNC int OpenConnection(char* Host, int Port)
{
	HINTERNET hIntrn = InternetOpen( TEXT("MT Request"), 
							INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY,
							NULL,
							NULL,
							0);
	if (!hIntrn)
	{
		printf("No Internet connection: %li.\n", GetLastError());
		return -1;
	}

	wstring wStrHost = ConvertFromChar(Host);

	hConn = InternetConnect( hIntrn, 
				wStrHost.c_str(),
				INTERNET_DEFAULT_HTTP_PORT,
				NULL,
				NULL,
				INTERNET_SERVICE_HTTP,
				0,
				NULL);
	if (!hConn)
	{
		printf("Connection to update server failed: %li.\n", GetLastError());
		return -1;
	}
	return (int)&hConn;
}

MT4_EXPFUNC bool SentRequest(int ConnectionId, char* RequestMethod, char* RequestPath, char* RequestData)
{
	static char szRawData[5000];
	memset(szRawData, 0x11, sizeof(szRawData));


	DWORD dwOpenRequestFlags = INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP |
								INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS |
								INTERNET_FLAG_KEEP_CONNECTION |
								INTERNET_FLAG_NO_AUTO_REDIRECT |
								INTERNET_FLAG_NO_COOKIES |
								INTERNET_FLAG_NO_CACHE_WRITE |
								INTERNET_FLAG_NO_UI |
								INTERNET_FLAG_RELOAD;

	wstring strMethod = ConvertFromChar(RequestMethod);
	wstring strPath = ConvertFromChar(RequestPath);
	wstring strData = ConvertFromChar(RequestData);


	HINTERNET hReq = HttpOpenRequest(hConn,
									strMethod.c_str(),
									strPath.c_str(),
									TEXT("HTTP/1.0"),
									NULL,
									NULL,
									dwOpenRequestFlags,
									NULL);
	
	string HttpHeaders = "Accept: */*; q=0.5, application/xml\r\nAccept-Encoding: gzip, deflate\r\nUser-Agent: API DLL\r\nHost: api.domena.com:9292\r\n\r\n"; 
	//CString strBoundary = TEXT("---------------------------autoupdater");
	//CString strContentHeader =  TEXT("Host: www.mydomain_at_powweb.com\r\n"
 //                             "Content-Type: multipart/form-data; boundary=");
 // strContentHeader+=strBoundary;

 // HttpAddRequestHeaders(hReq, strContentHeader, strContentHeader.GetLength(), HTTP_ADDREQ_FLAG_ADD);


  //CString strHeaders;
  //strHeaders.precache(16384);
  //sprintf(strHeaders,
  //        "--%s\r\n"
  //        "Content-Disposition: form-data; name=\"userfile\"; "
  //        "filename=\"test.raw\"\r\n"
  //        "Content-Type: application/octet-stream\r\n\r\n",
  //        (LPCTSTR)strBoundary);

  //tCharSeq s;//this is a just a dynamic array of bytes;
  ////
  //// append headers and file to request:
  ////
  //s.precache(16384);
  //s.append(strHeaders.length(), strHeaders);
  ////append with file data:
  //s.append(2000, szRawData); //<------------------- depending on this size, SendRequest fails.
  ////trailing end of data:
  //s.append(4,"\r\n--");
  //s.append(strBoundary.length(), (LPTSTR)strBoundary);
  //s.append(4,"--\r\n");

  //InternetSetOption(hReq, INTERNET_OPTION_USERNAME, "username\0", 9);
  //InternetSetOption(hReq, INTERNET_OPTION_PASSWORD, "password\0", 9);

	if (!HttpSendRequest(hReq, NULL, 0, (void*)HttpHeaders.c_str(), HttpHeaders.length()))
	{
		return printf("HttpSendRequest failed: %li.\n", GetLastError());
	}

	LPVOID lpOutBuffer=NULL;
	DWORD dwSize = 0;

	// This call will fail on the first pass, because
	// no buffer is allocated.
	if(!HttpQueryInfo(hReq, HTTP_QUERY_RAW_HEADERS_CRLF,
	(LPVOID)lpOutBuffer,&dwSize,NULL))
		{
		if (GetLastError()==ERROR_HTTP_HEADER_NOT_FOUND)
		{
			// Code to handle the case where the header isn't available.
			return false;
		}
		else
		{
			// Check for an insufficient buffer.
			if (GetLastError()==ERROR_INSUFFICIENT_BUFFER)
			{
				// Allocate the necessary buffer.
				lpOutBuffer = new char[dwSize];

				// Retry the call.
				if(!HttpQueryInfo(hReq, HTTP_QUERY_RAW_HEADERS_CRLF,
				(LPVOID)lpOutBuffer,&dwSize,NULL))
				{
					if (GetLastError()==ERROR_HTTP_HEADER_NOT_FOUND)
					{
						// Code to handle the case where the header isn't available.
						return false;
					}
					else
					{
						// Error handling code.
						if (lpOutBuffer)
						{
							delete [] lpOutBuffer;
						}
						return FALSE;
					}
				}
			}
			else
			{
				// Error handling code.
				if (lpOutBuffer)
				{
					delete [] lpOutBuffer;
				}
				return FALSE;
			}
		}
	}

	if (lpOutBuffer)
	{
		delete [] lpOutBuffer;
	}
	return TRUE;
}

MT4_EXPFUNC char*  GetResponseData(char* Key)
{
	return NULL;
}

MT4_EXPFUNC bool CloseConnection(int ConnectionId)
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
