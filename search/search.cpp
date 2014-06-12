// search.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <string>
#include <vector>
/*
#include "../stlcompare.h"

int main(int argc, char* argv[])
{
	if (argc < 3) { // We expect 3 arguments: the program name, the source path and the destination path
        std::cerr << "Usage: " << argv[0] << "SOURCE DESTINATION" << std::endl;
        return 1;
    }
    //std::vector <std::string> file1;
	std::string file1;
    std::string file2;
    //for (int i = 1; i < argc; ++i) { // Remember argv[0] is the path to the program, we want from argv[1] onwards
       // if (i + 1 < argc)
          //  file1.push_back(argv[i]); // Add all but the last argument to the vector.
        //else
	file1 = argv[1];
    file2 = argv[2];
    //}
    //return move(sources, destination);
	//std::string s1 = "C:/dev/3DSearch/Models/stl_models/venus.stl";
	//std::string s2 = "C:/dev/3DSearch/Models/stl_models/venus2.stl";
	StlCompare compare;
	char res = compare.init(file1,file2);
	if (res != -1) {
		char buf[4];
		_itoa(res, buf, 10);
		std::cout << "Content-type:text/html\r\n\r\n";
		std::cout << "<html>\n";
		std::cout << "<head>\n";
		std::cout << "<title>Result of the Huffman Code</title>\n";
		std::cout << "</head>\n";
		std::cout << "<body>\n";
		std::cout << "<h2>The percent equality of two mesh is equal " << buf << "% </h2> \n";
		std::cout << "</body>\n";
		std::cout << "</html>\n";
	}
	return 0;
}
*/

#include "mgcgi/mgcgi.h" 


void main( int argc, char**argv ) 
{
	char Color[128]; 
	/* Loads input form data */ 
	MGCGILoadInput(); /* Starts output page data */ 
    /* Retrieves a single value, for the FavColor Field */ 
    MGCGIStartPage( NULL, "Favorite Color Response" ); 
    /* Note: If the field were not found, MGCGIGetValue returns NULL */ 
    strcpy( Color, MGCGIGetValue("FavColor") ); 
    /* Add a horizontal rule to page */ 
    MGCGIHRule( HA_CENTER ); 
    /* MGCGIPrintPage is similar to printf, but sends output to web page */ 
    if( strlen(Color) == 0 )
		MGCGIPrintPage( "Hey, you left color blank!" ); 
    else if( StrICmp( Color, "black" ) ==0 )
        MGCGIPrintPage( "Cool, black is my favorite color also" ); 
    else
        MGCGIPrintPage( "%s is nice, but I prefer black.", Color ); 

    MGCGIHRule( HA_CENTER ); 
    /* Signal end of page */ 
    MGCGIEndPage(); 
    /* Terminate program, and shut down MGCGI library */ 
    MGCGIExit(); 
} 