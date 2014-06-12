#include "mgcgi.h"



void main( int argc, char**argv )
{
	char Color[128];

	/* Loads input form data */
	MGCGILoadInput();	

	/* Starts output page data */
	MGCGIStartPage( NULL, "Favorite Color Response" );

	/* Retrieves a single value, for the FavColor Field */
	strcpy( Color, MGCGIGetValue("FavColor") );
	/* Note: If the field were not found, MGCGIGetValue returns NULL */

	/* Add a horizontal rule to page */
	MGCGIHRule( HA_CENTER );

	/* MGCGIPrintPage is similar to printf, but sends output to web page */
	if( strlen(Color) == 0 )
		MGCGIPrintPage( "Hey, you left color blank!" );
	else
		if( StrICmp( Color, "black" ) ==0 )
			MGCGIPrintPage( "Cool, black is my favorite color also" );
		else
			MGCGIPrintPage( "%s is nice, but I prefer black.", Color );

	MGCGIHRule( HA_CENTER );

	/* Signal end of page */
	MGCGIEndPage();

	/* Terminate program, and shut down MGCGI library */
	MGCGIExit();
}
