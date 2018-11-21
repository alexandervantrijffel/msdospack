// NoConsolEx.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

BOOL CALLBACK EnumWindowsProc(  HWND hwnd,      // handle to parent window
							    LPARAM lParam )  // application-defined value
{
	// hide the window if the classname == ConsoleWindowClass
	char szClassName[256];
	GetClassName(hwnd, szClassName, 256);
	if (!stricmp(szClassName, "ConsoleWindowClass") )
		ShowWindow(hwnd, SW_HIDE);
		
	return TRUE;
}
								

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	EnumWindows(EnumWindowsProc, NULL);
	return 0;
}



