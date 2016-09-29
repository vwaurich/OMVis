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

#ifndef INCLUDE_VIEW_DIALOGS_SIMSETTINGDIALOG_HPP_
#define INCLUDE_VIEW_DIALOGS_SIMSETTINGDIALOG_HPP_

#include "Model/VisualizationTypes.hpp"
#include "Model/SimSettings.hpp"

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>

#include <memory>

namespace OMVIS
{
    namespace View
    {

        /*! \brief This is a dialog to specify the simulation settings for FMU visualization.
         *
         * The user can specify the simulation step size, the render frequency and the solver method.
         */
        class SimSettingDialogFMU : public QDialog
        {
        Q_OBJECT

         public:
            /*-----------------------------------------
             * CONSTRUCTORS
             *---------------------------------------*/

            SimSettingDialogFMU(QWidget* parent = Q_NULLPTR);

            /*-----------------------------------------
             * GETTERS AND SETTERS
             *---------------------------------------*/

            Model::UserSimSettingsFMU getSimSettings() const;

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

            std::unique_ptr<QComboBox> _solverBox;
            std::unique_ptr<QLineEdit> _simStepSizeLineEdit;
            std::unique_ptr<QLineEdit> _visStepSizeLineEdit;
            Model::UserSimSettingsFMU _simSet;
        };


        /*! \brief This is a dialog to specify the simulation settings for MAT file visualization.
         *
         * At the moment, the user can set the speed of the visualization by choosing a speed up value. The
         * visualization of the simulation result is slowed down if speed up < 1 and accelerated if the speed up > 1,
         * respectively.
         */
        class SimSettingDialogMAT : public QDialog
        {
        Q_OBJECT

         public:
            /*-----------------------------------------
             * CONSTRUCTORS
             *---------------------------------------*/

            SimSettingDialogMAT(QWidget* parent = Q_NULLPTR);

            /*-----------------------------------------
             * GETTERS AND SETTERS
             *---------------------------------------*/

            Model::UserSimSettingsMAT getSimSettings() const;

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

            std::unique_ptr<QLineEdit> _speedupLineEdit;
            Model::UserSimSettingsMAT _simSet;
        };

    }  // End namespace View
}  // End namespace OMVIS

#endif /* INCLUDE_VIEW_DIALOGS_SIMSETTINGDIALOG_HPP_ */
