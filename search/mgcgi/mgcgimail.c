#include "mgcgi.h"
#include <time.h>

/********************************************************************
** mgcgimto.c = EMail support
**
** MG - 10/9/97
********************************************************************/

/*******************************************************************
** Name:	MGCGIMailTo
** Descr:	Sends an email
** Params:	Name - Email address to send to
**          Subject - Subject of message
**          Message - Message
** Returns:	MGCI_ERROK upon success, or an error code upon failure
** Author:	MG 10/9/97
** Comment:	This function is available in UNIX or Windows, using
**          conditional compilation.  If you 'define' UNIX, then
**          The UNIX version is compiled, if you don't the Windows
**          MAPI version is compiled.
**
**          Windows MAPI Notes:
**          MAPI, and MAPI Server software (such as Netscape or Eudora)
**          must be installed and functional.  The MAPI system must have a
**          default logon and password for sending email.
**
**          UNIX Notes:
**          On my tested server, my CGIs do not have file
**          create and delete clearance on the directory.
**          For that reason, this code re-uses the same mail file,
**          which means that if two users attempt to send mail at
**          the exact same time, the results are unpredictable.
**          For additional security, the mail file is truncated.
** Type:	EMail
********************************************************************/
#ifdef UNIX
int MGCGIMailTo( char *Name, char *Subject, char *Message )
{
	FILE * Tmp=NULL;
	char Buffer[128], File[]="mail.tmp";
	
	Tmp = fopen( File, "w+" );
	if( Tmp )
	{
		fprintf( Tmp, "%s\n", Message );
		fclose( Tmp );
		sprintf( Buffer, "mail -s \"%s\" %s < %s", Subject, Name, File );
		system( Buffer );
		fclose( Tmp );
		Tmp = fopen( File, "w+" ); /* Emtpy file, for security*/
		fclose( Tmp ); 
		return( MGCGI_ERROK );
	}
	else
		return( MGCGI_ERRNOCREATE );
}
#endif

#ifndef UNIX
/* This conditional compilation is for a Windows MAPI-Compliant
** MGCGIMailTo function.  MAPI must be installed with the 
** appropriate server running.
*/
#include <windows.h>
#include <mapi.h>

int MGCGIMailTo( char *Name, char *Subject, char *Message )
{
	HINSTANCE hlibMAPI;
	LPMAPILOGON lpfnMAPILogon;
	LPMAPISENDMAIL lpfnMAPISendMail;
	LPMAPILOGOFF lpfnMAPILogoff;
	LHANDLE Session;
	ULONG Ret;
	MapiMessage TheMessage;
	MapiRecipDesc To;
	char Address[128];  /* Probably safe size, but watch it! */
	
	
	if( WINVER >= 0x400 )
		hlibMAPI = LoadLibrary("MAPI32.DLL");  /* 32 bit clients*/
	else
		hlibMAPI = LoadLibrary("MAPI.DLL");    /* 16 bit clients */

	if( !hlibMAPI )
		return( MGCGI_ERRINIT );
	
	lpfnMAPILogon = (LPMAPILOGON) GetProcAddress(hlibMAPI, "MAPILogon");
	lpfnMAPISendMail = (LPMAPISENDMAIL) GetProcAddress(hlibMAPI, "MAPISendMail");
	lpfnMAPILogoff = (LPMAPILOGOFF) GetProcAddress(hlibMAPI, "MAPILogoff");
	if( !lpfnMAPILogon || !lpfnMAPILogon || !lpfnMAPILogon )
	{
		FreeLibrary( hlibMAPI );
		return( MGCGI_ERRINIT );
	}
	
	sprintf( Address, "SMTP:%s", Name );
	To.ulReserved = 0;
    To.ulRecipClass = MAPI_TO;
    To.lpszName = Name;
    To.lpszAddress = Address;
    To.ulEIDSize = 0;
    To.lpEntryID = NULL;

	TheMessage.ulReserved = 0;
	TheMessage.lpszSubject = Subject;
	TheMessage.lpszNoteText = Message;
    TheMessage.lpszMessageType = NULL;
    TheMessage.lpszDateReceived = NULL;
    TheMessage.lpszConversationID = NULL;
    TheMessage.flFlags = 0;
    TheMessage.lpOriginator; 
    TheMessage.nRecipCount = 1;
    TheMessage.lpRecips = &To;
    TheMessage.nFileCount = 0;     
    TheMessage.lpFiles = NULL;

	Ret = lpfnMAPILogon( 0, NULL, NULL, MAPI_NEW_SESSION, 0, &Session );
	if( Ret != SUCCESS_SUCCESS )
		Ret = MGCGI_ERRLOGON;
	else
	{
		Ret = lpfnMAPISendMail( Session, 0, &TheMessage, 0, 0 );
		if( Ret != SUCCESS_SUCCESS )
			Ret = MGCGI_ERRSEND;
	}

	FreeLibrary( hlibMAPI );
	
	return( Ret );

}

#endif
