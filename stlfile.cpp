// Copyright (c) 2014 Andranik Abrahamyan

#include <math.h>
#include <algorithm>

#ifdef QT_CORE_LIB
	#include <QtCore/QtGlobal>
	#include <QtGui/QApplication>
	#include <QErrorMessage>
#endif //QT_CORE_LIB

#include "stlfile.h"
#include "stlsphere.h"

#define HEADER_SIZE 84
#define JUNK_SIZE 80
#define SIZE_OF_FACET 50
#define ASCII_LINES_PER_FACET 7

StlFile::StlFile(StlSphere* stlSphere)
{
	m_stlSphere = stlSphere;
}

StlFile::~StlFile()
{
  close();
}

void StlFile::open(const ::std::string& fileName)
{
	initialize(fileName);
	allocate();
	readData(0, 1);
	m_file.close();
}

void StlFile::write(const ::std::string& fileName)
{
	if (m_vFacets.size ()> 0) 
	{
		if (m_stats.type == ASCII)
			writeAscii(fileName);
		else
			writeBinary(fileName);
	}
}

void StlFile::close()
{
	//if (m_vFacets != 0) {
	//	delete[] m_vFacets;
	//	m_vFacets = 0;
	//}
	m_vFacets.clear ();
}

void StlFile::setFormat(const int format)
{
  if (format == ASCII)
		m_stats.type = ASCII;
  else if (format == BINARY)
		m_stats.type = BINARY;
}

void StlFile::initialize(const ::std::string& fileName)
{
	m_stats.numFacets = 0;
	m_stats.numPoints = 0;
	m_stats.surface = -1.0;
	m_stats.volume = -1.0;
	//m_vFacets = 0;
	// Open the file
	m_file.open(fileName.c_str(), ::std::ios::binary);
	if (m_file.is_open()) {
		int numFacets;
		// Find length of file
		m_file.seekg(0, ::std::ios::end);
		int fileSize = m_file.tellg();
		// Check for binary or ASCII file
		m_file.seekg(0, ::std::ios::beg);
		m_stats.type = BINARY;
		int c;
		while((c = m_file.get()) != EOF && c <= 127) 
        ;
		if(c == EOF) {
			m_stats.type = ASCII;
		}
		m_file.seekg(0, ::std::ios::beg);
		// Get the header and the number of facets in the .STL file 
		// If the .STL file is binary, then do the following 
		if (m_stats.type == BINARY) {
			// Test if the STL file has the right size
			if ((fileSize - HEADER_SIZE) % SIZE_OF_FACET != 0) {
				::std::cerr << "The file " << m_file << " has a wrong size."
                    << ::std::endl;
				throw wrong_header_size();
			}
			numFacets = (fileSize - HEADER_SIZE) / SIZE_OF_FACET;
			// Read the header
			char buffer[JUNK_SIZE];
			m_file.read(buffer, JUNK_SIZE);
			m_stats.header = buffer;
			// Read the int following the header.
			// This should contain the number of facets
			int headerNumFacets = readIntFromBytes(m_file);
			if (numFacets != headerNumFacets) {
				::std::cerr << "Warning: File size doesn't match number of "
                    << "facets in the header." << ::std::endl;
#ifdef QT_CORE_LIB
				QErrorMessage errMessage;
				errMessage.showMessage("File size doesn't match number of facets "
                               "in the header.");
				QApplication::restoreOverrideCursor();
				errMessage.exec();
				QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
			}
		}
		else {  // Otherwise, if the .STL file is ASCII, then do the following
			m_file.seekg(0, ::std::ios::beg);
			// Get the header
			getline(m_file, m_stats.header);
			// Find the number of facets
			int numLines = 0;
			::std::string line;
			while (!getline(m_file, line).eof()) {
				if (line.size() > 4) {  // don't count short lines
					numLines++;
				}
			}
			m_file.seekg(0, ::std::ios::beg);
			numFacets = numLines / ASCII_LINES_PER_FACET;
		}
		m_stats.numFacets += numFacets;
	} else {
		::std::cerr << "The file " << m_file << " could not be found." << ::std::endl;
		throw error_opening_file();
	}
}

void StlFile::allocate()
{
	// Allocate memory for the entire .STL file
	//m_vFacets = new Facet[m_stats.numFacets];
	//if (m_vFacets == 0) {
	//	::std::cerr << "Problem allocating memory" << ::std::endl;
	//	throw ::std::bad_alloc();
	//}
	m_vFacets.resize (m_stats.numFacets);
	m_stlSphere ->allocate(m_stats.numFacets);
}

