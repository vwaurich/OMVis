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

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>

#include <memory>
#include <string>

namespace OMVIS
{
    namespace View
    {

        // \todo Move to appropriate place
        enum Solver
        {
            forwardEuler = 0
        };


        struct SimSettingsFMU
        {
            Solver solver;
            float simStepSize;
            float visStepSize;
        };

        struct SimSettingsMAT
        {
            float speedup;
        };


        /*! \brief This is a dialog to specify the simulation settings for FMU visualization.
         *
         * The user can specify the simulation step size, the render frequency and the solver method.
         */
        class SimSettingDialogFMU : public QDialog
        {
        Q_OBJECT
         public:
            SimSettingDialogFMU(QWidget* parent = Q_NULLPTR);
            SimSettingsFMU getSimSettings() const;

         private slots:
            void accept() Q_DECL_OVERRIDE;

         private:
            QComboBox* _solverBox;
            QLineEdit* _simStepSizeLineEdit;
            QLineEdit* _visStepSizeLineEdit;
            SimSettingsFMU _simSet;
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
            SimSettingDialogMAT(QWidget* parent = Q_NULLPTR);
            SimSettingsMAT getSimSettings() const;

         private slots:
            void accept() Q_DECL_OVERRIDE;

         private:
            QLineEdit* _speedupLineEdit;
            SimSettingsMAT _simSet;
        };

    }  // End namespace View
}  // End namespace OMVIS

#endif /* INCLUDE_VIEW_DIALOGS_SIMSETTINGDIALOG_HPP_ */
