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
                  _serverName("")
        {
            setWindowTitle(tr("Open Remote Connection"));

            QLabel* label = new QLabel(tr("&Server Name or IP:"));
            _lineEdit = new QLineEdit();
            _lineEdit->setPlaceholderText("taurus.hrsk.tu-dresden.de or 203.0.113.195 or 2001:0db8:85a3:08d3:1319:8a2e:0370:7344");
            _lineEdit->setMaximumWidth(400);
            _lineEdit->setFixedWidth(320);
            label->setBuddy(_lineEdit);

            QHBoxLayout* perspectiveLayout = new QHBoxLayout();
            perspectiveLayout->addWidget(label);
            perspectiveLayout->addWidget(_lineEdit);

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
            if (_lineEdit->isModified())
            {
                if (checkServerName(_lineEdit->text()))
                    _serverName = _lineEdit->text();
                else
                    QMessageBox::warning(0, QString("Information"), QString("Invalid Server Name or IP "));
            }
            QDialog::accept();
        }

        QString OpenRemoteConnectionDialog::getResult() const
        {
            return _serverName;
        }

    }  // End namespace View
}  // End namespace OMVIS