void StlFile::readData(int firstFacet, int first)
{
	if (m_stats.type == BINARY) {
		m_file.seekg(HEADER_SIZE, ::std::ios::beg);
	} else {
		m_file.seekg(0, ::std::ios::beg);
		::std::string line;
		// Skip the first line of the file
		getline(m_file, line);
	}
	Facet facet;
	for (int i = firstFacet; i < m_stats.numFacets; i++)
	{
		if (m_stats.type == BINARY) {  // Read a single facet from a binary .STL file
			facet.normal.x = readFloatFromBytes(m_file);
			facet.normal.y = readFloatFromBytes(m_file);
			facet.normal.z = readFloatFromBytes(m_file);
			facet.vector[0].x = readFloatFromBytes(m_file);
			facet.vector[0].y = readFloatFromBytes(m_file);
			facet.vector[0].z = readFloatFromBytes(m_file);
			facet.vector[1].x = readFloatFromBytes(m_file);
			facet.vector[1].y = readFloatFromBytes(m_file);
			facet.vector[1].z = readFloatFromBytes(m_file);
			facet.vector[2].x = readFloatFromBytes(m_file);
			facet.vector[2].y = readFloatFromBytes(m_file);
			facet.vector[2].z = readFloatFromBytes(m_file);
			facet.extra[0] = m_file.get();
			facet.extra[1] = m_file.get();
		} else {  // Read a single facet from an ASCII .STL file
			::std::string junk;
			m_file >> junk >> junk;
			m_file >> facet.normal.x >> facet.normal.y >> facet.normal.z;
			m_file >> junk >> junk >> junk;
			m_file >> facet.vector[0].x >> facet.vector[0].y >> facet.vector[0].z;
			m_file >> junk;
			m_file >> facet.vector[1].x >> facet.vector[1].y >> facet.vector[1].z;
			m_file >> junk;
			m_file >> facet.vector[2].x >> facet.vector[2].y >> facet.vector[2].z;
			m_file >> junk >> junk;
		}
		// Write the facet into memory.
		m_vFacets[i] = facet;

		// While we are going through all of the facets, let's find the
		// maximum and minimum values for x, y, and z
		// Initialize the max and min values the first time through
		if (first) {
			m_stats.max.x = facet.vector[0].x;
			m_stats.min.x = facet.vector[0].x;
			m_stats.max.y = facet.vector[0].y;
			m_stats.min.y = facet.vector[0].y;
			m_stats.max.z = facet.vector[0].z;
			m_stats.min.z = facet.vector[0].z;
	  	  
			float xDiff = std::abs(facet.vector[0].x - facet.vector[1].x);
			float yDiff = std::abs(facet.vector[0].y - facet.vector[1].y);
			float zDiff = std::abs(facet.vector[0].z - facet.vector[1].z);
			float maxDiff = std::max(xDiff, yDiff);
			maxDiff = std::max(zDiff, maxDiff);
			m_stats.shortestEdge = maxDiff;
			first = 0;
		}
		// Now find the max and min values
		m_stats.max.x = std::max(m_stats.max.x, facet.vector[0].x);
		m_stats.min.x = std::min(m_stats.min.x, facet.vector[0].x);
		m_stats.max.y = std::max(m_stats.max.y, facet.vector[0].y);
		m_stats.min.y = std::min(m_stats.min.y, facet.vector[0].y);
		m_stats.max.z = std::max(m_stats.max.z, facet.vector[0].z);
		m_stats.min.z = std::min(m_stats.min.z, facet.vector[0].z);

		m_stats.max.x = std::max(m_stats.max.x, facet.vector[1].x);
		m_stats.min.x = std::min(m_stats.min.x, facet.vector[1].x);
		m_stats.max.y = std::max(m_stats.max.y, facet.vector[1].y);
		m_stats.min.y = std::min(m_stats.min.y, facet.vector[1].y);
		m_stats.max.z = std::max(m_stats.max.z, facet.vector[1].z);
		m_stats.min.z = std::min(m_stats.min.z, facet.vector[1].z);

		m_stats.max.x = std::max(m_stats.max.x, facet.vector[2].x);
		m_stats.min.x = std::min(m_stats.min.x, facet.vector[2].x);
		m_stats.max.y = std::max(m_stats.max.y, facet.vector[2].y);
		m_stats.min.y = std::min(m_stats.min.y, facet.vector[2].y);
		m_stats.max.z = std::max(m_stats.max.z, facet.vector[2].z);
		m_stats.min.z = std::min(m_stats.min.z, facet.vector[2].z);
	}
	m_stats.size.x = m_stats.max.x - m_stats.min.x;
	m_stats.size.y = m_stats.max.y - m_stats.min.y;
	m_stats.size.z = m_stats.max.z - m_stats.min.z;
	m_stats.boundingDiameter =  sqrt(m_stats.size.x * m_stats.size.x +
                                   m_stats.size.y * m_stats.size.y +
                                   m_stats.size.z * m_stats.size.z);
	m_stats.numPoints = getNumPoints();
	//m_stats.surface = getSurface();
	//m_stats.volume = getVolume();
	// Set unit Sphere data(cennter of mass ...)
	m_stlSphere->setUnitSphere(&m_vFacets, &m_stats);
}

