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
 * OMVManager.cpp
 *
 *  Created on: 03.02.2016
 *      Author: mf
 */

#include "View/OMVManager.hpp"

namespace View
{
	OMVManager::OMVManager(double simTime, double realTime, double visTime, double hVisual, double startTime, double endTime)
			: _simTime(simTime),
              _realTime(realTime),
              _visTime(visTime),
              _hVisual(hVisual),
			  _startTime(startTime),
			  _endTime(endTime),
			  _visualTimer(new osg::Timer),
			  pause(true)
    {
    }

    void OMVManager::updateTick()
    {
		_visualTimer->tick();
        _realTime = _visualTimer->time_m() / 1000.0;
    }

    bool OMVManager::simTimeReady()
    {
        return _realTime > _simTime && _simTime <= _endTime;
    }

    bool OMVManager::visTimeReady()
    {
        return _simTime >= _visTime;
    }

	int OMVManager::getTimeProgress()
	{
		return (int)_visTime * 99.0 / (_endTime - _startTime);
	}

}  // End namespace View
