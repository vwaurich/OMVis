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

/** @addtogroup Model
 *  @{
 *  \copyright TU Dresden. All rights reserved.
 *  \authors Volker Waurich, Martin Flehmig
 *  \date Feb 2016
 */

#ifndef INCLUDE_SIMSETTINGS_HPP_
#define INCLUDE_SIMSETTINGS_HPP_

#include "WrapperFMILib.hpp"

namespace OMVIS
{
    namespace Model
    {
        /*! \brief Class that holds the simulation settings.
         *
         *
         */
        class SimSettings
        {
         public:
            /*! \brief Constructs SimSettings object.
             *
             * The attributes are initialized with default values.
             */
            SimSettings();

            /// Destructs SimSettings object.
            ~SimSettings() = default;

            SimSettings(const SimSettings& ss) = delete;
            SimSettings& operator=(const SimSettings& ss) = delete;

            /// \todo Refactor method to setTend
            void setTend(const fmi1_real_t t);
            void setTstart(const fmi1_real_t t);
            void setHdef(const fmi1_real_t h);
            void setRelativeTolerance(const fmi1_real_t t);
            fmi1_real_t getTend();
            fmi1_real_t getTstart();
            fmi1_real_t getHdef();
            fmi1_real_t getRelativeTolerance();
            fmi1_boolean_t getToleranceControlled();
            fmi1_boolean_t getCallEventUpdate();
            fmi1_boolean_t getIntermediateResults();

            static int iunused;

         public:
            fmi1_boolean_t _callEventUpdate;
            fmi1_boolean_t _toleranceControlled;
            fmi1_boolean_t _intermediateResults;

         private:
            fmi1_real_t _tstart;
            fmi1_real_t _hdef;
            fmi1_real_t _tend;
            fmi1_real_t _relativeTolerance;
        };

    }  // End namespace Model
}  // End namespace OMVIS

#endif /* INCLUDE_SIMSETTINGS_HPP_ */
/**
 * @}
 */
