// Copyright (c) 2014 Andranik Abrahamyan

#include <QtGui/QtGui>

#include "propertiesgroupbox.h"

PropertiesGroupBox::PropertiesGroupBox(QWidget *parent)
    : QGroupBox(tr("Properties"), parent) {

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
  layout->addWidget(new QLabel("mm^3"), 0, 2);
  layout->addWidget(new QLabel("mm^2"), 1, 2);
  setLayout(layout);
}

PropertiesGroupBox::~PropertiesGroupBox() {}

void PropertiesGroupBox::reset() {
  // Reset values
  volume->setText("");
  surface->setText("");
}

void PropertiesGroupBox::setValues(const StlFile::Stats stats) {
  QString data;
  // Write values contained in stats
  data.setNum(stats.volume, 'f', 3);
  volume->setText(data);
  data.setNum(stats.surface, 'f', 3);
  surface->setText(data);
}
