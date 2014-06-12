#include "mgcgi.h"

/********************************************************************
** mgctl.c = control/component creation for web pages
**
** MG - 10/9/97
********************************************************************/


/*******************************************************************
** Name:	MGCGICheckBox
** Descr:	Places a Checkbox on the output web page
** Params:	Name = Name of the field
**			Checked = boolean value, representing checked status
**			Text = Caption that appears near the checkbox
**			Para = boolean value to control insertion of paragraph tag after control
** Returns:	MGCGI_ERROK, or error code upon failure
** Author:	MG 10/9/97
** Type:	Output/Components and Controls
********************************************************************/
int MGCGICheckBox( char *Name, int Checked, char * Text, int Para )
{
	char * TmpText;
	
	if( Text )
		if( (TmpText = MGCGIConvertToHTML( NULL, Text )) == NULL )
			return( MGCGI_ERRNOMEM );
	
	if( Para )
		fprintf( MGCGIDest, "<P>");
	fprintf(MGCGIDest, "<INPUT TYPE=\"checkbox\" NAME=\"%s\" VALUE=\"checkbox\"%s>%s", 
		Name, Checked?" CHECKED=\"true\"":"", TmpText?TmpText:"" );
	if( Para )
		fprintf( MGCGIDest, "</P>");
	fprintf( MGCGIDest, "\n");
	if( Text )
		free( TmpText );
	return( MGCGI_ERROK );
}

/*******************************************************************
** Name:	MGCGIRadioButton
** Descr:	Places a single radio button on web page
** Params:	Name = Name for the field
**			Value = Value to be associated with field, if checked
**			Checked = boolean value, indicated checked status
**			Text = Text to appear to right of radio button
**			Para = boolean value indicating if paragraph tag should be added
** Returns:	MGCGI_ERROK, or error code upon failure
** Author:	MG 10/9/97
** See:		MGCGIRadioGroup
** Type:	Output/Components and Controls
********************************************************************/
int MGCGIRadioButton( char *Name, char *Value, int Checked, char *Text, int Para )
{
	char * TmpText;
	
	if( Text )
		if( (TmpText = MGCGIConvertToHTML( NULL, Text )) == NULL )
			return( MGCGI_ERRNOMEM );
	if( Para )
		fprintf( MGCGIDest, "<P>");
	fprintf( MGCGIDest, "<INPUT TYPE=\"radio\" NAME=\"%s\" VALUE=\"%s\"%s>%s", 
		Name, Value, Checked?" CHECKED=\"true\"":"", Text?TmpText:"" );
	if( Para )
		fprintf( MGCGIDest, "</P>");
	fprintf( MGCGIDest, "\n");
	if( Text )
		free( TmpText );
	return( MGCGI_ERROK );
}

/*******************************************************************
** Name:	MGCGIRadioGroup
** Descr:	Creates a group of radio buttons
** Params:	Name = Name of field
**			ValueSrc = boolean indicating if index position of control, 
**				or Text should be used for return value.
**			Checked = boolean flag indicating initial checked status
**			Text = array of string pointers, for values to return for each
**				selection.  May be NULL if ValueSrc is true.
**			Para = Indicates if paragraph tag should be inserted after each control
** Returns:	MGCGI_ERROK, or error code upon failure
** Author:	MG 10/9/97
** Type:	Output/Components and Controls
********************************************************************/
int MGCGIRadioGroup( char *Name, int ValueSrc, int Checked, char**Text, int Para)
{
	int Err, Count=0;
	char Value[5];
		
	while( *Text )
	{
		sprintf( Value, "%d", Count++ );
		Err=MGCGIRadioButton( Name, ValueSrc?Value:*Text, Count==Checked, *Text, Para );
		if( Err != MGCGI_ERROK )
			return( Err );
		Text++;
	}
	return( MGCGI_ERROK );
}

