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

        /*-----------------------------------------
         * CONSTRUCTORS
         *---------------------------------------*/

        SimSettingDialogFMU::SimSettingDialogFMU(QWidget* parent,  const Model::UserSimSettingsFMU& simSetFMU)
                : OkCancelHelpButtonBox(),
                  _solverBox(new QComboBox()),
                  _simStepSizeLineEdit(new QLineEdit(QString::number(simSetFMU.simStepSize))),
                  _visStepSizeLineEdit(new QLineEdit(QString::number(simSetFMU.visStepSize))),
                  _simEndTimeLineEdit(new QLineEdit(QString::number(simSetFMU.simEndTime))),
                  _simSet()
        {
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
            solverLayout->addWidget(_solverBox.get());

            // Step size
            QHBoxLayout* simStepSizeLayout = new QHBoxLayout();
            QLabel* simStepSizeLabel = new QLabel(tr("Simulation Step Size: "));
            simStepSizeLayout->addWidget(simStepSizeLabel);
            simStepSizeLayout->addWidget(_simStepSizeLineEdit.get());

            // Visualization step size, aka render frequency
            QHBoxLayout* visStepSizeLayout = new QHBoxLayout();
            QLabel* visStepSizeLabel = new QLabel(tr("Visualization Step Size [ms]: "));
            visStepSizeLayout->addWidget(visStepSizeLabel);
            visStepSizeLayout->addWidget(_visStepSizeLineEdit.get());

            // Visualization step size, aka render frequency
            QHBoxLayout* endTimeLayout = new QHBoxLayout();
            QLabel* endTimeLabel = new QLabel(tr("Simulation End Time [s]: "));
            endTimeLayout->addWidget(endTimeLabel);
            endTimeLayout->addWidget(_simEndTimeLineEdit.get());

            mainLayout->addLayout(solverLayout);
            mainLayout->addLayout(simStepSizeLayout);
            mainLayout->addLayout(visStepSizeLayout);
            mainLayout->addLayout(endTimeLayout);
            mainLayout->addLayout(_okCancelHelpButtonLayout);
        }

        SimSettingDialogMAT::SimSettingDialogMAT(QWidget* parent)
                : OkCancelHelpButtonBox(),
                  _speedupLineEdit(new QLineEdit("1.0")),
                  _simSet()
        {
            // Main layout
            QVBoxLayout* mainLayout = new QVBoxLayout();
            mainLayout->addStretch(1);
            setLayout(mainLayout);
            setWindowTitle(tr("Simulation Settings"));

            // Speed up or slow down of visualization
//            _speedupLineEdit = new QLineEdit("1.0");

            // MF: Yeah, the validator stuff just do no work for me or I do not understand it right.
            //     From my point of view, I want to set a restriction to only enter values for which
            //     1.0 <= val <= 10.0 holds.
//            QDoubleValidator* valid = new QDoubleValidator(1.0, 10.0, 2, this);
//            valid->setLocale(QLocale::English);
//            valid->setBottom(1.0);
            //valid.setLocale(QLocale::English);
//            _speedupLineEdit->setValidator(valid);
//            new QRegExpValidator(QRegExp("[1-9]"));
            //_speedupLineEdit->setValidator(new QDoubleValidator(1.0, 10.0, 2, this));

            QHBoxLayout* speedUpLayout = new QHBoxLayout();
            QLabel* speedUpLabel = new QLabel(tr("Visualization Speedup: "));
//            QLabel* explanationLabel = new QLabel(tr("A speedup greater than 1.0 means that<br>"
//                                                     "the simulation runs faster and gives a <br>"
//                                                     "rough overview on the model behavior."));

            speedUpLayout->addWidget(speedUpLabel);
            speedUpLayout->addWidget(_speedupLineEdit.get());

            mainLayout->addLayout(speedUpLayout);
//            mainLayout->addWidget(explanationLabel);
            mainLayout->addLayout(_okCancelHelpButtonLayout);
        }

        /*-----------------------------------------
         * SLOTS
         *---------------------------------------*/

        void SimSettingDialogFMU::accept()
        {
            // Shift by one, because 0 equals to Solver::NONE which is not available from the dialog.
            _simSet.solver = static_cast<Model::Solver>(_solverBox->currentIndex() + 1);
            _simSet.simStepSize = _simStepSizeLineEdit->text().toDouble();
            _simSet.visStepSize = _visStepSizeLineEdit->text().toDouble();
            _simSet.simEndTime = _simEndTimeLineEdit->text().toDouble();
            QDialog::accept();
        }

        void SimSettingDialogFMU::help() const
        {
          QString information("For a FMU visualization, the user can select several settings:"
                              "<ul>"
                              "<li><b>Solver:</b> The integration algorithm (a.k.a. solver) can be chosen. </li>"
                              "<li><b>Simulation Step Size:</b> </li>"
                              "<li><b>Visualization Step Size (aka render frequency):</b> </li>"
                              "<li><b>Simulation End Time:</b> Set the simulation end time.</li>"
                              "</ul>"
          );
          QMessageBox msgBox(QMessageBox::Information, tr("Help"), information);
          msgBox.setStandardButtons(QMessageBox::Close);
          msgBox.exec();
        }

        void SimSettingDialogMAT::accept()
        {
            if (_speedupLineEdit->isModified())
            {
                _simSet.speedup = _speedupLineEdit->text().toDouble();
                if (1.0 > _simSet.speedup)
                {
                    QMessageBox::warning(0, QString("Information"), QString("A speedup less than 1.0 is not valid."));
                    _simSet.speedup = 1.0;
                }
            }
            QDialog::accept();
        }

        void SimSettingDialogMAT::help() const
        {
          QString information("A speedup greater than 1.0 means that<br>"
                               "the simulation runs faster and gives a <br>"
                               "rough overview on the model behavior. <br><br>"
                               "[A speedup less than 1.0 is not possible, <br>"
                               "since the result file does not provide <br>"
                               "enough (intermediate) data.]");
          QMessageBox msgBox(QMessageBox::Information, tr("Help"), information);
          msgBox.setStandardButtons(QMessageBox::Close);
          msgBox.exec();
        }

        /*-----------------------------------------
         * GETTERS AND SETTERS
         *---------------------------------------*/

        Model::UserSimSettingsFMU SimSettingDialogFMU::getSimSettings() const
        {
            return _simSet;
        }

        Model::UserSimSettingsMAT SimSettingDialogMAT::getSimSettings() const
        {
            return _simSet;
        }

    }  // End namespace View
}  // End namespace OMVIS