int StlFile::readIntFromBytes(::std::ifstream& file) 
{
	int value;
	value  =  file.get() & 0xFF;
	value |= (file.get() & 0xFF) << 0x08;
	value |= (file.get() & 0xFF) << 0x10;
	value |= (file.get() & 0xFF) << 0x18;
	return(value);
}

float StlFile::readFloatFromBytes(::std::ifstream& file) 
{
	union 
	{
		int intValue;
		float floatValue;
	} value;

	value.intValue  =  file.get() & 0xFF;
	value.intValue |= (file.get() & 0xFF) << 0x08;
	value.intValue |= (file.get() & 0xFF) << 0x10;
	value.intValue |= (file.get() & 0xFF) << 0x18;
	return(value.floatValue);
}

void StlFile::writeBytesFromInt(::std::ofstream& file, int valueIn)
{
	union {
		int intValue;
		char charValue[4];
	} value;
	value.intValue = valueIn;
	int newValue  = value.charValue[0] & 0xFF;
	newValue |= (value.charValue[1] & 0xFF) << 0x08;
	newValue |= (value.charValue[2] & 0xFF) << 0x10;
	newValue |= (value.charValue[3] & 0xFF) << 0x18;
	file.write(reinterpret_cast<char *>(&newValue), sizeof(newValue));
}

void StlFile::writeBytesFromFloat(::std::ofstream& file, float valueIn)
{
	union {
		float floatValue;
		char charValue[4];
	} value;
	value.floatValue = valueIn;
	int newValue  = value.charValue[0] & 0xFF;
	newValue |= (value.charValue[1] & 0xFF) << 0x08;
	newValue |= (value.charValue[2] & 0xFF) << 0x10;
	newValue |= (value.charValue[3] & 0xFF) << 0x18;
	file.write(reinterpret_cast<char *>(&newValue), sizeof(newValue));
}

void StlFile::writeBinary(const ::std::string& fileName)
{
	// Open the file
	::std::ofstream file(fileName.c_str(), ::std::ios::out|::std::ios::binary);
	if (file.is_open()) {
		for (int i = 0; i < JUNK_SIZE; i++) file.put(0);
		writeBytesFromInt(file, m_stats.numFacets);
		for (int i = 0; i < m_stats.numFacets; i++) {
			writeBytesFromFloat(file, m_vFacets[i].normal.x);
			writeBytesFromFloat(file, m_vFacets[i].normal.y);
			writeBytesFromFloat(file, m_vFacets[i].normal.z);
			writeBytesFromFloat(file, m_vFacets[i].vector[0].x);
			writeBytesFromFloat(file, m_vFacets[i].vector[0].y);
			writeBytesFromFloat(file, m_vFacets[i].vector[0].z);
			writeBytesFromFloat(file, m_vFacets[i].vector[1].x);
			writeBytesFromFloat(file, m_vFacets[i].vector[1].y);
			writeBytesFromFloat(file, m_vFacets[i].vector[1].z);
			writeBytesFromFloat(file, m_vFacets[i].vector[2].x);
			writeBytesFromFloat(file, m_vFacets[i].vector[2].y);
			writeBytesFromFloat(file, m_vFacets[i].vector[2].z);
			file << m_vFacets[i].extra[0];
			file << m_vFacets[i].extra[1];
		}
		file.close();
	} else {
		::std::cerr << "The file " << file << " could not be found." << ::std::endl;
		throw error_opening_file();
	}
}


void	StlFile::writeAsSphere(const ::std::string& fileName)
{
	// Open the file
	::std::ofstream file(fileName.c_str(), ::std::ios::out);
	file.setf(::std::ios::scientific);
	file.precision(8);
	if (file.is_open()) {
		file << "solid" << ::std::endl;
		for (int i = 0; i < m_stats.numFacets; i++) {
			file << "  facet normal " << m_vFacets[i].normal.x << " "
				<< m_vFacets[i].normal.y << " " << m_vFacets[i].normal.z << ::std::endl;
			file << "    outer loop Spherical" << ::std::endl;
			StlSphere::Spherical sp = m_stlSphere->getSpherical(i);
			file << "       radial: " << sp.radial[0]  << "  azimuth: "
				<< sp.azimuth[0]  << "  polar: " << sp.polar[0]  << ::std::endl;
			
			file << "       radial: " << sp.radial[1]  << "  azimuth: "
				<< sp.azimuth[1]  << "  polar: " << sp.polar[1]  << ::std::endl;
			file << "       radial: " << sp.radial[2]  << "  azimuth: "
				<< sp.azimuth[2]  << "  polar: " << sp.polar[2]  << ::std::endl;

			file << "    endloop Spherical" << ::std::endl;
			file << "  endSpherical" << ::std::endl;
		}
		file << "endsolid" << ::std::endl;
		file.close();
	} else {
		::std::cerr << "The file " << file << " could not be found." << ::std::endl;
		throw error_opening_file();
	}

}


