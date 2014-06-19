// Copyright (c) 2014 Andranik Abrahamyan

#ifndef STLFILE_H
#define STLFILE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <exception>

#include "types.h"

class	StlSphere;

/*! \brief  The StlFile used for opening,saving and creating stl mesh data.
*
*	StlSphere has referance to the associated StlSphere object, the stl mesh stats and vector of the stl mesh facets.
*/

class	StlFile  
{
public:
	class wrong_header_size : public std::exception {}; //!< exception for the in wrong header size
	class error_opening_file : public std::exception {}; //!< exception for the in error opening file 

	
	StlFile();
	~StlFile();
	bool	open(const	std::string&); //!< opens the stl file
	void	write(const std::string&); //!< writes the stl file
	void	writeAsSphere(const	std::string&); //!< writes the spherical file(has .sph extention)	
	void	setFormat(const int format); //!< sets the format of the stl file
	Stl_Stats	getStats() const { return m_stats; }; //!< returns the stats
	Facet		getFacet(int i) const { return m_vFacets[i]; }; //!< returns item of facets
	StlSphere*	getStlSphere() const { return m_stlSphere; }; //!< returns the associated StlSphere object
	

private:
	bool	initialize(const std::string&); //!< initialize stl file
	void	allocate();	//!< allocate memory for the data
	void	readData(int, int);	//!< after opening the file, reads the data of the mesh
	int		readIntFromBytes(std::ifstream&); //!< read integer from bytes  
	float	readFloatFromBytes(std::ifstream&); //!< read float from bytes  
	void	writeBytesFromInt(std::ofstream&, int); //!< writes bytes from integer
	void	writeBytesFromFloat(std::ofstream& file, float); //!< writes bytes from float 
	void	writeBinary(const std::string&); //!< writes stl file in Binary format
	void	writeAscii(const std::string&); //!< writes stl file in ASCII format
	int		getNumPoints();	//!< returns number of points 

	/// members
	::std::ifstream		m_file; //!< the .stl file stream
	std::vector<Facet>	m_vFacets;	//!< vector of .stl mesh facets
	Stl_Stats			m_stats;	//!< Stl file stats
	StlSphere*			m_stlSphere;	//!< referance to the associated StlSphere object
};

#endif  // STLFILE_H