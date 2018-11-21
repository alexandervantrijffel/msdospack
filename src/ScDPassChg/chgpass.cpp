



#ifndef UNICODE
#define UNICODE
#endif

#include <stdio.h>
#include <windows.h> 
#include <lm.h>
#include <lmaccess.h> 

int wmain(int argc, wchar_t *argv[])
{	
	DWORD dwError = 0;
   NET_API_STATUS nStatus;
   //
   // All parameters are required.
   //
   if (argc != 5)
   {
      fwprintf(stderr, L"Usage: %s \\\\ServerName UserName OldPassword NewPassword\n", argv[0]);
      exit(1);
   }
   //
   // Call the NetUserChangePassword function.
   //

   nStatus = NetUserChangePassword(argv[1], argv[2], argv[3], argv[4]);
   //
   // If the call succeeds, inform the user.
   	switch ( nStatus){
	case NERR_Success			:	fwprintf(stderr, L"The user password has been changed successfully\n");
									break;
	case ERROR_ACCESS_DENIED	:	fwprintf(stderr, L"The user does not have access to the requested information.\n");
									break;
	case ERROR_INVALID_PASSWORD	:	fwprintf(stderr, L"The user has entered an invalid password.\n");
									break;
	case NERR_InvalidComputer	:	fwprintf(stderr, L"The computer name is invalid.\n");
									break;
	case NERR_NotPrimary		:	fwprintf(stderr, L"The operation is allowed only on the primary domain controller of the domain.\n");
									break;
	case NERR_UserNotFound		:	fwprintf(stderr, L"The user name could not be found.\n");
									break;
	case NERR_PasswordTooShort	:	fwprintf(stderr, L"The password is shorter than required.\n");
									break;		
	default						:	fwprintf(stderr, L"An unknown error occured\n");
									break;
	}

   return 0;

}
