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

#ifndef INCLUDE_VIEW_DIALOGS_OPENREMOTECONNECTIONDIALOG_HPP_
#define INCLUDE_VIEW_DIALOGS_OPENREMOTECONNECTIONDIALOG_HPP_

#include <QDialog>
#include <QLineEdit>

QT_FORWARD_DECLARE_CLASS(QComboBox)

namespace OMVIS
{
    namespace View
    {
        /*! This class provides a dialog to specify the IP address or name of the server for remote connection.
         *
         */
        class OpenRemoteConnectionDialog : public QDialog
        {
        Q_OBJECT

         public:
            /*! \brief Constructor which creates the Qt dialog window to chose the perspective.
             *
             */
            OpenRemoteConnectionDialog(QWidget* parent = Q_NULLPTR);

            /*! \brief Return result from this dialog.
             *
             * \remark If the user input was not a valid IP or server name, the result will be an empty QString.
             */
            QString getResult() const;

            /*! \brief Checks if the specified serverName is a valid IPV4 or IPv6 address or server name.
             *
             */
            bool checkServerName(const QString& serverName) const;

         private slots:
            /*! \brief Accepts user input.
             *
             * If the user made a selection and clicked the apply button, this method reads and checks the
             * user specification. If the value is a valid IPv4, IPV6 or server name, it is stored into
             * the member \ref _serverName.
             */
            void accept() Q_DECL_OVERRIDE;

         private:
            QLineEdit* _lineEdit;
            QString _serverName;
        };

    }  // End namespace View
}  // End namespace OMVIS

#endif /* INCLUDE_VIEW_DIALOGS_OPENREMOTECONNECTIONDIALOG_HPP_ */
