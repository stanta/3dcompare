// Copyright (c) 2014 Andranik Abrahamyan

#include <QtCore/QtGlobal>
#include <QtGui/QApplication>
#include <QErrorMessage>
#include <math.h>
#include <string>
#include <algorithm>
#include <cctype>

#include "stlsearcher.h"

StlSearcher::StlSearcher()
{
	m_Spherical = 0;
}

StlSearcher::~StlSearcher()
{
	close();
}

void StlSearcher::close()
{
	if (m_Spherical != 0) {
		delete[] m_Spherical;
		m_Spherical = 0;
	}
}


void StlSearcher::allocate(int numFacets)
{
	// Allocate memory 
	m_Spherical = new Spherical[numFacets];
	if (m_Spherical == 0) {
		::std::cerr << "Problem allocating memory" << ::std::endl;
		throw ::std::bad_alloc();
	}
}

int	StlSearcher::getMinRadial(int ind)
{
	if( m_Spherical[ind].radial[0] < m_Spherical[ind].radial[1])
	{
		if( m_Spherical[ind].radial[0] < m_Spherical[ind].radial[2])
			return  0;
	} else {
		if( m_Spherical[ind].radial[1] < m_Spherical[ind].radial[2])
			return  1;			
	}
	return  2;
}

void	StlSearcher::setSphericalItem(Vector* vector, int ind)
{
	for (int i = 0; i < 3; i++) {
		double s =  vector[i].x * vector[i].x + vector[i].y * vector[i].y + vector[i].z * vector[i].z;
		m_Spherical[ind].radial[i] = sqrt(s);
		m_Spherical[ind].azimuth[i] = atan2(vector[i].y ,vector[i].x);
		m_Spherical[ind].polar[i] = acos(vector[i].z / m_Spherical[ind].radial[i]);
	}
	int a  = 7;

	/*
	double c[3];
	double x[9];
	double r[3];
	double theta[6];
	int i = 0;
	for(int j = 0; j < 3;  j++)
	{
		x[i] = vector[j].x;
		x[i+1] = vector[j].y;
		x[i+2] = vector[j].z;
		i+=3;
	}
	cartesianToSphere(3,3, c, x, r, theta );

	m_Spherical[ind].radial[0] = float(r[0]);
	m_Spherical[ind].radial[1] = float(r[1]);
	m_Spherical[ind].radial[2] = float(r[2]);

	float k[3];	
	k[0] = acos(x[2]/r[0]);
	k[1] = acos(x[5]/r[1]);
	k[2] = acos(x[8]/r[2]);
	
	float t[3];
	t[0] = ::atan2 (x[1],x[0]);
	t[1] = ::atan2 (x[4],x[3]);
	t[2] = ::atan2 (x[7],x[6]);

	//cartesianToSphere(3,1, c, x, r, theta );
	//m_Spherical[ind].polar[0] = 
	//m_Spherical[i].radial 
	double a = k[0]/theta[1];
	a = k[1]/theta[3];
	a = k[2]/theta[5];

*/

}
//  Purpose:
//    Cartesian to sphere coordinate transform.
//  Discussion:
//    We allow the trivial case M = 1; in that case alone, the value R must be assumed to have a sign.
//  Parameters:
//    Input, int M, the spatial dimension.
//    1 <= M.
//    Input, int N, the number of points to transform.
//    Input, double C[M], the center of the sphere.
//    Input, double X[M*N], the Cartesian coordinates of the points.
//    Output, double R[N], the radius of the points on the sphere. 
//		Except for the trivial case M = 1, R is assumed nonnegative.
//    Output, double THETA[(M-1)*N], the coordinate angles of the points, measured in radians.
void StlSearcher::cartesianToSphere(int m, int n, double c[], double x[],
									double r[],	double theta[] )
{
  int i;
  int i1;
  int j;
  double t;
  double *top;
  double *x2;
//
//  Handle special case of M = 1.
//
  if ( m == 1 )
  {
    for ( j = 0; j < n; j++ )
    {
      r[j] = x[0+j*m] - c[0];
    }
    return;
  }
//
//  Subtract the center.
//
  x2 = new double[ m * n ];
  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      x2[i+j*m] = x[i+j*m] - c[i];
    }
  }
//
//  Compute R.
//
  for ( j = 0; j < n; j++ )
  {
    t = 0.0;
    for ( i = 0; i < m; i++ )
    {
      t = t + pow ( x2[i+m*j], 2 );
    }
    r[j] = sqrt ( t );
  }
//
//  Compute M-2 components of THETA.
//
  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m - 1; i++ )
    {
      theta[i+j*(m-1)] = 0.0;
    }
  }

  for ( i = 1; i < m - 1; i++ )
  {
    for ( i1 = 0; i1 <= i - 1; i1++ )
    {
      for ( j = 0; j < n; j++ )
      {
        theta[i1+j*(m-1)] = theta[i1+j*(m-1)] + pow ( x2[i+j*m], 2 );
      }
    }
  }

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m - 2; i++ )
    {
      theta[i+j*(m-1)] = theta[i+j*(m-1)] + pow ( x2[m-1+j*m], 2 );
    }
  }

  for ( i = 0; i < m - 2; i++ )
  {
    for ( j = 0; j < n; j++ )
    {
      theta[i+j*(m-1)] = atan2 ( sqrt ( theta[i+j*(m-1)] ), x2[i+j*m] );
    }
  }
//
//  Compute last component of THETA.
//
  top = new double[n];

  for ( j = 0; j < n; j++ )
  {
    top[j] = sqrt ( pow ( x2[m-1+j*m], 2 ) + pow ( x2[m-2+j*m], 2 ) ) + x2[m-2+j*m];
  }

  for ( j = 0; j < n; j++ )
  {
    theta[m-2+j*(m-1)] = 2.0 * atan2 ( x2[m-1+j*m], top[j] );
  }

  delete [] top;
  delete [] x2;

  return;
}
