// DiskSpace.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include "winbase.h"
#include "stdio.h"

int main(int argc, char* argv[])
{
	if (argc != 4) {
		printf("Usage:\n\ndiskspace.exe pathtofoldertocheck minimumdiskspaceneeded(MB) loglocation\n");
		return 1;
	}

	// receive diskspaceinfo
	ULARGE_INTEGER BytesToCaller, TotalNumBytes,
		TotalNumFreeBytes;
	
	GetDiskFreeSpaceEx(argv[1], &BytesToCaller, &TotalNumBytes,
		&TotalNumFreeBytes);
	
	//if (*argv[2] == 'l')
	printf("%u MB free\n",TotalNumFreeBytes.QuadPart >> 20);
	printf("%u MB free to caller\n\n",BytesToCaller.QuadPart >> 20);
	printf("%u MB total bytes\n",TotalNumBytes.QuadPart >> 20);

	long lMinNeeded = atol(argv[2]);
	
	if ( (TotalNumFreeBytes.QuadPart >> 20) >= lMinNeeded)
		return 0;

	// workstation hasn't enough space so log it
	char szUserName[256], szHostName[256] = "";
	DWORD dwSize = 256;
	GetUserName(szUserName, &dwSize);
	WSADATA wsadat;
			
	if (0 == WSAStartup( 0x202, &wsadat ) )	{
		gethostname(szHostName, 256);
		WSACleanup();
	}
	
	// printf the info
	printf("\nNot enough space (%d available)\n",TotalNumFreeBytes.QuadPart >> 20 );

	// create empty logfile
	char szLogFile[MAX_PATH];
	sprintf(szLogFile, "%s\\%s--%s.log", argv[3], szHostName, szUserName);
	FILE* fp;
	fp = fopen(szLogFile, "w");
	if (!fp)
		return 1;

	fwrite(szLogFile, strlen(szLogFile), 1, fp);
	fclose(fp);



	return 1;
}

	
/*	argc = 4;
		for (int i = 0 ; i < 4 ; i++)
			argv[i] =  new char[64];
		
		strcpy(argv[0], "diskspace.exe");
		strcpy(argv[1], "c:\\");
		strcpy(argv[2], "200");
		strcpy(argv[3], "f:\\update\\log\\diskspace");

  		for ( i = 0 ; i < 4 ; i++)
			delete[] argv[i];
	*/