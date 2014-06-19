// Copyright (c) 2014 Andranik Abrahamyan
#ifndef TYPES_H
#define TYPES_H

#include	<string>
#include	"vector.h"

/*! \brief  The 3d normal.
*
*	The normal of the face . */
struct Normal 
{
	float x; //!<  x coordinate of the normal
	float y; //!<  y coordinate of the normal
	float z; //!<  z coordinate of the normal
};

/*! \brief  Facet parameters .
*
*	Facet parameters of the stl file . */
struct Facet
{
	Normal	normal;	//!<  normal of the facet
	Vector	vector[3]; //!<  coordinates of the triangle 
	char	extra[2];  //!<  extra data, not used
};

enum Stl_Format //!<  Stl file format (Asscii, or binary)
{
	ASCII, //!<  Stl file format is ASCII
	BINARY //!<  Stl file format is BINARY
};

/*! \brief  Stl file stats .
*
*	Used for calculating the 3D mesh(.stl format) properties. */
struct Stl_Stats 
{
	::std::string   header;	//!<  Stl file header
	Stl_Format      type; //!<  Stl file format 
	int             numFacets; //!<  number of facetes in the stl file
	int             numPoints; //!<  number of points in the stl file
	Vector          max;		//!<   maximal size of the distance
	Vector          min;		//!<   minimal size of the distance
	Vector          size;		//!<   size of the distance
	float           boundingDiameter; //!< maximal bounding diameter in the stl 
	float           shortestEdge;  //!<  shortest edge in the stl 
	float           volume; //!<  volume value in the stl 
	float           surface; //!<  surface value in the stl
};
#endif  // TYPES_H
