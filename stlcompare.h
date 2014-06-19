/// Copyright (c) 2014 Andranik Abrahamyan

#ifndef StlCompare_H
#define StlCompare_H

#include	<string>
class	StlFile;
/// class for comparing two meshes file by using Huffman_Code an getting the difference in percent.
/// 

/*! \brief  comparing two stl meshes.
*
*	class for comparing two meshes file by using Huffman_Code an getting the difference in percent.
*   After creating I call init() function. Which call getHuffman_Code.
*/

class StlCompare
{
public:	
	///constructor
	StlCompare();
	///destructor
	~StlCompare();
	/// create stl file from the file path, and call Huffman_Code;
	char	init(std::string &fileName1, std::string &fileName2);
	/// gets 2 StlFile  and calls Huffman_Code;
	char	init(StlFile* stlFile1, StlFile* stlFile2);
private:	
	/// compare 2 stl files Huffman_Code, returned by procent
	char		getHuffman_Code();
	// members
	StlFile*	m_stlFile1; //!< first StlFile file
	StlFile*	m_stlFile2; //!< second StlFile file
	bool		m_deleteStl; //!< need to delete stl files?
};
#endif  // StlCompare_H
