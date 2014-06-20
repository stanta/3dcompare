#include "mgcgi.h"

/********************************************************************
** mgcgiout.c = basic output page generration for MGCGI
**
** MG - 10/9/97
********************************************************************/


/******************   Global variables  *********************/
char * MGCGIRequiredMsg="%s may not be blank";	/* Msg for missing, required fields */
char * MGCGIOpenProb;	/* Msg for unable to open debug output file */
char MGCGICopyRight[128];	/* Copy right message */

/*******************************************************************
** Name:	MGCGIStartOutput
** Descr:	Initializes output to file destination
** Params:	None
** Returns:	Nothing
** Author:	MG 10/9/97
** Comment:	Internal function, should not be called by application
** Type:	Output/Initialization
********************************************************************/
void MGCGIStartOutput( void )
{
	if( MGCGIDest==NULL )
	{
		MGCGIDest=stdout;
		if( MGCGIOutputFile )
		{
			if( (MGCGIDest=fopen(MGCGIOutputFile, "w+"))==NULL )
			{
				MGCGIOpenProb="Can't open output file";
				MGCGIDest = stdout;
			}
		}
	}
}

/*******************************************************************
** Name:	MGCGIHTMLMessage
** Descr:	Basic message-only output routine to make a response page
** Params:	Heading = Heading for page
**			Format = Like printf, a format string
**			Any number of values may follow Format
** Returns:	Nothing
** Author:	MG 10/9/97
** Comment:	Used internally to report various error messages, this routine
**			should not be used if MGCGIStartPage has already been called
** Type:	Output/Basic Message
********************************************************************/
void MGCGIHTMLMessage( char * Heading, char * Format, ... )
{
	va_list Args;
	va_start( Args, Format );
	
	MGCGIStartOutput();
	fprintf( MGCGIDest, "Content-type: text/html\n\n" );
	fprintf( MGCGIDest, "<HTML><BODY>\n");
	if( MGCGIOpenProb )
		fprintf( MGCGIDest, "[Error creating debug output file '%s']\n", MGCGIOutputFile );
	if( Heading )
		fprintf( MGCGIDest, "<H1><CENTER>%s<HR></CENTER></H1>", Heading );
	
	vfprintf(MGCGIDest, Format, Args );
	if( *MGCGICopyRight )
		fprintf( MGCGIDest, "<HR><CENTER><FONT SIZE=-1>%s</FONT></CENTER>", MGCGICopyRight );
	
	fprintf( MGCGIDest, "</BODY></HTML>\n");

	MGCGIExit();	
}

/*******************************************************************
** Name:	MGCGIStartPage
** Descr:	Begins output of a page
** Params:	Color = Color for web page, NULL to use browser default
**			Title = Title for page, in printf format
**			Any number of parameters can follow Title
** Returns:	Nothing
** Author:	MG 10/9/97
** Comment:	This is the function to call to at the start of normal page output
**			See also MGCGIStartForm
** Type:	Output/Initialization
********************************************************************/
void MGCGIStartPage( char * Color, char * Title, ... )
{
	va_list Args;
	va_start( Args, Title );

	MGCGIStartOutput();
	fprintf( MGCGIDest, "Content-type: text/html\n\n" );
	fprintf( MGCGIDest, "<HTML>\n");

	if( Title )
	{
		fprintf( MGCGIDest, "<HEAD><TITLE>" );
		vfprintf( MGCGIDest, Title, Args );
		fprintf( MGCGIDest, "</TITLE></HEAD>\n" );
	}
	if( Color )
		fprintf( MGCGIDest, "<BODY BGCOLOR=\"%s\">\n", Color );
	else
		fprintf( MGCGIDest, "<BODY>\n");
	if( MGCGIOpenProb )
		fprintf( MGCGIDest, "[Error creating debug output file '%s']\n", MGCGIOutputFile );
	
}


/*******************************************************************
** Name:	MGCGIStartForm
** Descr:	Begins output of a form-style page
** Params:	Action = Name of CGI program to call for the form
**			PostMethod = Either "GET" or "POST"
** Returns:	Nothing
** Author:	MG 10/9/97
** Comment:	MGCGIStartPage should be called first.
** Type:	Output/Initialization
********************************************************************/
void MGCGIStartForm( char* Action, int PostMethod)
{
	fprintf( MGCGIDest, "<FORM ACTION=\"%s\" METHOD=%s>\n", Action, PostMethod?"POST":"GET" );
}

