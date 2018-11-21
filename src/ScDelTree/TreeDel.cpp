// TreeDel.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "deldir.h"

#define IDP_WELCOME			"ScDelTree member of ScDOSPack Copyright (c) 2002 - SoftCentral\nAvailable at www.soft-central.net\nSend comments and bugreports to feedback@soft-central.net\n\n"
#define IDP_USAGE			"Usage: ScDelTree.exe directory\n\n"
  
int main(int argc, char* argv[])
{
	if (argc < 2) 
	{
		printf("Usage: ScDelTree.exe directory\n");
		return 0;
	}
	DelDir(argv[1]);
	return 0;
}

 