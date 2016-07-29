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

#include "View/Dialogs/OpenRemoteConnectionDialog.hpp"
#include "Util/Logger.hpp"
#include "Util/Util.hpp"

#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>
#include <QDialogButtonBox>
#include <QMessageBox>

#include <string>

#ifndef _WIN32
#include <arpa/inet.h>
#endif

namespace OMVIS
{
    namespace View
    {

        /*-----------------------------------------
         * CONSTRUCTORS
         *---------------------------------------*/

        OpenRemoteConnectionDialog::OpenRemoteConnectionDialog(QWidget* parent)
                : QDialog(parent),
                  _ipAddressLineEdit(new QLineEdit()),
                  _portNumberLineEdit(new QLineEdit()),
                  _modelFileLineEdit(new QLineEdit()),
                  _workingDirectoryLineEdit(new QLineEdit()),
                  _ipAddress(),
                  _portNumber(-1),
                  _workingDirectory(),
                  _modelFile(),
                  _path()
        {
            setWindowTitle(tr("Open Remote Connection"));

            // Server
            QLabel* serverLabel = new QLabel(tr("&IP address:"));
            _ipAddressLineEdit->setPlaceholderText("141.30.73.103 or 2001:0db8:85a3:08d3:1319:8a2e:0370:7344");
            _ipAddressLineEdit->setMaximumWidth(400);
            _ipAddressLineEdit->setFixedWidth(320);
            serverLabel->setBuddy(_ipAddressLineEdit.get());

            // Port
            QLabel* portLabel = new QLabel(tr("&Port:"));
            _portNumberLineEdit->setPlaceholderText("4444");
            _portNumberLineEdit->setMaximumWidth(400);
            _portNumberLineEdit->setFixedWidth(320);
            portLabel->setBuddy(_portNumberLineEdit.get());

            // Local Working Directory
            QLabel* modelFileLabel = new QLabel(tr("&Location of the model file on server:"));
            _modelFileLineEdit->setPlaceholderText("/scratch/p_hpcom/remoteVisualization/cranes_crane_input5.fmu"); //("/PATH/TO/MODEL.fmu");
            _modelFileLineEdit->setMaximumWidth(400);
            _modelFileLineEdit->setFixedWidth(320);
            modelFileLabel->setBuddy(_modelFileLineEdit.get());

            // Local Working Directory
            QLabel* wDirLabel = new QLabel(tr("&Local working directory:"));
            _workingDirectoryLineEdit->setPlaceholderText("./rVis/"); //("./");
            _workingDirectoryLineEdit->setMaximumWidth(400);
            _workingDirectoryLineEdit->setFixedWidth(320);
            wDirLabel->setBuddy(_workingDirectoryLineEdit.get());

            QVBoxLayout* perspectiveLayout = new QVBoxLayout();
            perspectiveLayout->addWidget(serverLabel);
            perspectiveLayout->addWidget(_ipAddressLineEdit.get());
            perspectiveLayout->addWidget(portLabel);
            perspectiveLayout->addWidget(_portNumberLineEdit.get());
            perspectiveLayout->addWidget(modelFileLabel);
            perspectiveLayout->addWidget(_modelFileLineEdit.get());
            perspectiveLayout->addWidget(wDirLabel);
            perspectiveLayout->addWidget(_workingDirectoryLineEdit.get());

            QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
            QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
            QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
            QVBoxLayout* buttonsLayout = new QVBoxLayout();
            buttonsLayout->addStretch(1);
            buttonsLayout->addWidget(buttonBox);
            QVBoxLayout* mainLayout = new QVBoxLayout();
            mainLayout->addLayout(perspectiveLayout);
            mainLayout->addLayout(buttonsLayout);
            mainLayout->addStretch(1);
            setLayout(mainLayout);
        }

        /*-----------------------------------------
         * SLOTS
         *---------------------------------------*/

        void OpenRemoteConnectionDialog::accept()
        {
//X123 MF: Abkuerzung fuer den Entwickler: Nutze Defaultwerte.
            /// Todo: Get cP.path from modelFileLineEdit
//X123            if (_ipAddressLineEdit->isModified())
//X123            {
//X123                if (Util::isValidIPv4(_ipAddressLineEdit->text().toStdString()) || Util::isValidIPv6(_ipAddressLineEdit->text().toStdString()))
//                    _cP.ipAddress = _ipAddressLineEdit->text().toStdString();
//                else
//                    QMessageBox::warning(0, QString("Information"), QString("Invalid IP Address!"));
//            }
//            if (_portNumberLineEdit->isModified())
//                _cP.portNumber = _portNumberLineEdit->text().toInt();
//            else
//                QMessageBox::warning(0, QString("Information"), QString("Specify a port! (e.g., 4444)"));
//            /// \todo Todo: Use default port number which is set in the constructor of this class.
//
//            if (_workingDirectoryLineEdit->isModified())
//                _cP.workingDirectory = _workingDirectoryLineEdit->text().toStdString();
//            else
//                QMessageBox::warning(0, QString("Information"), QString("Specify a Working Directory!"));
//
//            if (_modelFileLineEdit->isModified())
//            {
//              _cP.modelFile = _modelFileLineEdit->text().toStdString();
//              _cP.isFMU = Util::isFMU(_cP.modelFile());
//            }
//            else
//X123                QMessageBox::warning(0, QString("Information"), QString("Specify a model file!"));

            //X123 MF: Abkuerzung fuer den Entwickler: Nutze Defaultwerte.
            _ipAddress = "127.0.0.1"; // 141.30.73.103";
            _portNumber = 4444;
            _workingDirectory = "./rVis/";
            _modelFile = "BouncingBall.fmu";    //cranes_crane_input5.fmu";
            _path = "/home/mf/opt/HPCOM/parallel_fmu_eclipse/test/data/"; //"/scratch/p_hpcom/remoteVisualization/";

            QDialog::accept();
        }

        /*-----------------------------------------
         * GETTERS AND SETTERS
         *---------------------------------------*/

        Initialization::RemoteVisualizationConstructionPlan OpenRemoteConnectionDialog::getConstructionPlan() const
        {
            return Initialization::RemoteVisualizationConstructionPlan(_modelFile, _path, _ipAddress, _portNumber, _workingDirectory);
        }

    }  // End namespace View
}  // End namespace OMVIS
