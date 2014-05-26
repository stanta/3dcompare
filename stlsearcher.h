// Copyright (c) 2014 Andranik Abrahamyan

#ifndef STLStlSearcher_H
#define STLStlSearcher_H

#include <iostream>
#include <fstream>
#include <exception>

#include "types.h"

class StlSearcher
{
public:
	struct Spherical
	{
		float	radial[3];
		float	polar[3];
		float	azimuth[3];
	};

	struct UnitStats // unit Sphere data
	{
		Vector		max;			// max size 
		Vector		min;			// min size 
		Vector		size;			// delta from max and min sizes
		float		box_length;		//bounding box length
		float		box_width;		//bounding box width
		float		box_height;		//bounding box height
		//float		boundingDiameter; // bounding box diameter
		Vector		centerOfMass; // center of mass 
		float       volume;		// volume(or mass) of the unit mesh
		float       surface;	// surface of the unit mesh
	};
	
	StlSearcher();
	~StlSearcher();

	Spherical*	getSpherical() const { return m_Spherical; };
	void	allocate(int numFacets);
	void	close();	
	int		getMinRadial(int index);
	void	setUnitSphere(Facet * facet, Stl_Stats * stats);
	float	getMaxRadial() { return m_maxRadial; }
	UnitStats	getStats() { return m_UnitStats; }

private:
	void	setSphericalItem(Vector* vector, int i);
	void	cartesianToSphere(int m, int n, double c[], double x[], double r[], double theta[]);
	void	facetMassCalc(Facet * facet, Stl_Stats * stats);
	float	getArea(Facet *facet);
	void	calculateNormal(float normal[], Facet *facet);
	void	normalizeVector(float v[]);

	// members
	Spherical * m_Spherical;  // Spherical values of the facets 
	float		m_maxRadial; // we get max radial and using this we doing unit trsnsform of the mesh
	UnitStats	m_UnitStats;
};

#endif  // STLStlSearcher_H
/*

public:
    float x1,y1,z1;
    float x2,y2,z2;
    float x3,y3,z3;
};
int calc_tetrahedrons ()  
{
    int numTriangles; // pull in the STL file and determine number of triangles
    data * triangles = new triangles [numTriangles];
    // fill the triangles array with the data in the STL file

    double totalVolume = 0, currentVolume;
    double xCenter = 0, yCenter = 0, zCenter = 0;

    for (int i = 0; i < numTriangles; i++)
    {
        
		currentVolume =(triangles[i].x1 * triangles[i].y2 * triangles[i].z3 - 
						triangles[i].x1 * triangles[i].y3 * triangles[i].z2 - 
						triangles[i].x2 * triangles[i].y1 * triangles[i].z3 + 
						triangles[i].x2 * triangles[i].y3 * triangles[i].z1 + 
						triangles[i].x3 * triangles[i].y1 * triangles[i].z2 - 
						triangles[i].x3 * triangles[i].y2 * triangles[i].z1) / 6;
		totalVolume += currentVolume;
        xCenter += ((triangles[i].x1 + triangles[i].x2 + triangles[i].x3) / 4) * currentVolume;
        yCenter += ((triangles[i].y1 + triangles[i].y2 + triangles[i].y3) / 4) * currentVolume;
        zCenter += ((triangles[i].z1 + triangles[i].z2 + triangles[i].z3) / 4) * currentVolume;
    }

    cout << endl << "Total Volume = " << totalVolume << endl;
    cout << endl << "X center = " << xCenter/totalVolume << endl;
    cout << endl << "Y center = " << yCenter/totalVolume << endl;
    cout << endl << "Z center = " << zCenter/totalVolume << endl;
}





void StlSearcher::cartesianToSphere( double x[], double r[], double theta[])
{
	int i, i1, j;
	double t, *top, *x2;

	//  Compute R.
	for ( j = 0; j < 3; j++ )  {
		t = 0.0;
		for ( i = 0; i < 3; i++ )
			t += pow(x[i+3*j], 2);
		r[j] = sqrt ( t );
	}
	//Compute M-2 components of THETA.
	for ( j = 0; j < 3; j++ ) // clear the area
	    for ( i = 0; i < 2; i++ )
		  theta[i+j*2] = 0.0;

	for ( i = 1; i < 2; i++ )
	{
		for ( i1 = 0; i1 <= i - 1; i1++ )
		  for ( j = 0; j < 3; j++ )
			theta[i1+j*2] += pow(x[i+j*3], 2);
	}
	for ( j = 0; j < 3; j++ )	{
		for ( i = 0; i < 3 - 2; i++ )
			theta[i+j*2] = theta[i+j*2] + pow( x[3-1+j*3], 2);
	}
	for ( j = 0; j < 3; j++ )
	  theta[j*2] = atan2(sqrt( theta[j*2] ), x[j*3] );

	//  Compute last component of THETA.
	top = new double[3];
	for ( j = 0; j < 3; j++ )
		top[j] = sqrt(pow( x[2+j*3], 2) + pow(x[1+j*3], 2)) + x[1+j*3];

	for(j = 0; j < 3; j++ )	{
		theta[1+j*2] = 2.0 * atan2(x[2+j*3], top[j]);
	}

	delete [] top;
	return;
}
*/