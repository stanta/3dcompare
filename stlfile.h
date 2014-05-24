// Copyright (c) 2014 Andranik Abrahamyan

#ifndef STLFILE_H
#define STLFILE_H

#include <iostream>
#include <fstream>
#include <exception>

#include "types.h"

class	StlSearcher;

class	StlFile 
{
public:
	class wrong_header_size : public ::std::exception {};
	class error_opening_file : public ::std::exception {};

	
	StlFile(StlSearcher* searcher);
	~StlFile();
	void	open(const ::std::string&);
	void	write(const ::std::string&);
	void	close();
	void	setFormat(const int format);
	Stl_Stats	getStats() const { return m_stats; };
	Facet*	getFacets() const { return m_facets; };
	StlSearcher*	getSearcher() const { return m_searcher; };

private:
	void	initialize(const ::std::string&);
	void	allocate();
	void	readData(int, int);
	int		readIntFromBytes(::std::ifstream&);
	float	readFloatFromBytes(::std::ifstream&);
	void	writeBytesFromInt(::std::ofstream&, int);
	void	writeBytesFromFloat(::std::ofstream& file, float);
	void	writeBinary(const ::std::string&);
	void	writeAscii(const ::std::string&);
	int		getNumPoints();
	//float	getVolume();
	//float	getSurface();
	// members
	::std::ifstream		m_file;
	Facet *				m_facets;
	Stl_Stats			m_stats;

	StlSearcher*	m_searcher;
};

#endif  // STLFILE_H