/*******************************************************************
** Name:	MGCGIEndForm
** Descr:	Ends output for a form-style page
** Params:	None
** Returns:	Nothing
** Author:	MG 10/9/97
** Comment:	MGCGIStartForm should have been called previously
** Type:	Output/Initialization
********************************************************************/
void MGCGIEndForm( void )
{
	fprintf( MGCGIDest, "</FORM>" );
}

/*******************************************************************
** Name:	MGCGIPrintPage
** Descr:	Prints text to a page, without any conversions
** Params:	Format = Format for string, works like printf
** Returns:	Nothing
** Author:	MG 10/9/97
** Comment:	Data is sent raw to the page, meaning you can use this function
**			to put anything on the page.  See the MGCGIConvertToHTML function
**			to handle things like converting '<' to '&lt;' (an HTML requirement)
** Type:	Output/Basic output
********************************************************************/
void MGCGIPrintPage( char * Format, ... )
{
	va_list Args;
	va_start( Args, Format );
	vfprintf( MGCGIDest, Format, Args );
}


/*******************************************************************
** Name:	MGCGICenterMode
** Descr:	Turns centering mode on or off
** Params:	Mode = indicates desired centering mode, on or off
** Returns:	Nothing
** Author:	MG 10/9/97
** Type:	Output/Basic output
********************************************************************/
void MGCGICenterMode( int Mode )
{
	fprintf(MGCGIDest, Mode?"<CENTER>":"</CENTER>" );
}

/*******************************************************************
** Name:	MGCGICenterText
** Descr:	Centers the text supplied as it's parameter
** Params:	Str = Text to be centered
** Returns:	Nothing
** Author:	MG 10/9/97
** Type:	Output/Basic output
********************************************************************/
void MGCGICenterText( char* Str )
{
	fprintf(MGCGIDest, "<CENTER>%s</CENTER>\n", Str );
}

/*******************************************************************
** Name:	MGCGIBoldMode
** Descr:	Turns bold mode on or off
** Params:	Mode = indicates desired bolding mode, on or off
** Returns:	Nothing
** Author:	MG 10/9/97
** Type:	Output/Basic output
********************************************************************/
void MGCGIBoldMode( int Mode )
{
	fprintf( MGCGIDest, Mode?"<B>":"</B>" );
}

/*******************************************************************
** Name:	MGCGIBoldText
** Descr:	Bolds the text supplied as it's parameter
** Params:	Str = Text to be bolded
** Returns:	Nothing
** Author:	MG 10/9/97
** Type:	Output/Basic output
********************************************************************/
void MGCGIBoldText( char * Str )
{
	fprintf( MGCGIDest, "<B>%s</B>", Str );
}

/*******************************************************************
** Name:	MGCGIItalicMode
** Descr:	Turns italics mode on or off
** Params:	Mode = indicates desired italics mode, on or off
** Returns:	Nothing
** Author:	MG 10/9/97
** Type:	Output/Basic output
********************************************************************/
void MGCGIItalicMode( int Mode )
{
	fprintf( MGCGIDest, Mode?"<I>":"</I>" );
}

/*******************************************************************
** Name:	MGCGIItalicText
** Descr:	Italicizes the text supplied as it's parameter
** Params:	Str = Text to be italicized
** Returns:	Nothing
** Author:	MG 10/9/97
** Type:	Output/Basic output
********************************************************************/
void MGCGIItalicText( char * Str )
{
	fprintf( MGCGIDest, "<I>%s</I>", Str );
}


/*******************************************************************
** Name:	MGCGIIncludeFile
** Descr:	Inserts contents of a file into a page
** Params:	Src = Name of file to be inserted
** Returns:	Nothing
** Author:	MG 10/9/97
** Comment:	The contents of the file should be HTML format.
** Type:	Output/Basic output
********************************************************************/
void MGCGIIncludeFile( char * Src )
{
	FILE * SrcFile;
	char Buffer[512];
	int BytesRead;

	if( (SrcFile=fopen( Src, "r")) == NULL )
		fprintf( MGCGIDest, "<P>[Unable to include file:%s]</P>", Src );
	else
	{
		while( (BytesRead=fread( Buffer, 1, sizeof(Buffer), SrcFile )) )
			fwrite( Buffer, 1, BytesRead, MGCGIDest );
		fclose(SrcFile);
	}
}

