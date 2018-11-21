#include "stdafx.h"
#include "deldir.h"
#include <tchar.h>
#ifndef __AFX_H__
	#include "windows.h"
#endif

void TrimRight(LPTSTR szText, TCHAR cRemove)
{
	TCHAR *pszLast=_tcsninc(sz, _tcslen(sz));
	do
	{
		*pszLast=0;
		pszLast=_tcsdec(sz, pszLast);
	}while(pszLast&& *pszLast== cRemove);
/*	LPTSTR lpszLast = &szText[0] + strlen(szText);
	
	// remove backslashes
	while (*--lpszLast == cRemove)
		*lpszLast = '\0';
*/
}

void GetPath(LPTSTR szPath, LPCTSTR szDir, LPCTSTR szFileName)
{
	strcpy(szPath, szDir);
	TrimRight(szPath, '\\');
	strcat(szPath, "\\");
	strcat(szPath, szFileName);
}

BOOL DelDir(LPCTSTR szDir)
{
	WIN32_FIND_DATA fd;
	char szPath[MAX_PATH];
	GetPath(szPath, szDir, "*.*");
	
	HANDLE hFind = ::FindFirstFile(szPath, &fd);
	
	if (hFind == INVALID_HANDLE_VALUE)
		return FALSE;
	
	while (FindNextFile(hFind, &fd) ) {
		GetPath(szPath, szDir, fd.cFileName);
		
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY
			&& fd.cFileName[0] != '.') 
			DelDir(szPath);
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
			SetFileAttributes(szPath, FILE_ATTRIBUTE_NORMAL);
		
		DeleteFile(szPath);
	}
		
	FindClose(hFind);
		
	strcpy(szPath, szDir);
	TrimRight(szPath, '\\');
	SetFileAttributes(szPath, FILE_ATTRIBUTE_DIRECTORY);\
	RemoveDirectory(szPath);
	return TRUE;
}