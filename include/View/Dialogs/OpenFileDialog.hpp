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

#ifndef INCLUDE_VIEW_DIALOGS_OPENFILEDIALOG_HPP_
#define INCLUDE_VIEW_DIALOGS_OPENFILEDIALOG_HPP_

#include "Initialization/VisualizationConstructionPlans.hpp"

#include <QString>
#include <QFileDialog>

#include <string>

namespace OMVIS
{
    namespace View
    {

        /*! \brief This class provides a dialog to select a simulation model file from the local file system.
         *
         */
        class OpenFileDialog : public QFileDialog
        {
        Q_OBJECT

         public:
            /*-----------------------------------------
             * CONSTRUCTORS
             *---------------------------------------*/

            /*! \brief Constructor which creates the Qt dialog window to select the simulation file.
             *
             * The user selects the simulation file from the local file system.
             */
            OpenFileDialog(QWidget* parent = Q_NULLPTR);

            /*-----------------------------------------
             * GETTERS AND SETTERS
             *---------------------------------------*/

            /*! \brief Returns the construction plan for the visualization of the chosen model. */
            Initialization::VisualizationConstructionPlan getConstructionPlan() const;

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

            std::string _modelFile;
            std::string _path;
        };

    }  // End namespace View
}  // End namespace OMVIS

#endif /* INCLUDE_VIEW_DIALOGS_OPENFILEDIALOG_HPP_ */
