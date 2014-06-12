#include "mgcgi.h"

/********************************************************************
** mgcgiin.c = input/initialization/debugging abilities for MGCGI
**
** MG - 10/9/97
********************************************************************/


/******************   Global variables  *********************/
char * MGCGIInput, *MGCGIInputDup;/* 2 copies of input string */
char * MGCGIInputFile;	/* For debugging, file to be used for input */
char * MGCGIOutputFile;	/* For debugging, file to be used for output */

FILE * MGCGIDest;	/* Where output goes, console or debug file */
char * MGCGIIter;	/* Used as an iterator, to go through all values */

static char* MGCGIMGCGIOpenProb; /* Holds filename in case couldn't make output debug file*/
/***********************************************************/

/*******************************************************************
** Name:	MGCGIExit
** Descr:	Properly shuts down program
** Params:	None
** Returns:	Nothing
** Author:	MG 10/9/97
** Comment:	Should be called at end of program
** Type:	Process Control
********************************************************************/
void MGCGIExit( void )
{
	free( MGCGIInput );
	if( MGCGIDest && MGCGIDest!=stdout )
		fclose( MGCGIDest );
	exit(0);
}


/*******************************************************************
** Name:	MGCGICriticalGetEnv
** Descr:	Called to get environment variables critical to the system.
** Params:	Src = Environment variable to retrieve
** Returns:	Pointer to value of environment variable
** Author:	MG 10/9/97
** Comment:	If the variable is not defined, generates a standard error message 
**			and terminates the program,
** Type:	Input/Value determination
********************************************************************/
char * MGCGICriticalGetEnv( char *Src ) 
{
	char *Tmp = getenv( Src );
	if( Tmp==NULL )
		MGCGIHTMLMessage( "Critical Error", "Can't locate information for '%s'", Src );
	return( Tmp );
}

/*******************************************************************
** Name:	MGCGIGetValue
** Descr:	Returns a field value, or envirnonment variable
** Params:	Src = variable to retrieve
** Returns:	Pointer to value of variable, or NULL if not found
** Author:	MG 10/9/97
** Comment:	If the variable is not defined, generates a standard error message 
**			and terminates the program,
** Type:	Input/Value determination
********************************************************************/
char * MGCGIGetValue( char * Src )
{
	char * Tmp = MGCGIInput;
	int i, SrcLen=strlen(Src);

	if( MGCGIInput )
	{
		while( *Tmp )
		{
			for( i=0; i < SrcLen; i++ )
				if( toupper(Src[i])!=toupper(Tmp[i]) )
					break;

			if( i == SrcLen ) /* a match? */
			{
				if( Tmp[i]=='=' )
					return( Tmp+i+1 ); /* Yes! */
			}
			while( *Tmp )
					Tmp++;
			Tmp++;
		}
	}
	return( getenv( Src ) );
}

/*******************************************************************
** Name:	MGCGIGetSelection
** Descr:	Parses a multi-selection return value for 1 value
** Params:	Src = String gotten from control (via MGCGIGetValue)
**			Index = Which selection index to get
** Returns:	Pointer to value of variable, or NULL if not found
** Author:	MG 10/9/97
** Type:	Input/Value determination
********************************************************************/
char * MGCGIGetSelection( char * Src, int Index )
{
	char * Tmp = MGCGIInput;
	int Match=0, i, SrcLen=strlen(Src);

	do {
		while( *Tmp )
		{
			for( i=0; i < SrcLen; i++ )
				if( toupper(Src[i])!=toupper(Tmp[i]) )
					break;

			if( i == SrcLen ) /* a match? */
			{
				if( Tmp[i]=='=' )
				{
					Match=1;
					break;
				}
			}
			while( *Tmp )
				Tmp++;
			Tmp++;
		}
	} while( Index-- );
	
	return( Match ? Tmp+1 : NULL );
	
}

