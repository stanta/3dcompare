// Copyright (c) 2014 Andranik Abrahamyan

#ifndef TYPES_H
#define TYPES_H

#include	<string>
#include	"vector.h"

struct Normal
{
	float x;
	float y;
	float z;
};
	
struct Facet
{
	Normal	normal;
	Vector	vector[3];
	char	extra[2];
};

enum Stl_Format
{
	ASCII,
	BINARY
};

struct Stl_Stats
{
	::std::string   header;
	Stl_Format      type;
	int             numFacets;
	int             numPoints;
	Vector          max;
	Vector          min;
	Vector          size;
	float           boundingDiameter;
	float           shortestEdge;
	float           volume;
	float           surface;
};
#endif  // TYPES_H
