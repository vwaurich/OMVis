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

#include "Initialization/VisualizationConstructionPlan.hpp"

#include <QDialog>
#include <QLineEdit>

QT_FORWARD_DECLARE_CLASS(QComboBox)

namespace OMVIS
{
    namespace View
    {
        /*! \brief This class provides a dialog to specify the settings for a remote visualization.
         *
         * The user has to specify the following settings:
         *  - IP address or server name
         *  - Port number to use
         *  - Local working directory: The visualization might create some temporary files in the
         *                             working directory.
         */
        class OpenRemoteConnectionDialog : public QDialog
        {
        Q_OBJECT

         public:
            /*-----------------------------------------
             * CONSTRUCTORS
             *---------------------------------------*/

            /*! \brief Constructor which creates the Qt dialog window to specify connection parameters.
             *
             * A connection is fully specified by the server name (or IP address) and the port.
             * On localhost side, the user has to specify a working directory.
             */
            OpenRemoteConnectionDialog(QWidget* parent = Q_NULLPTR);

            /*-----------------------------------------
             * SLOTS
             *---------------------------------------*/

         private slots:
            /*! \brief Accepts user input and stores it into member variables. */
            void accept() Q_DECL_OVERRIDE;

            /*-----------------------------------------
             * GETTERS AND SETTERS
             *---------------------------------------*/

         public:
            Initialization::RemoteVisualizationConstructionPlan getConstructionPlan() const;

         private:
            /*-----------------------------------------
             * MEMBERS
             *---------------------------------------*/

            QLineEdit* _ipAddressLineEdit;
            QLineEdit* _portNumberLineEdit;
            QLineEdit* _modelFileLineEdit;
            QLineEdit* _workingDirectoryLineEdit;

            Initialization::RemoteVisualizationConstructionPlan _cP;
        };

    }  // End namespace View
}  // End namespace OMVIS

#endif /* INCLUDE_VIEW_DIALOGS_OPENREMOTECONNECTIONDIALOG_HPP_ */
