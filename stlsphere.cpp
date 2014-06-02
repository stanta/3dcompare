// Copyright (c) 2014 Andranik Abrahamyan

#include <math.h>
#include <algorithm>

#include "stlsphere.h"

#define M_PI     3.14159265358979323846
#define M_PI_2   1.57079632679489661923

bool sortByRadial (Facet a,Facet b)
{ 
	float ar, br;
	float ar1 = a.vector[0].x * a.vector[0].x + a.vector[0].y * a.vector[0].y +
					a.vector[0].z * a.vector[0].z; 
	float ar2 = a.vector[1].x * a.vector[1].x + a.vector[1].y * a.vector[1].y +
					a.vector[1].z * a.vector[1].z; 
	float ar3 = a.vector[2].x * a.vector[2].x + a.vector[2].y * a.vector[2].y +
					a.vector[2].z * a.vector[2].z; 

	float br1 = b.vector[0].x * b.vector[0].x + b.vector[0].y * b.vector[0].y +
					b.vector[0].z * b.vector[0].z; 
	float br2 = b.vector[1].x * b.vector[1].x + b.vector[1].y * b.vector[1].y +
					b.vector[1].z * b.vector[1].z; 
	float br3 = b.vector[2].x * b.vector[2].x + b.vector[2].y * b.vector[2].y +
					b.vector[2].z * b.vector[2].z; 

	if(ar1 < ar2)
		ar = ar1;
	else
		ar = ar2;
	if(ar3 < ar)
		ar = ar3;

	if(br1 < br2)
		br = br1;
	else
		br = br2;
	if(br3 < br)
		br = br3;
	
	if(ar < br)
		return true;
	else
		return false;
}


StlSphere::StlSphere(int devideSphere)
{
	m_devideSphere = devideSphere;
}

StlSphere::~StlSphere()
{
	close();
}

void StlSphere::close()
{
	m_vSpherical.clear();
}


void StlSphere::allocate(int numFacets)
{
	// Allocate memory 
	m_vSpherical.resize (numFacets);
}

int	StlSphere::getMinRadial(int ind)
{
	if( m_vSpherical[ind].radial[0] < m_vSpherical[ind].radial[1])
	{
		if( m_vSpherical[ind].radial[0] < m_vSpherical[ind].radial[2])
			return  0;
	} else {
		if( m_vSpherical[ind].radial[1] < m_vSpherical[ind].radial[2])
			return  1;			
	}
	return  2;
}

