/*
 * Copyright (C) 2016, Volker Waurich
 *
 * This file is part of OMVis.
 *
 * OMVis is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OMVis is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OMVis.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "View/Dialogs/SimSettingDialog.hpp"

#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>
#include <QDialogButtonBox>
#include <QLineEdit>

namespace OMVIS
{
    namespace View
    {
		SimSettingDialog::SimSettingDialog(QWidget* parent)
			: QDialog(parent),
			_solverMethodBox(new QComboBox()),
			_simStepSize(new QLineEdit("0.0001")),
			_visStepSize(new QLineEdit("0.1"))

        {
            // Perspective
			_solverMethodBox->addItem(QString("forward euler"));

            QLabel* solverMethodLabel = new QLabel(tr("Solver Method: "));
            QHBoxLayout* solverLayout = new QHBoxLayout();
			solverLayout->addWidget(solverMethodLabel);
			solverLayout->addWidget(_solverMethodBox);

            QHBoxLayout* simStepLayout = new QHBoxLayout();
			QLabel* simStepLabel = new QLabel(tr("Simulation Step Size: "));
			simStepLayout->addWidget(simStepLabel);
			simStepLayout->addWidget(_simStepSize);
			QHBoxLayout* visStepLayout = new QHBoxLayout();
			QLabel* visStepLabel = new QLabel(tr("Visualization Step Size: "));
			visStepLayout->addWidget(visStepLabel);
			visStepLayout->addWidget(_visStepSize);

            // Main layout
            QVBoxLayout* mainLayout = new QVBoxLayout();
            mainLayout->addLayout(solverLayout);
            mainLayout->addLayout(simStepLayout);
			mainLayout->addLayout(visStepLayout);
            mainLayout->addStretch(1);
            setLayout(mainLayout);
            setWindowTitle(tr("Simulation Settings"));
        }

    }  // End namespace View
 }  // End namespace OMVIS