void StlFile::writeAscii(const ::std::string& fileName) {
	// Open the file
	::std::ofstream file(fileName.c_str(), ::std::ios::out);
	file.setf(::std::ios::scientific);
	file.precision(8);
	if (file.is_open()) {
		file << "solid" << ::std::endl;
		for (int i = 0; i < m_stats.numFacets; i++) {
			file << "  facet normal " << m_vFacets[i].normal.x << " "
				<< m_vFacets[i].normal.y << " " << m_vFacets[i].normal.z << ::std::endl;
			file << "    outer loop " << ::std::endl;
			file << "      Vector " << m_vFacets[i].vector[0].x << " "
				<< m_vFacets[i].vector[0].y << " " << m_vFacets[i].vector[0].z << ::std::endl;
			file << "      Vector " << m_vFacets[i].vector[1].x << " " 
				<< m_vFacets[i].vector[1].y << " " << m_vFacets[i].vector[1].z << ::std::endl;
			file << "      Vector " << m_vFacets[i].vector[2].x << " "
				<< m_vFacets[i].vector[2].y << " " << m_vFacets[i].vector[2].z << ::std::endl;
			file << "    endloop" << ::std::endl;
			file << "  endfacet" << ::std::endl;
		}
		file << "endsolid" << ::std::endl;
		file.close();
	} else {
		::std::cerr << "The file " << file << " could not be found." << ::std::endl;
		throw error_opening_file();
	}
  /*FILE *fp = fopen(fileName.c_str(), "w");
  fprintf(fp, "solid\n");
  for (int i = 0; i < stats.numFacets; i++) {
    fprintf(fp, "  facet normal % 0.8f % 0.8f % 0.8f\n",
    facets[i].normal.x, facets[i].normal.y,
    facets[i].normal.z);
    fprintf(fp, "    outer loop\n");
    fprintf(fp, "      vertex % 0.8f % 0.8f % 0.8f\n",
    facets[i].vector[0].x, facets[i].vector[0].y,
    facets[i].vector[0].z);
    fprintf(fp, "      vertex % 0.8f % 0.8f % 0.8f\n",
    facets[i].vector[1].x, facets[i].vector[1].y,
    facets[i].vector[1].z);
    fprintf(fp, "      vertex % 0.8f % 0.8f % 0.8f\n",
    facets[i].vector[2].x, facets[i].vector[2].y,
    facets[i].vector[2].z);
    fprintf(fp, "    endloop\n");
    fprintf(fp, "  endfacet\n");
  }
  fprintf(fp, "endsolid\n");
  fclose(fp);*/
}

static bool compareVectors(Vector i, Vector j)
{
	Vector diff;
	diff = i - j;
	return (diff.Magnitude() < 0);
}

static bool equalVectors(Vector i, Vector j)
{
	Vector diff;
	diff = i - j;
	return (diff.Magnitude() == 0);
}

int StlFile::getNumPoints()
{
	::std::vector<Vector> vectors;
	for (int i = 0; i < m_stats.numFacets; i++) {
		for (int j = 0; j < 3; j++) {
			vectors.push_back(m_vFacets[i].vector[j]);
		}
	}
	::std::sort(vectors.begin(), vectors.end(), compareVectors);
	::std::unique(vectors.begin(), vectors.end(), equalVectors);
	return vectors.size();
}
/*
float StlFile::getVolume()
{
	Vector p0;
	Vector p;
	float volume = 0.0;
	// Choose a point, any point as the reference
	p0.x = m_vFacets[0].vector[0].x;
	p0.y = m_vFacets[0].vector[0].y;
	p0.z = m_vFacets[0].vector[0].z;
	for (int i = 0; i < m_stats.numFacets; i++) {
		p.x = m_vFacets[i].vector[0].x - p0.x;
		p.y = m_vFacets[i].vector[0].y - p0.y;
		p.z = m_vFacets[i].vector[0].z - p0.z;
		// Do dot product to get distance from point to plane
		Normal n = m_vFacets[i].normal;
		float height = (n.x * p.x) + (n.y * p.y) + (n.z * p.z);
		float area = getArea(&m_vFacets[i]);
		volume += (area * height) / 3.0;
	}
	if (volume < 0.0) 
		volume = -volume;

	return volume;
}
*/
/*
float StlFile::getSurface()
{
	float surface = 0.0;
	for (int i = 0; i < m_stats.numFacets; i++) {
		float area = getArea(&m_vFacets[i]);
		surface += area;
	}
	if (surface < 0.0) 
		surface = -surface;

	return surface;
}
*/