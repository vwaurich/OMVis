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

#ifndef INCLUDE_SIMSETTINGS_HPP_
#define INCLUDE_SIMSETTINGS_HPP_

namespace OMVIS
{
    namespace Model
    {

        /*! \brief All available numerical integration algorithms (aka solvers).
         *
         */
        enum class Solver
        {
            NONE = 0,
            EULER_FORWARD = 1
        };

        /*! \brief This struct holds the simulation settings the user can chose via the GUI for a FMU.
         *
         * The user can specify the settings of a FMU based simulation via the \ref View::SimSettingDialog. The user
         * can specify the solver that should be used for integration, the simulation step size and the visualization
         * step size. The later determines the interval to call the sceneUpdate() method.
         */
        struct UserSimSettingsFMU
        {
            Solver solver;
            double simStepSize;
            double visStepSize;
        };

        /*! \brief This struct holds the simulation settings the user can chose via the GUI for a MAT file.
         *
         * The user can specify the settings of a MAT result file based simulation via the \ref View::SimSettingDialog.
         * The user can specifically specify the speedup of the simulation, i.e., a speedup less than one will slow
         * down the simulation, a speed up greater than one, will speed it up.
         */
        struct UserSimSettingsMAT
        {
            double speedup;
        };

    }  // End namespace Model
}  // End namespace OMVIS

#endif /* INCLUDE_SIMSETTINGS_HPP_ */
