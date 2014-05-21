// Copyright (c) 2014 Andranik Abrahamyan

#ifndef STLStlSearcher_H
#define STLStlSearcher_H

#include <iostream>
#include <fstream>
#include <exception>

#include "vector.h"

class StlSearcher
{
public:
	typedef struct {
		float	radial[3];
		float	polar[3];
		float	azimuth[3];
	} Spherical;
	
	StlSearcher();
	~StlSearcher();

	Spherical*	getSpherical() const { return m_Spherical; };
	void	allocate(int numFacets);
	void	close();
	void	setSphericalItem(Vector* vector, int i);
	int		getMinRadial(int index);

private:
	void cartesianToSphere(int m, int n, double c[], double x[], double r[], double theta[]);

	Spherical * m_Spherical;
};

#endif  // STLStlSearcher_H
/*


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