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
#include "Util/Util.hpp"

#include <QGroupBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QString>

#include <iostream>

namespace OMVIS
{
    namespace View
    {
        SimSettingDialogFMU::SimSettingDialogFMU(QWidget* parent)
                : QDialog(parent),
                  _solverBox(new QComboBox()),
                  _simStepSizeLineEdit(new QLineEdit("0.0001")),
                  _visStepSizeLineEdit(new QLineEdit("100"))
        {
            QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
            QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
            QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
            QVBoxLayout* buttonsLayout = new QVBoxLayout();
            buttonsLayout->addStretch(1);
            buttonsLayout->addWidget(buttonBox);

            // Main layout
            QVBoxLayout* mainLayout = new QVBoxLayout();
            mainLayout->addStretch(1);
            setLayout(mainLayout);
            setWindowTitle(tr("Simulation Settings"));

            // Solver method
            _solverBox->addItem(QString("Forward Euler"));
            QLabel* solverLabel = new QLabel(tr("Solver Method: "));
            QHBoxLayout* solverLayout = new QHBoxLayout();
            solverLayout->addWidget(solverLabel);
            solverLayout->addWidget(_solverBox);

            // Step size
            QHBoxLayout* simStepSizeLayout = new QHBoxLayout();
            QLabel* simStepSizeLabel = new QLabel(tr("Simulation Step Size: "));
            simStepSizeLayout->addWidget(simStepSizeLabel);
            simStepSizeLayout->addWidget(_simStepSizeLineEdit);

            // Visualization step size, aka render frequency
            QHBoxLayout* visStepSizeLayout = new QHBoxLayout();
            QLabel* visStepSizeLabel = new QLabel(tr("Visualization Step Size [ms]: "));
            visStepSizeLayout->addWidget(visStepSizeLabel);
            visStepSizeLayout->addWidget(_visStepSizeLineEdit);

            mainLayout->addLayout(solverLayout);
            mainLayout->addLayout(simStepSizeLayout);
            mainLayout->addLayout(visStepSizeLayout);
            mainLayout->addLayout(buttonsLayout);
        }

        void SimSettingDialogFMU::accept()
        {
            // Shift by one, because 0 equals to Solver::NONE which is not available from the dialog.
            _simSet.solver = static_cast<Model::Solver>(_solverBox->currentIndex() + 1);
            _simSet.simStepSize = _simStepSizeLineEdit->text().toDouble();
            _simSet.visStepSize = _visStepSizeLineEdit->text().toDouble();
            QDialog::accept();
        }

        Model::UserSimSettingsFMU SimSettingDialogFMU::getSimSettings() const
        {
            return _simSet;
        }

        SimSettingDialogMAT::SimSettingDialogMAT(QWidget* parent)
                : QDialog(parent)
        {
            QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
            QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
            QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
            QVBoxLayout* buttonsLayout = new QVBoxLayout();
            buttonsLayout->addStretch(1);
            buttonsLayout->addWidget(buttonBox);

            // Main layout
            QVBoxLayout* mainLayout = new QVBoxLayout();
            mainLayout->addStretch(1);
            setLayout(mainLayout);
            setWindowTitle(tr("Simulation Settings"));

            // Speed up or slow down of visualization
            QLineEdit* speedUp = new QLineEdit("1");
            speedUp->setValidator(new QDoubleValidator(0, 10, 2, this));

            //                int* l = new int();
            //                int* t = new int();
            //                int* r = new int();
            //                int* b = new int();
            //                //speedUp->getTextMargins(int &l, int &t, int &r, int &b);
            //                speedUp->getTextMargins(l, t, r, b);
            //                std::cout << "l= " << *l << std::endl;
            //                std::cout << "r= " << *l << std::endl;
            //                std::cout << "t= " << *l << std::endl;
            //                std::cout << "b= " << *l << std::endl;

            QHBoxLayout* speedUpLayout = new QHBoxLayout();
            QLabel* speedUpLabel = new QLabel(tr("Speed Up: "));
            speedUpLayout->addWidget(speedUpLabel);
            speedUpLayout->addWidget(speedUp);

            mainLayout->addLayout(speedUpLayout);

            mainLayout->addLayout(buttonsLayout);
        }

        void SimSettingDialogMAT::accept()
        {
            _simSet.speedup = _speedupLineEdit->text().toFloat();
            QDialog::accept();
        }

        Model::UserSimSettingsMAT SimSettingDialogMAT::getSimSettings() const
        {
            return _simSet;
        }

    }  // End namespace View
}  // End namespace OMVIS
