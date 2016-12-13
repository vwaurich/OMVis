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

#include "Model/SimSettingsFMU.hpp"
//#include "Model/FMUWrapper.hpp"

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

        void SimSettingsFMU::setTend(const double t)
        {
            _tend = t;
        }

        void SimSettingsFMU::setTstart(const double t)
        {
            _tstart = t;
        }

        void SimSettingsFMU::setHdef(const double h)
        {
            _hdef = h;
        }

        void SimSettingsFMU::setRelativeTolerance(const double t)
        {
            _relativeTolerance = t;
        }

        double SimSettingsFMU::getTend() const
        {
            return _tend;
        }

        double SimSettingsFMU::getTstart() const
        {
            return _tstart;
        }

        double SimSettingsFMU::getHdef() const
        {
            return _hdef;
        }

        double SimSettingsFMU::getRelativeTolerance()
        {
            return _relativeTolerance;
        }

        int SimSettingsFMU::getToleranceControlled() const
        {
            return _toleranceControlled;
        }

        void SimSettingsFMU::setSolver(const Solver& solver)
        {
            _solver = solver;
        }

        Solver SimSettingsFMU::getSolver() const
        {
            return _solver;
        }

        int* SimSettingsFMU::getCallEventUpdate()
        {
            return &_callEventUpdate;
        }

        int SimSettingsFMU::getIntermediateResults()
        {
            return _intermediateResults;
        }

    }  // namespace Model
}  // namespace OMVIS