/*******************************************************************
** Name:	MGCGILoadInput
** Descr:	Loads and parses information from a form
** Params:	None
** Returns:	Nothing
** Author:	MG 10/9/97
** Comment:	Should be called at start of program, to get user input
**			Also supports loading data from a debug file
** Type:	Input/Initialization
********************************************************************/
void MGCGILoadInput( void )
{
	char *RequestMethod, *QueryString, *ContentLength, *Temp;
	int InputLength, i, j;

	if( MGCGIInputFile )
	{
		FILE * Src = fopen( MGCGIInputFile, "r" );
		if( Src==NULL )
			MGCGIHTMLMessage( "Error", "Can't open debug input file %s", MGCGIInputFile );
		fseek( Src, 0L, SEEK_END );
		InputLength=ftell( Src ); 
		fseek( Src, 0L, SEEK_SET );
		if( (MGCGIInput = (char*)calloc( 1, InputLength+2 )) == NULL )
		{
			fclose( Src );
			MGCGIHTMLMessage( "Error", "Out of memory loading debug input file" );
		}
		InputLength=fread( MGCGIInput, 1, InputLength, Src );
		while( MGCGIInput[InputLength]<=' ' )
			MGCGIInput[InputLength--]='\0';
		fclose( Src );
	}
	else
	{
		RequestMethod = MGCGICriticalGetEnv( "REQUEST_METHOD" );
		if( StrICmp( RequestMethod, "GET" )==0 )
		{	
			QueryString = getenv( "QUERY_STRING" ); 
			if( !QueryString )
				MGCGIInput = (char*)calloc( 2, sizeof(char) );
			else
			{
				MGCGIInput = (char*)calloc( strlen(QueryString)+2, sizeof(char) );
				if( MGCGIInput )
					strcpy( MGCGIInput, QueryString );
			}
			if( MGCGIInput == NULL )
				MGCGIHTMLMessage( "Error", "Can't allocate space for QUERY_STRING" );
		}
		else
		{
			Temp = MGCGIGetValue("CONTENT_TYPE");
			if( Temp!= NULL && StrICmp( Temp, "application/x-www-form-urlencoded") )
				MGCGIHTMLMessage( "Error", "Unsupported page type: %s", Temp?Temp:"(NULL)" );

			ContentLength = MGCGICriticalGetEnv( "CONTENT_LENGTH" );
			
			if( (InputLength=atoi(ContentLength)) == 0 )
				MGCGIHTMLMessage( "Error", "Content Length is zero" );
			if( (MGCGIInput = (char*)malloc( InputLength+2 )) == NULL )
				MGCGIHTMLMessage( "Error", "Can't allocate space for Content" );
			if( fread( MGCGIInput, InputLength, 1, stdin )==0 )
			{
				free( MGCGIInput );
				MGCGIHTMLMessage( "Error", "Error reading input stream" );
			}
			MGCGIInput[InputLength]=MGCGIInput[InputLength+1]='\0';
		}
	}
	MGCGIInputDup = strdup( MGCGIInput );
	/* At This Point - The input values are sitting in MGCGIInput. */
	/* It's now time to reverse the string conversion the sender */
	/* performed before transmission */
	for( j=i=0; MGCGIInput[i]; i++ )
	{
		MGCGIInput[i] = (MGCGIInput[i]=='+'? ' ' : MGCGIInput[i]);
		if( MGCGIInput[i]=='%' )
		{
			MGCGIInput[j++]=H2D(MGCGIInput[i+1])*16+H2D(MGCGIInput[i+2]);
			i+=2;
		}
		else
		if( MGCGIInput[i]=='&' )
			MGCGIInput[j++]='\0';
		else
			MGCGIInput[j++]=MGCGIInput[i];
	}
	MGCGIInput[j++] = '\0';
	MGCGIInput[j] = '\0';
}


/*******************************************************************
** Name:	MGCGISetRequiredText
** Descr:	Registers error message to be used when a required form value is blank
** Params:	Src = String to be used (may use %s, to specify field name).
** Returns:	Nothing
** Author:	MG 10/9/97
** Comment:	See MGCGIRequiredInput
** Type:	Input/Value determination
********************************************************************/
void MGCGISetRequiredText( char * Src )
{
	MGCGIRequiredMsg = Src;
}

/*******************************************************************
** Name:	MGCGIRequiredText
** Descr:	Verifies that specific, required, fields are not blank
** Params:	List = Aray of string pointers with names of fields to validate
** Returns:	Nothing
** Author:	MG 10/9/97
** Comment:	If you provide this routine with a list of field names that can not be blank, 
**			it will validate those fields, and in case of an error, display the message
**			which was registered with MGCGIRequiredText.  Helpfil for form validation
**			List points to strings organized as groups of 2 strings: 
**			FieldName, Descr, FieldName, Descr, etc.
** Type:	Input/Value determination
********************************************************************/
void MGCGIRequiredInput( char** List )
{
	char * Tmp;
	for( ; *List; List+=2 )
	{
		Tmp = MGCGIGetValue(List[0]);
		if(  Tmp==NULL || !*Tmp)
			MGCGIHTMLMessage( "Error", MGCGIRequiredMsg, List[1] );
	}
}

