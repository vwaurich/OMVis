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

/** \addtogroup Model
 *  \{
 *  \copyright TU Dresden. All rights reserved.
 *  \authors Volker Waurich, Martin Flehmig
 *  \date Feb 2016
 */

#ifndef INCLUDE_SIMSETTINGSFMU_HPP_
#define INCLUDE_SIMSETTINGSFMU_HPP_

#include "Model/SimSettings.hpp"
#include "WrapperFMILib.hpp"

namespace OMVIS
{
    namespace Model
    {

        /*! \brief This class holds the simulation settings for a FMU.
         *
         * For example, a simulation is defined by:
         *   - Numerical integration algorithm (, e.g. Euler Forward)
         *   - Start and end time
         *   - Step size
         *   - Relative tolerance
         *
         */
        class SimSettingsFMU
        {
         public:
            /*-----------------------------------------
             * CONSTRUCTORS
             *---------------------------------------*/

            /*! \brief Constructs SimSettings object.
             *
             * The attributes are initialized with default values.
             */
            SimSettingsFMU();

            ~SimSettingsFMU() = default;

            SimSettingsFMU(const SimSettingsFMU& rhs) = delete;

            SimSettingsFMU& operator=(const SimSettingsFMU& rhs) = delete;

            /*-----------------------------------------
             * GETTERS AND SETTERS
             *---------------------------------------*/

            /*!
             * Set end time for the simulation
             * \param t
             */
            void setTend(const double t);
            double getTend() const;

            void setTstart(const double t);
            double getTstart() const;

            void setHdef(const double h);
            double getHdef() const;

            void setRelativeTolerance(const double t);
            double getRelativeTolerance();

            int getToleranceControlled() const;

            void setSolver(const Solver& solver);
            Solver getSolver() const;

            /*!
             * Returns a pointer to the member _callEventUpdate. It is perfectly fine to return just a pointer and not
             * a copy of the member, because the FMI Library takes pointers.
             *
             * \return Pointer to member _callEventUpdate.
             */
            int* getCallEventUpdate();

            /*!
             * \return Member _intermediateResults.
             */
            int getIntermediateResults();

         private:
            /*-----------------------------------------
             * MEMBERS
             *---------------------------------------*/

//X11            fmi1_boolean_t _callEventUpdate;
//X11            fmi1_boolean_t _toleranceControlled;
//X11            fmi1_boolean_t _intermediateResults;
//
//            fmi1_real_t _tstart;
//            fmi1_real_t _hdef;
//            fmi1_real_t _tend;
//X11            fmi1_real_t _relativeTolerance;

            int _callEventUpdate;
            int _toleranceControlled;
            int _intermediateResults;

            double _tstart;
            double _hdef;
            double _tend;
            double _relativeTolerance;

            /// \todo Implement me. Currently, only the Euler Forward algorithm is implemented.
            Solver _solver;
        };

    }  // namespace Model
}  // namespace OMVIS

#endif /* INCLUDE_SIMSETTINGSFMU_HPP_ */
/**
 * \}
 */
