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

#include <Control/TimeManager.hpp>

#include <cmath>

namespace OMVIS
{
    namespace Control
    {

        /*-----------------------------------------
         * CONSTRUCTORS
         *---------------------------------------*/

        TimeManager::TimeManager(const double simTime, const double realTime, const double realTimeFactor, const double visTime,
                                 const double hVisual, const double startTime, const double endTime)
                : _simTime(simTime),
                  _realTime(realTime),
                  _realTimeFactor(realTimeFactor),
                  _visTime(visTime),
                  _hVisual(hVisual),
                  _startTime(startTime),
                  _endTime(endTime),
                  _pause(true),
                  _visualTimer(),
                  _sliderRange(0)
        {
        }

        /*-----------------------------------------
         * SIMULATION METHODS
         *---------------------------------------*/

        void TimeManager::updateTick()
        {
            _visualTimer.tick();
            _realTime = _visualTimer.time_m() / 1000.0;
        }

        bool TimeManager::simTimeReady()
        {
            return _realTime > _simTime && _simTime <= _endTime;
        }

        bool TimeManager::visTimeReady()
        {
            return _simTime >= _visTime;
        }

        /*-----------------------------------------
         * GETTERS and SETTERS
         *---------------------------------------*/

        int TimeManager::getSliderPosition() const
        {
            return std::round(_visTime * _sliderRange / (_endTime - _startTime));
        }

        void TimeManager::setSliderRange(const int min, const int max)
        {
            _sliderRange = max - min;
        }

        double TimeManager::getEndTime() const
        {
            return _endTime;
        }

        void TimeManager::setEndTime(const double endTime)
        {
            _endTime = endTime;
        }

        double TimeManager::getStartTime() const
        {
            return _startTime;
        }

        void TimeManager::setStartTime(const double startTime)
        {
            _startTime = startTime;
        }

        double TimeManager::getSimTime() const
        {
            return _simTime;
        }

        void TimeManager::setSimTime(const double simTime)
        {
            _simTime = simTime;
        }

        double TimeManager::getVisTime() const
        {
            return _visTime;
        }

        void TimeManager::setVisTime(const double visTime)
        {
            _visTime = visTime;
        }

        double TimeManager::getHVisual() const
        {
            return _hVisual;
        }

        void TimeManager::setHVisual(const double hVis)
        {
            _hVisual = hVis;
        }


        double TimeManager::getRealTime() const
        {
            return _realTime;
        }

        double TimeManager::getRealTimeFactor() const
        {
            return _realTimeFactor;
        }

        void TimeManager::setRealTimeFactor(const double rtf)
        {
            _realTimeFactor = rtf;
        }

        bool TimeManager::isPaused() const
        {
            return _pause;
        }

        void TimeManager::setPause(const bool status)
        {
            _pause = status;
        }

    }  // End namespace Control
}  // End namespace OMVIS
