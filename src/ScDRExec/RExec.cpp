// RExec.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include <io.h> // _access

char	GetFirstDriveLetter()
{
	char szLocDrives[MAX_PATH], cPrevious = 'A' - 1;
	DWORD dwNumDrives = GetLogicalDriveStrings( MAX_PATH, szLocDrives);
	char *pszCurDrive = szLocDrives;

	while (dwNumDrives--)	{
		if ( (++cPrevious) != *pszCurDrive)
			return (cPrevious);

		cPrevious = *pszCurDrive;
		pszCurDrive += strlen(pszCurDrive) + 1;
	}	

	return '\0';
}

// returns ERROR_SUCCESS on success
// > 0 on failure
DWORD ConnectToNetresIfRequested(LPTSTR szCmdLine, 
								 LPTSTR szLocalPath)
{
	char* pszBreak = strstr(szCmdLine, "€");
	if (!pszBreak)
		return 0;

	// we found a € sign 
	// this means that we must connect to the path
	// specified in the char array before the €
	// terminate with NULL at € so that WNetAddConnection
	// sees only the remote path
	*pszBreak = '\0';

	szLocalPath[0] = GetFirstDriveLetter();
	if (szLocalPath == '\0')
		return ERROR_NO_SYSTEM_RESOURCES;
	szLocalPath[1] = ':';
	szLocalPath[2] = '\0';

	NETRESOURCE netres;
	memset(&netres, 0, sizeof(NETRESOURCE));
	netres.lpLocalName = szLocalPath;
	netres.lpRemoteName = szCmdLine;

	DWORD dwRet = WNetAddConnection2(&netres, "supportvg", 
		"beheerder", NULL);

	if (dwRet!=ERROR_SUCCESS)	{
		strcpy(szLocalPath, "");
		return dwRet;
	}

	// format new cmdline
	char _szBuf[MAX_PATH+100];
	strcpy(_szBuf, szLocalPath);
	strcat(_szBuf, "\\");
	strcat(_szBuf, pszBreak+1);
	strcpy(szCmdLine, _szBuf);

	return ERROR_SUCCESS;
}
/*
// user32.lib verwijderen als je onderstaande functie 
// niet gebruikt!
#include "stdio.h"
void SimpleMsgBox(char* inFormat, ...)
{
	char* _szMsg = new char[256]; 

	va_list args; 
	va_start(args,inFormat); 
	vsprintf(_szMsg,inFormat,args); 
	va_end(args); 
	
	MessageBox(NULL, _szMsg, "", NULL);
}
*/
BOOL IsFileInCmdLineAvailable(LPTSTR szCmdLine)
{
	// strip of arguments by replacing a space
	// after the last backslash
	int iPos=0;
	char *pszChar = strstr(strrev(szCmdLine), "\\");
	strrev(szCmdLine);

	if (pszChar)
		iPos = strlen(szCmdLine)-(pszChar - szCmdLine);
	
	// search for the first space after iPos
	pszChar = strstr(szCmdLine+iPos, " ");

	if (pszChar)
		*pszChar='\0';
	
	// check for file availability
	if (_access(szCmdLine, 0) == -1)
		return FALSE;
	
	// replace the space character
	if (pszChar)
		*pszChar=' ';
	
	return TRUE;
}

// returns a negative GetLastError() value on error,
// if not the return value of the executed process
// NOTE: you won't get the returnvalue if 
// you've started this executable with a different 
// account
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	char szCmdLine[MAX_PATH*2], szLocalPath[3] = "";
	strcpy(szCmdLine, lpCmdLine);
	
	// connect to a netresource if requested
	DWORD dwRet = ConnectToNetresIfRequested(szCmdLine, 
		szLocalPath);
	
	if (dwRet != ERROR_SUCCESS)
		return dwRet;

	// check for file availability
	if (!IsFileInCmdLineAvailable(szCmdLine))
		return ERROR_FILE_NOT_FOUND;
	
	PROCESS_INFORMATION pi;
	STARTUPINFO si = {sizeof(si), 0, ""};
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;
	si.lpDesktop = "winsta0\\default";

	if(!CreateProcess( NULL, szCmdLine, NULL,
		NULL, FALSE, NULL, NULL, NULL, &si, &pi))
		dwRet = GetLastError();
	else
		dwRet = 0;
	
	if (0 == dwRet)
	{
		// we have to wait for the process
		// if we've connected to a netres
		// => the connection cannot be canceled
		// before the executed process has ended
		WaitForSingleObject(pi.hProcess, INFINITE);
	}

	// close the handles
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	if (strlen(szLocalPath))
		WNetCancelConnection(szLocalPath, TRUE);

	return dwRet;
}
