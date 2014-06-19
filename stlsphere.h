// Copyright (c) 2014 Andranik Abrahamyan

#ifndef STLStlSphere_H
#define STLStlSphere_H

#include <iostream>
#include <fstream>
#include <exception>
#include <vector>

#include "types.h"

/*! \brief  The StlSphere class provides spherical and  normal frequency implementations.
*
*	StlSphere has spherical cordinates of the unit mesh, normals frequency and unit mesh data.
*/

class StlSphere
{
public:
	/// normal frequency
	struct NormalFrequency
	{
		short	x_pos;	//!<  x position of the normal
		short	y_pos;	//!<  y position of the normal
		short	z_pos;  //!<  z position of the normal
		int		frequency; //!<  frequency of the normal
	};

	/// Spherical cordinates of the unit mesh
	struct Spherical
	{
		float	radial[3];  //!<  radial cordinate
		float	polar[3];   //!<  polar cordinate
		float	azimuth[3]; //!<  azimuth cordinate
	};

	/// unit mesh data
	struct UnitStats 
	{
		Vector		max;			//!< max size 
		Vector		min;			//!< min size 
		Vector		size;			//!< delta from max and min sizes
		float		box_length;		//!< bounding box length
		float		box_width;		//!< bounding box width
		float		box_height;		//!< bounding box height
		Vector		centerOfMass;	//!< center of mass 
		float       volume;			//!< volume(or mass) of the unit mesh
		float       surface;		//!< surface of the unit mesh
		float		averageRadial;  //!< average radial of the unit mesh
		float		minRadial;		//!< minimal radial of the unit mesh
	};
	StlSphere(int devideSphere);
	~StlSphere();

	/// Allocate memory for the unit spherical coordinates
	void	allocate(int numFacets);
	/// get the minimal radial
	int		getMinRadial(int index);
	///	set unit mesh data
	void	setUnitSphere(std::vector<Facet>* facet, Stl_Stats * stats);
	/// get the unit mesh  stats
	UnitStats	getStats() { return m_UnitStats; }	
	/// get the unit spherical coordinates item
	Spherical	getSpherical(int i) const { return m_vSpherical[i]; };
	/// creates the file for the normals frequency( have .nfr extention)
	void		writeAsNormalFrequency(const ::std::string&);	
	/// get the vector for the normals frequency
	std::vector<NormalFrequency>* getNormalFrequency() { return & m_vNormalFreq; }

private:
	/// get minRadial and maxRadial without sqrt  
	void	setMinMaxRadial(Vector* vector );
	/// set spherical coordinates
	void	setSphericalItem(std::vector<Facet>* fac, int i);
	/// Cartesian to sphere coordinate transform
	void	cartesianToSphere(int m, int n, double c[], double x[], double r[], double theta[]);
	/// facets total mass and center of mass calculation
	void	facetMassCalc(std::vector<Facet>* fac, Stl_Stats * stats);
	float	getArea(Facet *facet);
	/// Calculate the normal of the facet
	void	calculateNormal( Facet *facet);
	/// normalize the normal
	void	normalizeVector(Normal* normal);
	/// set frequency for the normal
	void	setNormalFrequency(float x, float y, float z);
	/// Calculate arctangent
	float	calcAtan(float x, float y);

	/// vector for the unit spherical coordinates
	std::vector<Spherical>	m_vSpherical;
	/// The unit mesh  stats
	UnitStats	m_UnitStats;
	/// maximal radial of the original mesh.using this we trsnsform to unit mesh 
	float		m_maxRadial; 
	/// devide the unit sphere now it is a constant= 36  (10 deg)
	int		m_devideSphere; 
	/// The vector for the normals frequency
	std::vector<NormalFrequency>	m_vNormalFreq;	
};

#endif  // STLStlSphere_H