/*******************************************************************
** Name:	MGCGIEndPage
** Descr:	Called to indicate page output is done
** Params:	None
** Returns:	Nothing
** Author:	MG 10/9/97
** Type:	Output/Basic output
********************************************************************/
void MGCGIEndPage( void )
{
	if( *MGCGICopyRight )
		fprintf( MGCGIDest, "<HR><CENTER><FONT SIZE=-1>%s</FONT></CENTER>", MGCGICopyRight );
	
	fprintf( MGCGIDest, "</BODY></HTML>" );
}

/*******************************************************************
** Name:	MGCGIConvertToHTML
** Descr:	Converts text to HTML-displayable code
** Params:	Dest = Where to store string (may be NULL)
**			Src = String to convert
** Returns:	Converted version of string (Dest, if it isn't NULL)
** Author:	MG 10/9/97
** Comment:	If Dest is NULL, then this function allocates string for
**			return value.  The caller is responsible for freeing it when
**			Done.
**			This routine converts characters like '<' to their HTML counterparts
**			like '&lt;'
** Type:	Output/Basic output
********************************************************************/
char* MGCGIConvertToHTML( char * Dest, char *Src )
{
	char *Tmp, *SrcStr, TmpArray[2]={'\0','\0'};
	unsigned int i, DestPos=0, BufSize=64, IAlloced=0;
	static char * Conversions[]={
	"<", "&lt;", ">", "&gt;", "&", "&amp;", "\"", "&quot", 
	"\n\r", "<BR>",
	"\r\n", "<BR>","\n", "<BR>","\r", "<BR>",
	NULL, NULL};
	if( Dest == NULL ) 
	{
		IAlloced=1;
		if(( Dest = (char*)malloc( BufSize )) == NULL )
			return( NULL );
	}

	*Dest='\0';
	for( ; *Src ; Src++ )
	{
		for( i=0; Conversions[i] && *Src!=*Conversions[i]; i+=2 )
			;
		if( Conversions[i] )
			SrcStr = Conversions[i+1];
		else
		{
			TmpArray[0] = *Src;
			SrcStr = TmpArray;
		}
		if( IAlloced && DestPos+strlen(SrcStr)+1>=BufSize )
		{
			BufSize += 64;
			if( (Tmp = (char*)realloc( Dest, BufSize )) == NULL )
			{
				free( Dest );
				return( NULL );
			}
			Dest = Tmp;
		}
		strcpy( Dest+DestPos, SrcStr );
		DestPos += strlen(SrcStr);
	}
	return( Dest );
}

/*******************************************************************
** Name:	MGCGISetCopyRight
** Descr:	Sets a copyright notice that will be displayed at the
**			bottom of all automatically-generated web pages
** Params:	Src = Copyright message
** Returns:	Nothing
** Author:	MG 10/9/97
** Type:	Output/Basic output
********************************************************************/
void MGCGISetCopyRight( char *Src )
{
	int Size = strlen(Src);
	
	Size = sizeof(MGCGICopyRight)-1>Size ? Size : sizeof(MGCGICopyRight)-1 ;

	memmove( MGCGICopyRight, Src, Size );
	MGCGICopyRight[Size]='\0';
}


/*******************************************************************
** Name:	MGCGIHRule
** Descr:	Inserts a horizontal rule
** Params:	Align = Specifies alignment and must be one of:
**				HA_NONE, HA_LEFT, HA_CENTER, HA_RIGHT
** Returns:	Nothing
** Author:	MG 10/9/97
** Type:	Output/Basic output
********************************************************************/
void MGCGIHRule( int Align )
{
	static char *HAlignLst[] = {"LEFT", "CENTER", "RIGHT"};
	if( Align )
		fprintf( MGCGIDest, "<HR ALIGN=%s>\n", HAlignLst[Align]);
	else
		fprintf( MGCGIDest, "<HR>\n");
}

