// RenDir.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"

int main(int argc, char* argv[])
{
	if (argc < 3)	{
		printf("\nUsage:	AT-Rendir OldPath NewPath\nNote: OldPath must exist!");
		return 0;
	}

	printf(( 0 == rename(argv[1], argv[2])) ? "Operation succeeded.\n" : 
											 "Operation failed.\n");
		  
	
	return 0;
}