/*******************************************************************
** Name:	MGCGIRequiredInputMissing
** Descr:	Similar to MGCGIRequiredText, only does not automatically handle the error
** Params:	List = Array of strings indicating fields names that can not be blank
** Returns:	Name of field which was fund to be blank, NULL if all check out ok.
** Author:	MG 10/9/97
** Comment:	See MGCGIRequiredInput
** Type:	Input/Value determination
********************************************************************/
char* MGCGIRequiredInputMissing( char** List )
{
	char * Tmp;
	for( ; *List; List++ )
	{
		Tmp = MGCGIGetValue(List[0]);
		if(  Tmp==NULL || !*Tmp)
			return( *List );
	}
	return( NULL );
}

/*******************************************************************
** Name:	MGCGISetDebugInput
** Descr:	Used internally, to set the name of the debug input file
** Params:	File = Filename to use
** Returns:	Nothing
** Author:	MG 10/9/97
** Comment:	See MGCGIDebugSetup
** Type:	Debugging
********************************************************************/
void MGCGISetDebugInput( char * File )
{
	MGCGIInputFile = File;
}

/*******************************************************************
** Name:	MGCGISetDebugOutput
** Descr:	Used internally, to set the name of the debug output file
** Params:	File = Filename to use
** Returns:	Nothing
** Author:	MG 10/9/97
** Comment:	See MGCGIDebugSetup
** Type:	Debugging
********************************************************************/
void MGCGISetDebugOutput( char * File )
{
	if( MGCGIDest && MGCGIDest != stdout )
	{
		fclose( MGCGIDest );
		MGCGIDest = stdout;
	}
	MGCGIOutputFile = File;
	if( File )
	{
		if( (MGCGIDest=fopen( MGCGIOutputFile, "w+" ))==NULL )
		{
			MGCGIOpenProb = MGCGIOutputFile;
			MGCGIDest=stdout;
		}
	}
}

/*******************************************************************
** Name:	MGCGIDebugSetup
** Descr:	Parses commandline arguments to setup debugging files
** Params:	argc = same as in main
**			argv = same as in main
** Returns:	Nothing
** Author:	MG 10/9/97
** Comment:	The routine parses command line options "-I" and "-O" to
**			setup debugging files.  It can be called as the first
**			routine from main, passing argc and argv to it.  If you don't
**			need debugging, you can leave it out, though there is little
**			overhead in making it a permanent part of a program.
** Type:	Debugging
********************************************************************/
void MGCGIDebugSetup( int argc, char *argv[] )
{
	while( argc-- )
	{
		if( argv[0][0]=='-' && toupper(argv[0][1])=='I' )
			MGCGISetDebugInput( argv[0]+2 );
		if( argv[0][0]=='-' && toupper(argv[0][1])=='O' )
			MGCGISetDebugOutput( argv[0]+2 );

		argv++;
	}

}

/*******************************************************************
** Name:	MGCGIGetFirstInput
** Descr:	An iterator function, gets the first "FieldName=Value" string for a form
** Params:	None
** Returns:	Pointer to string
** Author:	MG 10/9/97
** Comment:	See MGCGIGetNextInput
** Type:	Debugging
********************************************************************/
char* MGCGIGetFirstInput( void )
{
	MGCGIIter=MGCGIInput;
	return( MGCGIIter );
}

/*******************************************************************
** Name:	MGCGIGetNextInput
** Descr:	An iterator function, gets the next "FieldName=Value" string for a form
** Params:	None
** Returns:	Pointer to string, or NULL if no more values left
** Author:	MG 10/9/97
** Comment:	See MGCGIGetNextInput
** Type:	Debugging
********************************************************************/
char* MGCGIGetNextInput( void )
{
	if( *MGCGIIter=='\0' )
		return( NULL );
	while( *MGCGIIter )
		MGCGIIter++;
	MGCGIIter++;
	return( *MGCGIIter ? MGCGIIter : NULL );
}