/*******************************************************************
** Name:	MGCGITextArea
** Descr:	Inserts a multi-line text-entry area in web page
** Params:	Name = Name for field
**			Initial = Initial contents for the field
**			Rows = Number of rows for field
**			Cols = Number of Cols for window
**			Para = Indicates if paragraph tag should be inserted after each control
** Returns: MGCGI_ERROK, or error code upon failure
** Author:	MG 10/9/97
** Type:	Output/Components and Controls
********************************************************************/
int MGCGITextArea( char * Name, char *Initial, int Rows, int Cols, int Para )
{
	char * TmpText;
	
	if( Initial )
		if( (TmpText = MGCGIConvertToHTML( NULL, Initial )) == NULL )
			return( MGCGI_ERRNOMEM );

	if( Para )
		fprintf( MGCGIDest, "<P>" );
	fprintf( MGCGIDest, "<TEXTAREA NAME=\"%s\" ROWS=\"%d\" COLS=\"%d\">%s</TEXTAREA>",
		Name, Rows, Cols, Initial?TmpText:"" );
	if( Para )
		fprintf( MGCGIDest, "</P>" );
	fprintf(MGCGIDest, "\n" );
	if( Initial )
		free( TmpText );
	return( MGCGI_ERROK );
}

/*******************************************************************
** Name:	MGCGIEditEx
** Descr:	Creates an edit control
** Params:	Name = Name for field
**			Initial = Initial contents for field
**			Size = Number of characters for string
**			Para = Indicates if paragraph tag should be inserted after control
**			Pass = boolean flag to indicate password-type or not
** Returns: MGCGI_ERROK, or error code upon failure
** Author:	MG 10/9/97
** Comment:	This routine is called by MGCGIEdit and MGCGIPassword
** Type:	Output/Components and Controls
********************************************************************/
int MGCGIEditEx( char *Name, char *Initial, int Size, int Para, int Pass )
{
	char * TmpText;
	
	if( Initial )
		if( (TmpText = MGCGIConvertToHTML( NULL, Initial )) == NULL )
			return( MGCGI_ERRNOMEM );

	if( Para )
		fprintf( MGCGIDest, "<P>" );
	if( Initial )
		fprintf( MGCGIDest, "<INPUT NAME=\"%s\" TYPE=\"%s\" VALUE=\"%s\" SIZE=\"%d\">",
		Name, Pass?"password":"text", TmpText, Size );
	else
		fprintf( MGCGIDest, "<INPUT NAME=\"%s\" TYPE=\"%s\" SIZE=\"%d\">",
			Name, Pass?"password":"text", Size );
	if( Para )
		fprintf( MGCGIDest, "</P>" );
	fprintf(MGCGIDest, "\n" );
	if( Initial )
		free( TmpText );
	return (MGCGI_ERROK );
}

/*******************************************************************
** Name:	MGCGIEdit
** Descr:	Places a standard input field on the web page
** Params:	Name = Name of field
**			Initial = Initial contents of the field
**			Size = Maximum size of the field
**			Para = Indicates if paragraph tag should be inserted after control
** Returns: MGCGI_ERROK, or error code upon failure
** Author:	MG 10/9/97
** Type:	Output/Components and Controls
********************************************************************/
int MGCGIEdit( char *Name, char *Initial, int Size, int Para )
{
	return( MGCGIEditEx( Name, Initial, Size, Para, 0 ) );
}

/*******************************************************************
** Name:	MGCGIPassword
** Descr:	Places a password input field on the web page
** Params:	Name = Name of field
**			Initial = Initial contents of the field
**			Size = Maximum size of the field
**			Para = Indicates if paragraph tag should be inserted after control
** Returns: MGCGI_ERROK, or error code upon failure
** Author:	MG 10/9/97
** Type:	Output/Components and Controls
********************************************************************/
int MGCGIPassword( char *Name, char *Initial, int Size, int Para )
{
	return( MGCGIEditEx( Name, Initial, Size, Para, 1 ) );
}

/*******************************************************************
** Name:	MGCGIHidden
** Descr:	Places a hidden field on a form
** Params:	Name = Name of field
**			Initial = Initial contents of the field
** Returns: MGCGI_ERROK, or error code upon failure
** Author:	MG 10/9/97
** Comment:	You must provide an initial value, since the user can't. 
**			This function is useful for CGI programs to 'hide' data in pages.
** Type:	Output/Components and Controls
********************************************************************/
void MGCGIHidden( char *Name, char *Initial )
{
	fprintf( MGCGIDest, "<INPUT TYPE=\"hidden\" NAME=\"%s\" VALUE=\"%s\">", Name, Initial?Initial:"" );
}

