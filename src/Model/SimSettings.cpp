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


/*
 * SimSettings.cpp
 *
 *  Created on: 03.02.2016
 *      Author: mf
 */

#include "Model/SimSettings.hpp"
#include "Model/FMUSimulate.hpp"

namespace Model
{

    SimSettings::SimSettings()
            : _callEventUpdate(fmi1_false),
              _toleranceControlled(fmi1_false),
              _intermediateResults(fmi1_false),
              _tstart(0.0),
              _hdef(0.1),
              _tend(0.1),
              _relativeTolerance(0.001)
    {
    }

    void SimSettings::set_tend(const fmi1_real_t t)
    {
        _tend = t;
    }

    void SimSettings::set_tstart(const fmi1_real_t t)
    {
        _tstart = t;
    }

    void SimSettings::set_hdef(const fmi1_real_t h)
    {
        _hdef = h;
    }

    void SimSettings::set_relativeTolerance(const fmi1_real_t t)
    {
        _relativeTolerance = t;
    }

    fmi1_real_t SimSettings::get_tend()
    {
        return _tend;
    }

    fmi1_real_t SimSettings::get_tstart()
    {
        return _tstart;
    }

    fmi1_real_t SimSettings::get_hdef()
    {
        return _hdef;
    }

    fmi1_real_t SimSettings::get_relativeTolerance()
    {
        return _relativeTolerance;
    }

    fmi1_boolean_t SimSettings::get_toleranceControlled()
    {
        return _toleranceControlled;
    }

    fmi1_boolean_t SimSettings::get_callEventUpdate()
    {
        return _callEventUpdate;
    }

    fmi1_boolean_t SimSettings::get_intermediateResults()
    {
        return _intermediateResults;
    }

}  // End namespace Model
