/*
 * SimSettings.hpp
 *
 *  Created on: 11.08.2016
 *      Author: mf
 */

#ifndef INCLUDE_MODEL_SIMSETTINGS_HPP_
#define INCLUDE_MODEL_SIMSETTINGS_HPP_

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
         * The user can specify the settings of a FMU based simulation via the Simulation Settings dialog.
         */
        struct UserSimSettingsFMU
        {
            Solver solver;
            double simStepSize;
            double visStepSize;
        };

        /*! \brief This struct holds the simulation settings the user can chose via the GUI for a MAT file.
         *
         * The user can specify the settings of a MAT result file based simulation via the Simulation Settings dialog.
         */
        struct UserSimSettingsMAT
        {
            double speedup;
        };

    }  // End namespace Model
}  // End namespace OMVIS

#endif /* INCLUDE_MODEL_SIMSETTINGS_HPP_ */
