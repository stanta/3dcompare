// Copyright (c) 2014 Andranik Abrahamyan

//#include <QtCore/QtGlobal>
//#include <QtGui/QApplication>
//#include <QErrorMessage>
#include <math.h>
#include <string>
#include <algorithm>
#include <cctype>

#include "stlsearcher.h"

StlSearcher::StlSearcher()
{
	m_Spherical = 0;
	m_maxRadial = 0;
//	m_maxRadial = 0;
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

void	StlSearcher::setUnitSphere(Facet * facet, Stl_Stats * stats)
{
	// get 
	facetMassCalc(facet, stats);
	// change mesh center to the mass center.
	for (int i = 0; i < stats->numFacets ; i++)
	{
		facet[i].vector[0].x -= m_UnitStats.centerOfMass.x;
		facet[i].vector[0].y -= m_UnitStats.centerOfMass.y;
		facet[i].vector[0].z -= m_UnitStats.centerOfMass.z;
		facet[i].vector[1].x -= m_UnitStats.centerOfMass.x;
		facet[i].vector[1].y -= m_UnitStats.centerOfMass.y;
		facet[i].vector[1].z -= m_UnitStats.centerOfMass.z;
		facet[i].vector[2].x -= m_UnitStats.centerOfMass.x;
		facet[i].vector[2].y -= m_UnitStats.centerOfMass.y;
		facet[i].vector[2].z -= m_UnitStats.centerOfMass.z;
		
		// set Spherical Item data and get m_maxRadial
		setSphericalItem(facet[i].vector, i); 
	}

	// we have m_maxRadial, transform to unit mesh
	m_UnitStats.surface = 0;
	m_UnitStats.volume = 0;
	for (int i = 0; i < stats->numFacets ; i++)
	{
		facet[i].vector[0].x /= m_maxRadial;
		facet[i].vector[0].y /= m_maxRadial;
		facet[i].vector[0].z /= m_maxRadial;
		facet[i].vector[1].x /= m_maxRadial;
		facet[i].vector[1].y /= m_maxRadial;
		facet[i].vector[1].z /= m_maxRadial;
		facet[i].vector[2].x /= m_maxRadial;
		facet[i].vector[2].y /= m_maxRadial;
		facet[i].vector[2].z /= m_maxRadial;
		
		// get volume of the unit mesh 
		Facet fac = facet[i];
		float	currentVolume;
		currentVolume = (fac.vector[0].x * fac.vector[1].y * fac.vector[2].z -
						 fac.vector[0].x * fac.vector[2].y * fac.vector[1].z - 
						 fac.vector[1].x * fac.vector[0].y * fac.vector[2].z + 
						 fac.vector[1].x * fac.vector[2].y * fac.vector[0].z + 
						 fac.vector[2].x * fac.vector[0].y * fac.vector[1].z - 
						 fac.vector[2].x * fac.vector[1].y * fac.vector[0].z) / 6;

		m_UnitStats.volume += currentVolume;

		// get surface of the unit mesh 
		float area = getArea(&facet[i]);
		m_UnitStats.surface += area;
	}

	if (m_UnitStats.surface < 0.0) 
		m_UnitStats.surface = -m_UnitStats.surface;
	// set min,max,size, boundingDiameter values 
	m_UnitStats.min = stats->min /m_maxRadial;
	m_UnitStats.max = stats->max /m_maxRadial;
	m_UnitStats.size = stats->size /m_maxRadial;
	//m_UnitStats.boundingDiameter =  sqrt(m_UnitStats.size.x * m_UnitStats.size.x +
      //                             m_UnitStats.size.y * m_UnitStats.size.y +
        //                           m_UnitStats.size.z * m_UnitStats.size.z);

	//implement this logic: box_length < box_width < box_height
	if(m_UnitStats.size.x < m_UnitStats.size.y){
		if(m_UnitStats.size.x < m_UnitStats.size.z){
			m_UnitStats.box_length = m_UnitStats.size.x;
			if(m_UnitStats.size.y < m_UnitStats.size.z) {
				m_UnitStats.box_width = m_UnitStats.size.y;
				m_UnitStats.box_height = m_UnitStats.size.z;
			}else {
				m_UnitStats.box_width = m_UnitStats.size.z;
				m_UnitStats.box_height = m_UnitStats.size.y;
			}
		}else {
			m_UnitStats.box_length = m_UnitStats.size.z;
			m_UnitStats.box_width = m_UnitStats.size.x;
			m_UnitStats.box_height = m_UnitStats.size.y;
		}
	} else  {
		if(m_UnitStats.size.y < m_UnitStats.size.z){
			m_UnitStats.box_length = m_UnitStats.size.y;
			if(m_UnitStats.size.x < m_UnitStats.size.z){
				m_UnitStats.box_width = m_UnitStats.size.x;
				m_UnitStats.box_height = m_UnitStats.size.z;
			} else {
				m_UnitStats.box_width = m_UnitStats.size.z;
				m_UnitStats.box_height = m_UnitStats.size.x;
			}
		}else{
			m_UnitStats.box_length = m_UnitStats.size.z;
			m_UnitStats.box_width = m_UnitStats.size.y;
			m_UnitStats.box_height = m_UnitStats.size.x;
		}
	}
}

// facets total mass and center of mass calculation
void	StlSearcher::facetMassCalc(Facet * pFacets, Stl_Stats * stats)
{
    double totalVolume = 0, currentVolume;
    double xCenter = 0, yCenter = 0, zCenter = 0;
	Vector	facetCenter;
	Vector  totalCenter;

    for (int i = 0; i < stats->numFacets ; i++)
    {
		Facet fac = pFacets[i];
		currentVolume = (fac.vector[0].x * fac.vector[1].y * fac.vector[2].z -
						 fac.vector[0].x * fac.vector[2].y * fac.vector[1].z - 
						 fac.vector[1].x * fac.vector[0].y * fac.vector[2].z + 
						 fac.vector[1].x * fac.vector[2].y * fac.vector[0].z + 
						 fac.vector[2].x * fac.vector[0].y * fac.vector[1].z - 
						 fac.vector[2].x * fac.vector[1].y * fac.vector[0].z) / 6;

		totalVolume += currentVolume;
        xCenter += ((fac.vector[0].x + fac.vector[1].x + fac.vector[2].x) / 4) * currentVolume;
        yCenter += ((fac.vector[0].y + fac.vector[1].y + fac.vector[2].y) / 4) * currentVolume;
        zCenter += ((fac.vector[0].z + fac.vector[1].z + fac.vector[2].z) / 4) * currentVolume;
    }
	m_UnitStats.volume = totalVolume;
	m_UnitStats.centerOfMass.x = xCenter/totalVolume;
	m_UnitStats.centerOfMass.y = yCenter/totalVolume;
	m_UnitStats.centerOfMass.z = zCenter/totalVolume;
}

void	StlSearcher::setSphericalItem(Vector* vector, int ind)
{
	for (int i = 0; i < 3; i++) {
		double s =  vector[i].x * vector[i].x + vector[i].y * vector[i].y + vector[i].z * vector[i].z;
		m_Spherical[ind].radial[i] = sqrt(s);
		m_Spherical[ind].azimuth[i] = atan2(vector[i].y ,vector[i].x);
		m_Spherical[ind].polar[i] = acos(vector[i].z / m_Spherical[ind].radial[i]);
		if (m_Spherical[ind].radial[i] > m_maxRadial)
			m_maxRadial = m_Spherical[ind].radial[i];
	}
}


float StlSearcher::getArea(Facet *facet) 
{
	float cross[3][3];
	float sum[3];
	float n[3];
	for (int i = 0; i < 3; i++) {
		cross[i][0] = ((facet->vector[i].y * facet->vector[(i + 1) % 3].z) -
		(facet->vector[i].z * facet->vector[(i + 1) % 3].y));
		cross[i][1] = ((facet->vector[i].z * facet->vector[(i + 1) % 3].x) -
		(facet->vector[i].x * facet->vector[(i + 1) % 3].z));
		cross[i][2] = ((facet->vector[i].x * facet->vector[(i + 1) % 3].y) -
		(facet->vector[i].y * facet->vector[(i + 1) % 3].x));
	}
	sum[0] = cross[0][0] + cross[1][0] + cross[2][0];
	sum[1] = cross[0][1] + cross[1][1] + cross[2][1];
	sum[2] = cross[0][2] + cross[1][2] + cross[2][2];
	// This should already be done.  But just in case, let's do it again
	calculateNormal(n, facet);
	normalizeVector(n);
	float area = 0.5 * (n[0] * sum[0] + n[1] * sum[1] + n[2] * sum[2]);
	return area;
}

void StlSearcher::calculateNormal(float normal[], Facet *facet) 
{
	float v1[3];
	float v2[3];
	v1[0] = facet->vector[1].x - facet->vector[0].x;
	v1[1] = facet->vector[1].y - facet->vector[0].y;
	v1[2] = facet->vector[1].z - facet->vector[0].z;
	v2[0] = facet->vector[2].x - facet->vector[0].x;
	v2[1] = facet->vector[2].y - facet->vector[0].y;
	v2[2] = facet->vector[2].z - facet->vector[0].z;
	normal[0] = (float)((double)v1[1] * (double)v2[2])
		          - ((double)v1[2] * (double)v2[1]);
	normal[1] = (float)((double)v1[2] * (double)v2[0])
		          - ((double)v1[0] * (double)v2[2]);
	normal[2] = (float)((double)v1[0] * (double)v2[1])
			      - ((double)v1[1] * (double)v2[0]);
}

void StlSearcher::normalizeVector(float v[]) 
{
	double length = sqrt((double)v[0] * (double)v[0]
                       + (double)v[1] * (double)v[1]
                          + (double)v[2] * (double)v[2]);
	float minNormalLength = 0.000000000001f;
	if (length < minNormalLength) {
		v[0] = 1.0;
		v[1] = 0.0;
		v[2] = 0.0;
		return;
	}  
	double factor = 1.0 / length;
	v[0] *= factor;
	v[1] *= factor;
	v[2] *= factor;
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