/*******************************************************************
** Name:	MGCGISelect
** Descr:	Adds a selection list (list box) to a web page
** Params:	Name = Name of field
**			Selected = An array of chars indicating inital selected status.
**			Items = List of strings to appear in the select (list) box.
**			Size = Indicates how man items can be selected by user
**			Para = Indicates if paragraph tag should be inserted after control
** Returns: MGCGI_ERROK, or error code upon failure
** Author:	MG 10/9/97
** Type:	Output/Components and Controls
********************************************************************/
int MGCGISelect( char *Name, char *Selected, char **Items, int Size, int Para )
{
	char * TmpText;
	
	if( Para )
		fprintf( MGCGIDest, "<P>" );
	if(Size)
		fprintf( MGCGIDest, "<SELECT SIZE=\"%d\" NAME=\"%s\">\n", Size, Name);
	else
		fprintf( MGCGIDest, "<SELECT NAME=\"%s\">\n", Name);
	while( *Items )
	{
		if( (TmpText = MGCGIConvertToHTML( NULL, *Items )) == NULL )
			return( MGCGI_ERRNOMEM );
		fprintf( MGCGIDest, "<OPTION%s>%s\n", *Selected++?" SELECTED":"", TmpText );
		Items++;
		free( TmpText );
	}
	fprintf( MGCGIDest, "</SELECT>");
	if( Para )
		fprintf( MGCGIDest, "</P>" );
	fprintf(MGCGIDest, "\n" );
	return( MGCGI_ERROK );
}

/*******************************************************************
** Name:	MGCGISubmit
** Descr:	Adds a submit button to a form
** Params:	Name = Name of the button
**			Value = Caption for the button (NULL="Submit")
**			Para = Indicates if paragraph tag should be inserted after control
** Returns:	Nothing
** Author:	MG 10/9/97
** Type:	Output/Components and Controls
********************************************************************/
void MGCGISubmit( char *Name, char *Value, int Para )
{
	if( Para )
		fprintf( MGCGIDest, "<P>" );
	
	Value = Value ? Value : "Submit";
	fprintf( MGCGIDest, "<INPUT NAME=\"%s\" TYPE=\"submit\" VALUE=\"%s\">", 
		Name, Value );

	if( Para )
		fprintf( MGCGIDest, "</P>" );
	fprintf(MGCGIDest, "\n" );

}

/*******************************************************************
** Name:	MGCGIReset
** Descr:	Adds a reset button to a form
** Params:	Name = Name of field
**			Value = Caption for button (NULL="Reset")
**			Para = Indicates if paragraph tag should be inserted after control
** Returns:	Nothing
** Author:	MG 10/9/97
** Type:	Output/Components and Controls
********************************************************************/
void MGCGIReset( char * Name, char * Value, int Para )
{
	if( Para )
		fprintf( MGCGIDest, "<P>" );
	
	Value = Value ? Value : "Reset";
	
	fprintf( MGCGIDest, "<INPUT NAME=\"%s\" TYPE=\"reset\" VALUE=\"%s\">", 
		Name, Value);

	if( Para )
		fprintf( MGCGIDest, "</P>" );
	fprintf(MGCGIDest, "\n" );

}

/*******************************************************************
** Name:	MGCGIStartTable
** Descr:	Starts a table definition for a web page
** Params:	Width = Width of table in page.  A negative value indicates 
**				pixel units, a positive value indicates percentage.
**			Border = Border width, in pixels
**			CellSpacing = Cell spacing, in pixels
**			CellPadding = Padding space inside cell, in pixels
** Returns:	Nothing
** Author:	MG 10/9/97
** Comment:	Be sure to call MGCGIEndTable at end of table output
** Type:	Output/Components and Controls
********************************************************************/
void MGICGIStartTable( int Width, int Border, int CellSpacing, int CellPadding )
{
	fprintf( MGCGIDest, "<TABLE WIDTH=\"%d%s\" BORDER=\"%d\" CELLSPACING=\"%d\" CELLPADDING=\"%d\">\n",
		Width<0?-Width:Width, Width<0?"":"%", Border, CellSpacing, CellPadding );
}

