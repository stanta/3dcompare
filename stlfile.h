// Copyright (c) 2014 Andranik Abrahamyan

#ifndef STLFILE_H
#define STLFILE_H

#include <iostream>
#include <fstream>
#include <exception>

#include "vector.h"

class	StlSearcher;

class	StlFile 
{
public:
	class wrong_header_size : public ::std::exception {};
	class error_opening_file : public ::std::exception {};
	enum Format {
		ASCII,
		BINARY
	};
	
	typedef struct {
		float x;
		float y;
		float z;
	} Normal;

	typedef char Extra[2];
	
	typedef struct {
		Normal	normal;
		Vector	vector[3];
		Extra	extra;
	} Facet;

	typedef struct {
		::std::string   header;
		Format          type;
		int             numFacets;
		int             numPoints;
		Vector          max;
		Vector          min;
		Vector          size;
		float           boundingDiameter;
		float           shortestEdge;
		float           volume;
		float           surface;
	} Stats;

	StlFile(StlSearcher* searcher);
	~StlFile();
	void	open(const ::std::string&);
	void	write(const ::std::string&);
	void	close();
	void	setFormat(const int format);
	Stats	getStats() const { return m_stats; };
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
	float	getVolume();
	float	getSurface();
	float	getArea(Facet *facet);
	void	calculateNormal(float normal[], Facet *facet);
	void	normalizeVector(float v[]);
	// members
	::std::ifstream	m_file;
	Facet *		m_facets;
	Stats		m_stats;

	StlSearcher* m_searcher;
};

#endif  // STLFILE_H