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

#ifndef INCLUDE_VIEW_DIALOGS_OPENREMOTECONNECTIONDIALOG_HPP_
#define INCLUDE_VIEW_DIALOGS_OPENREMOTECONNECTIONDIALOG_HPP_

#include "Initialization/VisualizationConstructionPlans.hpp"

#include <QDialog>
#include <QLineEdit>

#include <memory>
#include <string>

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

            ~OpenRemoteConnectionDialog() = default;

            OpenRemoteConnectionDialog(const OpenRemoteConnectionDialog& rhs) = delete;

            OpenRemoteConnectionDialog& operator=(const OpenRemoteConnectionDialog& rhs) = delete;

            /*-----------------------------------------
             * GETTERS AND SETTERS
             *---------------------------------------*/

            Initialization::RemoteVisualizationConstructionPlan getConstructionPlan() const;

            /*-----------------------------------------
             * SLOTS
             *---------------------------------------*/

         private slots:
            /*! \brief Accepts user input and stores it into member variables. */
            void accept() Q_DECL_OVERRIDE;

         private:
            /*-----------------------------------------
             * MEMBERS
             *---------------------------------------*/

            std::unique_ptr<QLineEdit> _ipAddressLineEdit;
            std::unique_ptr<QLineEdit> _portNumberLineEdit;
            std::unique_ptr<QLineEdit> _modelFileLineEdit;
            std::unique_ptr<QLineEdit> _pathLineEdit;
            std::unique_ptr<QLineEdit> _workingDirectoryLineEdit;

            std::string _ipAddress;
            int _portNumber;
            std::string _modelFile;
            std::string _path;
            std::string _workingDirectory;
        };

    }  // namespace View
}  // namespace OMVIS

#endif /* INCLUDE_VIEW_DIALOGS_OPENREMOTECONNECTIONDIALOG_HPP_ */
