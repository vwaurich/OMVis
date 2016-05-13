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

#include "Control/OMVisManager.hpp"

#include <cmath>
#include <iostream>

namespace OMVIS
{
    namespace Control
    {

        /*-----------------------------------------
         * CONSTRUCTORS
         *---------------------------------------*/
        OMVisManager::OMVisManager(const double simTime, const double realTime, const double realTimeFactor, const double visTime, const double hVisual,
                                   const double startTime, const double endTime)
                : _simTime(simTime),
                  _realTime(realTime),
                  _realTimeFactor(realTimeFactor),
                  _visTime(visTime),
                  _hVisual(hVisual),
                  _startTime(startTime),
                  _endTime(endTime),
                  _visualTimer(),
                  _pause(true),
                  _sliderRange(0)
        {
        }

        /*-----------------------------------------
         * SIMULATION METHODS
         *---------------------------------------*/

        void OMVisManager::updateTick()
        {
            _visualTimer.tick();
            _realTime = _visualTimer.time_m() / 1000.0;
        }

        bool OMVisManager::simTimeReady()
        {
            return _realTime > _simTime && _simTime <= _endTime;
        }

        bool OMVisManager::visTimeReady()
        {
            return _simTime >= _visTime;
        }

        /*-----------------------------------------
         * GETTERS and SETTERS
         *---------------------------------------*/

        void OMVisManager::setSliderRange(const int min, const int max)
        {
            _sliderRange = max - min;
        }

        int OMVisManager::getTimeProgress() const
        {
            return std::round(_visTime * _sliderRange / (_endTime - _startTime));
        }

        double OMVisManager::getEndTime() const
        {
            return _endTime;
        }

        void OMVisManager::setEndTime(const double endTime)
        {
            _endTime = endTime;
        }

        double OMVisManager::getStartTime() const
        {
            return _startTime;
        }

        void OMVisManager::setStartTime(const double startTime)
        {
            _startTime = startTime;
        }

        double OMVisManager::getSimTime() const
        {
            return _simTime;
        }

        void OMVisManager::setSimTime(const double simTime)
        {
            _simTime = simTime;
        }

        double OMVisManager::getVisTime() const
        {
            return _visTime;
        }

        void OMVisManager::setVisTime(const double visTime)
        {
            _visTime = visTime;
        }

        double OMVisManager::getRealTime() const
        {
            return _realTime;
        }

        double OMVisManager::getRealTimeFactor() const
        {
            return _realTimeFactor;
        }

        void OMVisManager::setRealTimeFactor(double rtf)
        {
            _realTimeFactor = rtf;
        }

        double OMVisManager::getHVisual() const
        {
            return _hVisual;
        }

        bool OMVisManager::isPaused() const
        {
            return _pause;
        }

        void OMVisManager::setPause(const bool status)
        {
            _pause = status;
        }

    }  // End namespace Control
}  // End namespace OMVIS
