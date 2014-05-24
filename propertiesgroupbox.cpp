// Copyright (c) 2014 Andranik Abrahamyan

#include <QtGui/QtGui>
#include "propertiesgroupbox.h"

PropertiesGroupBox::PropertiesGroupBox(QWidget *parent)
    : QGroupBox(tr("Properties"), parent)
{
	QGridLayout *layout = new QGridLayout;
	// Write labels and values
	layout->addWidget(new QLabel("Volume:"), 0, 0);
	volume = new QLabel("");
	volume->setAlignment(Qt::AlignRight);
	layout->addWidget(volume, 0, 1);
	layout->addWidget(new QLabel("Surface:"), 1, 0);
	surface = new QLabel("");
	surface->setAlignment(Qt::AlignRight);
	layout->addWidget(surface, 1, 1);

	layout->addWidget(new QLabel("boundingDiameter:"), 2, 0);
	diameter = new QLabel("");
	diameter->setAlignment(Qt::AlignRight);
	layout->addWidget(diameter, 2, 1);

	layout->addWidget(new QLabel("mm^3"), 0, 2);
	layout->addWidget(new QLabel("mm^2"), 1, 2);
	layout->addWidget(new QLabel("mm"), 2, 2);

	setLayout(layout);
}

PropertiesGroupBox::~PropertiesGroupBox()
{}

void PropertiesGroupBox::reset()
{
	// Reset values
	volume->setText("");
	surface->setText("");
	diameter->setText("");
}

void PropertiesGroupBox::setValues(const StlSearcher::UnitStats stats)
{
	QString data;
	// Write values contained in stats
	data.setNum(stats.volume, 'f', 5);
	volume->setText(data);
	data.setNum(stats.surface, 'f', 5);
	surface->setText(data);
	data.setNum(stats.boundingDiameter , 'f', 5);
	diameter->setText(data);	
}
