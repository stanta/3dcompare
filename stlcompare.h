// Copyright (c) 2014 Andranik Abrahamyan

#ifndef StlCompare_H
#define StlCompare_H

#include	<string>
class	StlFile;

class StlCompare
{
public:	
	StlCompare();
	~StlCompare();
	char	init(std::string &fileName1, std::string &fileName2);
	char	init(StlFile* stlFile1, StlFile* stlFile2);
private:
	// compare 2 stl files Huffman_Code, returnd by procent
	char		getHuffman_Code();

	// members
	StlFile*	m_stlFile1;
	StlFile*	m_stlFile2;
	bool		m_deleteStl;
};
#endif  // StlCompare_H
