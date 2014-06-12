/*
** This program provides anonymous feedback support, using the
** MGCGI library.
**
** It assumes the form that invokes it has 3 parameters:
**   email - User's email - optional
**   class - User's class - optional
**   comments - User'sComments - required
**
*/
#include "mgcgi.h"

/* Mail message buffer */ 
char MailMessage[256];

/* List of required fields */
char * ReqFields[] = {
	"COMMENTS", "Feedback comments",
	NULL, NULL };

void main( int argc, char**argv )
{
	char *EMail, *Class, *Comments;
	char Format[]="From:   %s\nClass: %s\nSubject:\n%s\n";

	/* Set copyright notice that will appear at bottom of all pages */
	MGCGISetCopyRight( "Copyright 1997-98, Mario Giannini" );

	/* Loads input form data */
	MGCGILoadInput();
	
	MGCGISetRequiredText( "Please make sure that %s are specified" );
	MGCGIRequiredInput( ReqFields );

	/* Validate and format our CGI parameters, for email */
	EMail = MGCGIGetValue("email");
	Class = MGCGIGetValue("class");
	Comments = MGCGIGetValue( "comments" );

	if( strlen(EMail)+strlen(Class)+strlen(Comments)+strlen(Format) > sizeof(MailMessage) )
	{
		MGCGIStartPage( NULL, "Feedback Response - Error" );
		MGCGIPrintPage( "Sorry. comments are limited to %d characters.<P>", sizeof(MailMessage)-1 );
		MGCGILink( "http://www.openroad.org/feedback.html", "Try again" );
		MGCGIEndPage();
		MGCGIExit();
	}
	/* At this point - We are cleared to send the message */
	sprintf( MailMessage, Format,  EMail, Class, Comments );
		
	/* Send via email */
	if( MGCGIMailTo( "mario@openroad.org", "Feedback", MailMessage ) != MGCGI_ERROK )
		MGCGIHTMLMessage( "Error", "An error occured in sending your comment.  Please send normal email." );

	/* Starts output page data */
	MGCGIStartPage( NULL, "Feedback Response" );

	
	MGCGIHRule( HA_CENTER );
	MGCGIPrintPage( "Thank you for your comments, they are appreciated.<P><P>Mario<P><P>" );
	MGCGILink( "http://www.openroad.org/welcome.shtml", "Back to home" );
	

	/* Signal end of page */
	MGCGIEndPage();

	/* Terminate program, and shut down MGCGI library */
	MGCGIExit();

}
