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

//#include <QComboBox>
//#include <QGroupBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <string>

#ifndef _WIN32
#include <arpa/inet.h>
#endif;

namespace OMVIS
{
    namespace View
    {
        /*! \brief Functions that tests the given string to be a valid IPv4 address.
         *
         * @param ipAddress The address to be tested.
         * @return Boolean.
         */
        bool isValidIpAddressV4(const std::string& ipAddress)
        {
#ifndef _WIN32
            struct sockaddr_in sa;
            int result = inet_pton(AF_INET, ipAddress.c_str(), &(sa.sin_addr));

            if (result != 0)
            {
                LOGGER_WRITE(ipAddress + std::string(" is a valid IPv4 address."), Util::LC_GUI, Util::LL_WARNING);
                return true;
            }
            else
            {
                LOGGER_WRITE(ipAddress + std::string(" is not a valid IPv4 address."), Util::LC_GUI, Util::LL_WARNING);
                return false;
            }
#elif _WIN32
            LOGGER_WRITE("isValidIpAddressV4 has not been implemented for Windows", Util::LC_GUI, Util::LL_WARNING);
            return false;
#endif;

            // Short way without logger usage.
            //return result != 0;
        }

        /*! \brief Functions that tests the given string to be a valid IPv6 address.
         *
         * @param ipAddress The address to be tested.
         * @return Boolean.
         */
        bool isValidIpAddressV6(const std::string& ipAddress)
        {
#ifndef _WIN32

            struct sockaddr_in6 sa;
            int result = inet_pton(AF_INET6, ipAddress.c_str(), &(sa.sin6_addr));

            if (result != 0)
            {
                LOGGER_WRITE(ipAddress + std::string(" is a valid IPv6 address."), Util::LC_GUI, Util::LL_WARNING);
                return true;
            }
            else
            {
                LOGGER_WRITE(ipAddress + std::string(" is not a valid IPv6 address."), Util::LC_GUI, Util::LL_WARNING);
                return false;
            }
            LOGGER_WRITE(ipAddress + std::string(" is not a valid IPv6 address."), Util::LC_GUI, Util::LL_WARNING);
            // Short way without logger usage.
            //return result != 0;
#elif _WIN32
            LOGGER_WRITE("isValidIpAddressV6 has not been implemented for Windows", Util::LC_GUI, Util::LL_WARNING);
            return false;
#endif;
        }

        /// \todo Implement me!
        bool isValidServerName(const std::string& serverName)
        {
            LOGGER_WRITE(serverName + std::string(" is assumed to be a valid Server Name. TODO: Implement a proper check!"), Util::LC_GUI, Util::LL_WARNING);
            return true;
        }

        bool OpenRemoteConnectionDialog::checkServerName(const QString& serverName) const
        {
            std::string tmp(serverName.toStdString());
            return (isValidIpAddressV4(tmp) || isValidIpAddressV6(tmp) || isValidServerName(tmp));
        }

        OpenRemoteConnectionDialog::OpenRemoteConnectionDialog(QWidget* parent)
                : QDialog(parent),
                  _serverName(""),
                  _portNumber(4444)
        {
            setWindowTitle(tr("Open Remote Connection"));

            QLabel* serverLabel = new QLabel(tr("&Server Name or IP:"));
            _ServerNameLineEdit = new QLineEdit();
            _ServerNameLineEdit->setPlaceholderText("taurus.hrsk.tu-dresden.de or 203.0.113.195 or 2001:0db8:85a3:08d3:1319:8a2e:0370:7344");
            _ServerNameLineEdit->setMaximumWidth(400);
            _ServerNameLineEdit->setFixedWidth(320);
            serverLabel->setBuddy(_ServerNameLineEdit);

            QLabel* portLabel = new QLabel(tr("&Port:"));
            _PortNumberLineEdit = new QLineEdit();
            _PortNumberLineEdit->setPlaceholderText("4444");
            _PortNumberLineEdit->setMaximumWidth(400);
            _PortNumberLineEdit->setFixedWidth(320);
            portLabel->setBuddy(_PortNumberLineEdit);

            QVBoxLayout* perspectiveLayout = new QVBoxLayout();
            perspectiveLayout->addWidget(serverLabel);
            perspectiveLayout->addWidget(_ServerNameLineEdit);
            perspectiveLayout->addWidget(portLabel);
            perspectiveLayout->addWidget(_PortNumberLineEdit);

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

        void OpenRemoteConnectionDialog::accept()
        {
            if (_ServerNameLineEdit->isModified())
            {
                if (checkServerName(_ServerNameLineEdit->text()))
                    _serverName = _ServerNameLineEdit->text();
                else
                    QMessageBox::warning(0, QString("Information"), QString("Invalid Server Name or IP "));
            }
            if (_PortNumberLineEdit->isModified())
            {
                if (checkServerName(_PortNumberLineEdit->text()))
                    _portNumber = _PortNumberLineEdit->text();
                // else: Use default port number which is set in the constructor of this class.
            }
            QDialog::accept();
        }

        QString OpenRemoteConnectionDialog::getResult() const
        {
            return _serverName;
        }

    }  // End namespace View
}  // End namespace OMVIS
