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

#include "Model/SimSettingsFMU.hpp"
#include "Model/FMUWrapper.hpp"

namespace OMVIS
{
    namespace Model
    {

        SimSettingsFMU::SimSettingsFMU()
                : _callEventUpdate(fmi1_false),
                  _toleranceControlled(fmi1_true),
                  _intermediateResults(fmi1_false),
                  _tstart(0.0),
                  _hdef(0.1),
                  _tend(0.1),
                  _relativeTolerance(0.001),
                  _solver(Solver::EULER_FORWARD)
        {
        }

        void SimSettingsFMU::setTend(const fmi1_real_t t)
        {
            _tend = t;
        }

        void SimSettingsFMU::setTstart(const fmi1_real_t t)
        {
            _tstart = t;
        }

        void SimSettingsFMU::setHdef(const fmi1_real_t h)
        {
            _hdef = h;
        }

        void SimSettingsFMU::setRelativeTolerance(const fmi1_real_t t)
        {
            _relativeTolerance = t;
        }

        fmi1_real_t SimSettingsFMU::getTend() const
        {
            return _tend;
        }

        fmi1_real_t SimSettingsFMU::getTstart() const
        {
            return _tstart;
        }

        fmi1_real_t SimSettingsFMU::getHdef()
        {
            return _hdef;
        }

        fmi1_real_t SimSettingsFMU::getRelativeTolerance()
        {
            return _relativeTolerance;
        }

        fmi1_boolean_t SimSettingsFMU::getToleranceControlled() const
        {
            return _toleranceControlled;
        }

        void SimSettingsFMU::setSolver(const Solver& solver)
        {
            _solver = solver;
        }

        fmi1_boolean_t* SimSettingsFMU::getCallEventUpdate()
        {
            return &_callEventUpdate;
        }

        fmi1_boolean_t SimSettingsFMU::getIntermediateResults()
        {
            return _intermediateResults;
        }

    }  // End namespace Model
}  // End namespace OMVIS
