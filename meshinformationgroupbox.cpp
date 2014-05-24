// Copyright (c) 2014 Andranik Abrahamyan

#include <QtGui/QtGui>

#include "meshinformationgroupbox.h"

MeshInformationGroupBox::MeshInformationGroupBox(QWidget *parent)
    : QGroupBox(tr("Mesh Information"), parent)
{
	QGridLayout *layout = new QGridLayout;
	// Write labels and values
	layout->addWidget(new QLabel("# Facets:"), 0, 0);
	numFacets = new QLabel("");
	numFacets->setAlignment(Qt::AlignRight);
	layout->addWidget(numFacets, 0, 1);
	layout->addWidget(new QLabel("# Points:"), 1, 0);
	numPoints = new QLabel("");
	numPoints->setAlignment(Qt::AlignRight);
	layout->addWidget(numPoints, 1, 1);
	setLayout(layout);
}

MeshInformationGroupBox::~MeshInformationGroupBox()
{}

void MeshInformationGroupBox::reset()
{
	// Reset values
	numFacets->setText("");
	numPoints->setText("");
}

void MeshInformationGroupBox::setValues(const Stl_Stats stats)
{
	QString data;
	// Write values contained in stats
	 data.setNum(stats.numFacets);
	numFacets->setText(data);
	data.setNum(stats.numPoints);
	numPoints->setText(data);
}
