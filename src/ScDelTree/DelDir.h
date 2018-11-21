#ifndef BOOL
	typedef int BOOL;
#endif

#ifndef LPCTSTR
	typedef const char* LPCTSTR;
#endif

#ifndef LPTSTR
	typedef char* LPTSTR;
#endif

#ifndef TCHAR
	typedef char TCHAR;
#endif

BOOL DelDir(LPCTSTR szDir);
void GetPath(LPTSTR szPath, LPCTSTR szDir, LPCTSTR szFileName);
void TrimRight(LPTSTR szText, TCHAR cRemove);
