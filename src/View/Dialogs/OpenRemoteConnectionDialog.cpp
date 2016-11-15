/*
 * Copyright (C) 2016, Volker Waurich
 *
 * This file is part of OMVIS.
 *
 * OMVIS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OMVIS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OMVIS.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "View/Dialogs/OpenRemoteConnectionDialog.hpp"
#include "Util/Logger.hpp"
#include "Util/Util.hpp"

#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>
#include <QDialogButtonBox>
#include <QMessageBox>

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
                  _pathLineEdit(new QLineEdit()),
                  _workingDirectoryLineEdit(new QLineEdit()),
                  _ipAddress(),
                  _portNumber(-1),
                  _modelFile(),
                  _path(),
                  _workingDirectory()
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

            // Model file
            QLabel* modelFileLabel = new QLabel(tr("&Model file:"));
            _modelFileLineEdit->setPlaceholderText("cranes_crane_input5.fmu");
            _modelFileLineEdit->setMaximumWidth(400);
            _modelFileLineEdit->setFixedWidth(320);
            modelFileLabel->setBuddy(_modelFileLineEdit.get());

            // Path
            QLabel* pathLabel = new QLabel(tr("&Location of the model file on server:"));
            _pathLineEdit->setPlaceholderText("/scratch/p_hpcom/remoteVisualization/");  //("/PATH/TO/MODEL.fmu");
            _pathLineEdit->setMaximumWidth(400);
            _pathLineEdit->setFixedWidth(320);
            pathLabel->setBuddy(_pathLineEdit.get());

            // Local Working Directory
            QLabel* wDirLabel = new QLabel(tr("&Local working directory:"));
            _workingDirectoryLineEdit->setPlaceholderText("./rVis/");  //("./");
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
            perspectiveLayout->addWidget(pathLabel);
            perspectiveLayout->addWidget(_pathLineEdit.get());
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
            /// Todo: Get cP.path from modelFileLineEdit
            if (_ipAddressLineEdit->isModified())
            {
                if (Util::isValidIPv4(_ipAddressLineEdit->text().toStdString()) || Util::isValidIPv6(_ipAddressLineEdit->text().toStdString()))
                    _ipAddress = _ipAddressLineEdit->text().toStdString();
            }
            else
            {
                //X123 MF: Abkuerzung fuer den Entwickler: Nutze Defaultwerte.
                _ipAddress = "127.0.0.1";  // 141.30.73.103";
                // QMessageBox::warning(0, QString("Information"), QString("Invalid IP Address!"));
            }

            /// \todo Todo: Use default port number which is set in the constructor of this class.
            if (_portNumberLineEdit->isModified())
                _portNumber = _portNumberLineEdit->text().toInt();
            else
            {
                //X123 MF: Abkuerzung fuer den Entwickler: Nutze Defaultwerte.
                _portNumber = 4444;
                // QMessageBox::warning(0, QString("Information"), QString("Specify a port! (e.g., 4444)"));
            }

            if (_workingDirectoryLineEdit->isModified())
                _workingDirectory = _workingDirectoryLineEdit->text().toStdString();
            else
            {
                //X123 MF: Abkuerzung fuer den Entwickler: Nutze Defaultwerte.
                _workingDirectory = "./rVis/";
                // QMessageBox::warning(0, QString("Information"), QString("Specify a Working Directory!"));
            }

            if (_modelFileLineEdit->isModified())
                _modelFile = _modelFileLineEdit->text().toStdString();
            else
            {
                //X123 MF: Abkuerzung fuer den Entwickler: Nutze Defaultwerte.
                _modelFile = "BouncingBall.fmu";    //cranes_crane_input5.fmu";
                // QMessageBox::warning(0, QString("Information"), QString("Specify a model file!"));
            }

            if (_pathLineEdit->isModified())
                _path = _pathLineEdit->text().toStdString();
            else
            {
                //X123 MF: Abkuerzung fuer den Entwickler: Nutze Defaultwerte.
                _path = "/home/mf/opt/HPCOM/parallel_fmu_eclipse/test/data/";  //"/scratch/p_hpcom/remoteVisualization/";
                // QMessageBox::warning(0, QString("Information"), QString("Specify the path where to find the model file!"));
            }

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
