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
	StlFile*	m_stlFile;
	StlFile*	m_tmpStlFile;
};
#endif  // StlCompare_H
