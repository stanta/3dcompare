#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

/********************************************************************
** mgcgi.h = Header file for MGCGI
**
** MG - 10/9/97
********************************************************************/


/* Note: If your using Unix and run into compile problems, un-comment
** the line below.  If Your using DOS/Windows and having compile problems,
** then make sure the line below is commented out.
*/
#define UNIX

#ifdef UNIX
#define StrICmp strcasecmp
#endif
#ifndef UNIX
#define StrICmp stricmp
#endif

#define HA_NONE 0
#define HA_LEFT 1
#define HA_CENTER 2
#define HA_RIGHT 3

#define VA_NONE 0
#define VA_TOP 1
#define VA_CENTER 2
#define VA_BOTTOM 3
#define VA_BASELINE 4

#define PM_GET 0
#define PM_POST 1


#define MGCGI_ERROK 0
#define MGCGI_ERRNOMEM 1
#define MGCGI_ERRNOCREATE 2
#define MGCGI_ERRINIT 3
#define MGCGI_ERRLOGON 4
#define MGCGI_ERRSEND 5

extern char * MGCGIInput, *MGCGIInputDup;
extern char * MGCGIRequiredMsg;
extern char * MGCGIInputFile;
extern char * MGCGIOutputFile;
extern char   MGCGICopyRight[];

extern FILE * MGCGIDest;
extern char * MGCGIOpenProb;

#define H2D(x) (toupper(x)>='A'?toupper(x)-'A'+10:(x)-'0')

/* Basic Input Abilities mgcgiin.c */
void MGCGIHTMLMessage( char * Heading, char * Format, ... );
char * MGCGIGetValue( char * Src );
char * MGCGIGetSelection( char * Src, int Index );
#define MGCGIIsChecked(Src) (MGCGIGetValue(Src)?1:0)
void MGCGILoadInput( void );
void MGCGISetReuiredText( char * Src );
void MGCGIRequiredInput( char** List );
void MGCGISetCopyRight( char *Src );
void MGCGIDebugSetup( int argc, char *argv[] );
void MGCGIExit( void );
char* MGCGIGetFirstInput( void );
char* MGCGIGetNextInput( void );
void MGCGISetRequiredText( char * Src );

/* Basic Output abilities - mgcgiout.c */
void MGCGIStartPage( char * Color, char * Title, ... );
void MGCGIStartForm( char* Action, int PostMethod);
void MGCGIEndForm( void );
void MGCGICenterMode( int Mode );
void MGCGICenterText( char *Str );
void MGCGIBoldMode( int Mode );
void MGCGIBoldText( char *Str );
void MGCGIItalicMode( int Mode );
void MGCGIItalicText( char *Str );
void MGCGIIncludeFile( char * Src );
void MGCGIEndPage( void );
void MGCGIPrintPage( char * Format, ... );
char* MGCGIConvertToHTML( char * Dest, char *Src );
void MGCGIHRule( int Align );

/* Forms creation abilities - mgictl.c */
int MGCGICheckBox( char *Name, int Checked, char * Text, int Para );
int MGCGIRadioButton( char *Name, char *Value, int Checked, char *Text, int Para );
int MGCGIRadioGroup( char *Name, int ValueSrc, int Checked, char**Text, int Para);
int MGCGITextArea( char * Name, char *Initial, int Rows, int Cols, int Para );
int MGCGIEditEx( char *Name, char *Initial, int Size, int Para, int Pass );
int MGCGIEdit( char *Name, char *Initial, int Size, int Para );
int MGCGIPassword( char *Name, char *Initial, int Size, int Para );
void MGCGIHidden( char *Name, char *Initial );
int MGCGISelect( char *Name, char *Selected, char **Items, int Size, int Para );
void MGCGISubmit( char *Name, char *Value, int Para );
void MGCGIReset( char * Name, char* Value, int Para );
void MGICGIStartTable( int Width, int Border, int CellSpacing, int CellPadding );
void MGICGIStartTableRow(void);
void MGICGIEndTableRow(void);
void MGICGIStartTableCell(int Width, int Align, int VAlign, int NoWrap, int Header);
void MGICGIEndTableCell(void);
void MGCGIEndTable( void );
int MGCGILink( char *URL, char *Text );


/* EMail support */
int MGCGIMailTo( char *Name, char *Subject, char *Message );
