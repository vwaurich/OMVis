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
                  _cP()
//                  _ipAddress(""),
//                  _portNumber(4444),
//                  _modelFile(""),
//                  _workingDirectory("")
        {
            setWindowTitle(tr("Open Remote Connection"));

            // Server
            QLabel* serverLabel = new QLabel(tr("&IP Address:"));
            _ipAddressLineEdit = new QLineEdit();
            _ipAddressLineEdit->setPlaceholderText("taurus.hrsk.tu-dresden.de or 203.0.113.195 or 2001:0db8:85a3:08d3:1319:8a2e:0370:7344");
            _ipAddressLineEdit->setMaximumWidth(400);
            _ipAddressLineEdit->setFixedWidth(320);
            serverLabel->setBuddy(_ipAddressLineEdit);

            // Port
            QLabel* portLabel = new QLabel(tr("&Port:"));
            _portNumberLineEdit = new QLineEdit();
            _portNumberLineEdit->setPlaceholderText("4444");
            _portNumberLineEdit->setMaximumWidth(400);
            _portNumberLineEdit->setFixedWidth(320);
            portLabel->setBuddy(_portNumberLineEdit);

            // Local Working Directory
            QLabel* modelFileLabel = new QLabel(tr("&Where to find the model file on server:"));
            _modelFileLineEdit = new QLineEdit();
            _modelFileLineEdit->setPlaceholderText("/PATH/TO/MODEL.fmu");
            _modelFileLineEdit->setMaximumWidth(400);
            _modelFileLineEdit->setFixedWidth(320);
            modelFileLabel->setBuddy(_modelFileLineEdit);

            // Local Working Directory
            QLabel* wDirLabel = new QLabel(tr("&Local Working Directory:"));
            _workingDirectoryLineEdit = new QLineEdit();
            _workingDirectoryLineEdit->setPlaceholderText("./");
            _workingDirectoryLineEdit->setMaximumWidth(400);
            _workingDirectoryLineEdit->setFixedWidth(320);
            wDirLabel->setBuddy(_workingDirectoryLineEdit);

            QVBoxLayout* perspectiveLayout = new QVBoxLayout();
            perspectiveLayout->addWidget(serverLabel);
            perspectiveLayout->addWidget(_ipAddressLineEdit);
            perspectiveLayout->addWidget(portLabel);
            perspectiveLayout->addWidget(_portNumberLineEdit);
            perspectiveLayout->addWidget(modelFileLabel);
            perspectiveLayout->addWidget(_modelFileLineEdit);
            perspectiveLayout->addWidget(wDirLabel);
            perspectiveLayout->addWidget(_workingDirectoryLineEdit);

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
            if (_ipAddressLineEdit->isModified())
            {
                if (Util::isValidIpAddressV6(_ipAddressLineEdit->text().toStdString()) || Util::isValidIpAddressV6(_ipAddressLineEdit->text().toStdString()))
                    _cP.ipAddress = _ipAddressLineEdit->text().toStdString();
//                    _ipAddress = _ipAddressLineEdit->text();
                else
                    QMessageBox::warning(0, QString("Information"), QString("Invalid IP Address!"));
            }
            if (_portNumberLineEdit->isModified())
                _cP.portNumber = _portNumberLineEdit->text().toInt();
//                _portNumber = _portNumberLineEdit->text().toInt();
            else
                QMessageBox::warning(0, QString("Information"), QString("Specify a port! (e.g., 4444)"));
            /// \todo Todo: Use default port number which is set in the constructor of this class.

            if (_workingDirectoryLineEdit->isModified())
                _cP.workingDirectory = _workingDirectoryLineEdit->text().toStdString();
//                _workingDirectory = _workingDirectoryLineEdit->text();
            else
                QMessageBox::warning(0, QString("Information"), QString("Specify a Working Directory!"));

            if (_modelFileLineEdit->isModified())
                _cP.modelFile = _modelFileLineEdit->text().toStdString();
//                _modelFile = _modelFileLineEdit->text();
            else
                QMessageBox::warning(0, QString("Information"), QString("Specify a model file!"));
            QDialog::accept();
        }

        /*-----------------------------------------
         * GETTERS AND SETTERS
         *---------------------------------------*/

//        /*! \brief Returns the IP address of the computing server. */
//        std::string OpenRemoteConnectionDialog::getIPAddress() const
//        {
//            return _ipAddress.toStdString();
//        }
//
//        /*! \brief Returns the port number to use for the connection. */
//        int OpenRemoteConnectionDialog::getPortNumer() const
//        {
//            return _portNumber;
//        }
//
//        /*! \brief Returns the path to the (local) working directory. */
//        std::string OpenRemoteConnectionDialog::getWorkingDirectory() const
//        {
//            return _workingDirectory.toStdString();
//        }
//
//        /*! \brief Returns the file name (incl. path) of the selected model. */
//        std::string OpenRemoteConnectionDialog::getModelName() const
//        {
//            return _modelFile.toStdString();
//        }

        Initialization::RemoteVisualizationConstructionPlan OpenRemoteConnectionDialog::getConstructionPlan() const
        {
            return _cP;
                    //Initialization::RemoteVisualizationConstructionPlan constructionPlan(_ipAddress, _portNumber, _modelFile, _workingDirectory);
        }


    }  // End namespace View
}  // End namespace OMVIS
