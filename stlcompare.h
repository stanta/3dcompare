// Copyright (c) 2014 Andranik Abrahamyan

#ifndef StlCompare_H
#define StlCompare_H


class	StlFile;

class StlCompare
{
public:	
	StlCompare();
	~StlCompare();


private:
	// members
	StlFile*	m_stlFile1;
	StlFile*	m_stlFile2;
};
#endif  // StlCompare_H