/*******************************************************************
** Name:	MGCGIStartTableRow
** Descr:	Indicates start of data for a table's row
** Params:	None
** Returns:	Nothing
** Author:	MG 10/9/97
** Comment:	Be sure to call MGCGIEndTableRow at end of row output
** Type:	Output/Components and Controls
********************************************************************/
void MGICGIStartTableRow()
{
	fprintf(MGCGIDest, "<TR>\n");
}

/*******************************************************************
** Name:	MGCGIEndTableRow
** Descr:	Indicates end of data for a single row in table
** Params:	None
** Returns:	Nothing
** Author:	MG 10/9/97
** Type:	Output/Components and Controls
********************************************************************/
void MGICGIEndTableRow()
{
	fprintf(MGCGIDest, "</TR>\n");
}

/*******************************************************************
** Name:	MGCGIStartTableCell
** Descr:	Starts data for a single cell of a table.
** Params:	Width = Width of cell, a negative value indicates pixels, 
**				while positive indicates percentage of table width
**			Align = Horizontal alignment, must be one of:
**				HA_NONE, HA_LEFT, HA_CENTER, or HA_RIGHT
**			VAlign = Vertical alignment, must be one of:
**			VA_NONE, VA_TOP, VA_CENTER, VA_BOTTOM, VA_BASELINE
**			NoWrap = flag indicating if the 'No Word Wrap' is on
**			Heading = Indicates if cell is a header cell or not
** Returns:	Nothing
** Author:	MG 10/9/97
** Comment:	Be sure to call MGCGIEndTableCell at end of cell output
** Type:	Output/Components and Controls
********************************************************************/
void MGICGIStartTableCell(int Width, int Align, int VAlign, int NoWrap, int Header)
{
	static char *HAlignLst[] = {"LEFT", "CENTER", "RIGHT"};
	static char *VAlignLst[] = {"TOP", "CENTER", "BOTTOM", "BASELINE"};

	fprintf(MGCGIDest, "<T%c", Header?'H':'D');
	if( Width )
		fprintf( MGCGIDest, " WIDTH=\"%d%s\"", Width<0?-Width:Width, Width<0?"":"%" );
	if( Align )
		fprintf( MGCGIDest, " ALIGN=\"%s\"", HAlignLst[Align] );
	if( VAlign )
		fprintf( MGCGIDest, " VALIGN=\"%s\"", VAlignLst[VAlign] );
	if( NoWrap )
		fprintf( MGCGIDest, " NOWRAP" );
		
	fprintf( MGCGIDest, ">\n" );
}

/*******************************************************************
** Name:	MGCGIEndTableCell 
** Descr:	Indicates end of a table cell definition
** Params:	None
** Returns:	Nothing
** Author:	MG 10/9/97
** Type:	Output/Components and Controls
********************************************************************/
void MGICGIEndTableCell()
{
	fprintf(MGCGIDest, "</TD>\n");
}
						 
/*******************************************************************
** Name:	MGCGIEndtTable
** Descr:	Indicates end of a table definition
** Params:	Nothing
** Returns:	Nothing
** Author:	MG 10/9/97
** Type:	Output/Components and Controls
********************************************************************/
void MGCGIEndTable( void )
{
	fprintf( MGCGIDest, "</TABLE>\n" );
}

/*******************************************************************
** Name:	MGCGILink
** Descr:	Inserts a url-style link in a web page
** Params:	URL = The url for the link
**			Text = Text that describes the URL.
** Returns: MGCGI_ERROK, or error code upon failure
** Author:	MG 10/9/97
** Type:	Output/Components and Controls
********************************************************************/
int MGCGILink( char *URL, char *Text )
{
	char *TmpText;
	
	if( (TmpText = MGCGIConvertToHTML( NULL, Text )) == NULL )
		return( MGCGI_ERRNOMEM );

	fprintf( MGCGIDest, "<A HREF=\"%s\">%s</A>", URL, TmpText );
	free( TmpText );
	return( MGCGI_ERROK );
}
