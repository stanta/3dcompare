// Copyright (c) 2014 Andranik Abrahamyan

#ifndef STLFILE_H
#define STLFILE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <exception>

#include "types.h"

class	StlSphere;

class	StlFile 
{
public:
	class wrong_header_size : public std::exception {};
	class error_opening_file : public std::exception {};

	
	StlFile();
	~StlFile();
	bool	open(const	std::string&);
	void	write(const std::string&);
	void	writeAsSphere(const	std::string&);	
	void	setFormat(const int format);
	Stl_Stats	getStats() const { return m_stats; };
	Facet		getFacet(int i) const { return m_vFacets[i]; };
	StlSphere*	getStlSphere() const { return m_stlSphere; };
	

private:
	bool	initialize(const std::string&);
	void	allocate();
	void	readData(int, int);
	int		readIntFromBytes(std::ifstream&);
	float	readFloatFromBytes(std::ifstream&);
	void	writeBytesFromInt(std::ofstream&, int);
	void	writeBytesFromFloat(std::ofstream& file, float);
	void	writeBinary(const std::string&);
	void	writeAscii(const std::string&);
	int		getNumPoints();

	// members
	::std::ifstream		m_file;
	std::vector<Facet>	m_vFacets;
	Stl_Stats			m_stats;

	StlSphere*		m_stlSphere;
};

#endif  // STLFILE_H