void	StlSphere::setUnitSphere(std::vector<Facet>* pfacet, Stl_Stats * stats)
{	
	// get the center of mass 
	facetMassCalc(pfacet, stats);
	// change mesh center to the mass center.
	m_maxRadial = 0;
	m_UnitStats.minRadial = 3.40282e+038;
	for (int i = 0; i < stats->numFacets ; i++)
	{
		pfacet->at(i).vector[0].x -= m_UnitStats.centerOfMass.x;
		pfacet->at(i).vector[0].y -= m_UnitStats.centerOfMass.y;
		pfacet->at(i).vector[0].z -= m_UnitStats.centerOfMass.z;
		pfacet->at(i).vector[1].x -= m_UnitStats.centerOfMass.x;
		pfacet->at(i).vector[1].y -= m_UnitStats.centerOfMass.y;
		pfacet->at(i).vector[1].z -= m_UnitStats.centerOfMass.z;
		pfacet->at(i).vector[2].x -= m_UnitStats.centerOfMass.x;
		pfacet->at(i).vector[2].y -= m_UnitStats.centerOfMass.y;
		pfacet->at(i).vector[2].z -= m_UnitStats.centerOfMass.z;
		
		// get minRadial and maxRadial without sqrt  
		setMinMaxRadial(pfacet->at(i).vector);
	}

	m_maxRadial = sqrt(m_maxRadial);
	m_UnitStats.minRadial = sqrt(m_UnitStats.minRadial) / m_maxRadial ;

	// sort the facets by ascending
	std::sort (pfacet->begin(), pfacet->end (), sortByRadial);

	// we have maxRadial, transform to unit mesh
	m_UnitStats.surface = 0;
	m_UnitStats.volume = 0;
	m_UnitStats.averageRadial = 0;
	for (int i = 0; i < stats->numFacets ; i++) 
	{
		pfacet->at(i).vector[0].x /= m_maxRadial;
		pfacet->at(i).vector[0].y /= m_maxRadial;
		pfacet->at(i).vector[0].z /= m_maxRadial;
		pfacet->at(i).vector[1].x /= m_maxRadial;
		pfacet->at(i).vector[1].y /= m_maxRadial;
		pfacet->at(i).vector[1].z /= m_maxRadial;
		pfacet->at(i).vector[2].x /= m_maxRadial;
		pfacet->at(i).vector[2].y /= m_maxRadial;
		pfacet->at(i).vector[2].z /= m_maxRadial;

		// set Spherical data 
		setSphericalItem(pfacet, i); 

		// calculate the average_radial
		m_UnitStats.averageRadial += m_vSpherical[i].radial[0] +
										m_vSpherical[i].radial[1] +
										m_vSpherical[i].radial[2];
		
		// get volume of the unit mesh 
		Facet fac = pfacet->at(i);
		float	currentVolume;
		currentVolume = (fac.vector[0].x * fac.vector[1].y * fac.vector[2].z -
						 fac.vector[0].x * fac.vector[2].y * fac.vector[1].z - 
						 fac.vector[1].x * fac.vector[0].y * fac.vector[2].z + 
						 fac.vector[1].x * fac.vector[2].y * fac.vector[0].z + 
						 fac.vector[2].x * fac.vector[0].y * fac.vector[1].z - 
						 fac.vector[2].x * fac.vector[1].y * fac.vector[0].z) / 6;

		m_UnitStats.volume += currentVolume;

		// get surface of the unit mesh 
		float area = getArea(&pfacet->at(i));
		m_UnitStats.surface += area;
	}
	m_UnitStats.averageRadial /= (3*stats->numFacets);
	
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
void	StlSphere::facetMassCalc(std::vector<Facet>* pFacets, Stl_Stats * stats)
{
    float totalVolume = 0, currentVolume;
    float xCenter = 0, yCenter = 0, zCenter = 0;
	Vector	facetCenter;
	Vector  totalCenter;

    for (int i = 0; i < stats->numFacets ; i++)
    {
		Facet fac = pFacets->at(i);
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

void	StlSphere::setMinMaxRadial(Vector* vector)
{
	for (int i = 0; i < 3; i++) {
		double s =  vector[i].x * vector[i].x + vector[i].y * vector[i].y + vector[i].z * vector[i].z;
		if (s > m_maxRadial)
			m_maxRadial = s;

		if (s < m_UnitStats.minRadial)
			m_UnitStats.minRadial = s;
	}
}

void	StlSphere::setNormalFrequency(float x, float y, float z)
{
	bool finded = false;
	NormalFrequency nf;
	int devideSphere = m_devideSphere/2;
	nf.x_pos = (short)(x*devideSphere);
	nf.y_pos = (short)(y*devideSphere);
	nf.z_pos = (short)(z*devideSphere);
	nf.frequency = 1;

	for( int i= 0; i < m_vNormalFreq.size (); i++)
	{
		if(m_vNormalFreq[i].x_pos ==  nf.x_pos &&
				m_vNormalFreq[i].y_pos ==  nf.y_pos &&
				m_vNormalFreq[i].z_pos ==  nf.z_pos)
		{
			m_vNormalFreq[i].frequency++;
			finded = true;
			break;
		}
	}
	if(!finded)
		m_vNormalFreq.push_back (nf);
	
}

void	StlSphere::writeAsNormalFrequency(const ::std::string& fileName)
{
	// Open the file, file have .nfr extention
	::std::ofstream file(fileName.c_str(), ::std::ios::out);
	file.setf(::std::ios::scientific);
	file.precision(8);
	if (file.is_open()) {
		file << "NormalFrequency" << ::std::endl;
		for (int i = 0; i < m_vNormalFreq.size() ; i++) {
			file << "x_pos =" << m_vNormalFreq[i].x_pos  << "\t"
				 << "y_pos =" << m_vNormalFreq[i].y_pos  << "\t"
				 << "z_pos =" << m_vNormalFreq[i].z_pos  << "\t"
				 << "frequency =" << m_vNormalFreq[i].frequency << ::std::endl;
		}
		file << "endNormalFrequency" << ::std::endl;
		file.close();
	} else {
		::std::cerr << "The file " << file << " could not be found." << ::std::endl;
	}

}


void	StlSphere::setSphericalItem(std::vector<Facet>* fac , int ind)
{
	Vector* vector = fac->at(ind).vector;
	for (int i = 0; i < 3; i++) {
		double s =  vector[i].x * vector[i].x + vector[i].y * vector[i].y + vector[i].z * vector[i].z;
		m_vSpherical[ind].radial[i] = sqrt(s);		
		m_vSpherical[ind].polar[i] = acos(vector[i].z / m_vSpherical[ind].radial[i]);
		m_vSpherical[ind].azimuth[i] = calcAtan(vector[i].x,vector[i].y);
	}
	setNormalFrequency(fac->at(ind).normal.x ,fac->at(ind).normal.y,fac->at(ind).normal.z);

/*
	// set normal value		
	float x  = fac->at(ind).normal.x ;
	float y  = fac->at(ind).normal.y ;
	float z  = fac->at(ind).normal.z  ;
	if (x > 0.95 || y > 0.95 || z > 0.95)
		int f = 4;
	//m_vSpherical[ind].normal.polar = acos(z / m_vSpherical[ind].radial[i]);
	// I will use atan for geting 0 - 2*Pi radians
	m_vSpherical[ind].normal.polar = calcAtan (sqrt(x*x + y*y), z);		
	m_vSpherical[ind].normal.azimuth = calcAtan(x, y);
*/
}

/*
theta^ = atan2(abs(y/x));
For theta in QI:    theta = theta^
For theta in QII:   theta= pi - theta^
For theta in QIII:  theta = pi + theta^
For theta in QIV:   theta = 2*pi - theta^ 
*/
float	StlSphere::calcAtan(float x, float y)
{
	float value;
	value = atan(abs(y /x));//For theta in QI:
	if (x == 0) {
		if (y > 0)
			value = M_PI_2;
		else 
			value = -M_PI_2;
	} else if (x > 0) {
		if (y < 0) //For theta in QIV: 
			value = 2*M_PI - value;
	} else {
		if (y > 0)//For theta in QII:
			value = M_PI - value;
		else // For theta in QIII
			value = M_PI + value;
	}
	return value;
}


float StlSphere::getArea(Facet *facet) 
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

void StlSphere::calculateNormal(float normal[], Facet *facet) 
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

void StlSphere::normalizeVector(float v[]) 
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
void StlSphere::cartesianToSphere(int m, int n, double c[], double x[],
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
