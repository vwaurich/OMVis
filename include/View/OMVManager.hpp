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
 * OMVManager.hpp
 *
 *  Created on: 03.02.2016
 *      Author: mf
 */

#ifndef INCLUDE_OMVMANAGER_HPP_
#define INCLUDE_OMVMANAGER_HPP_

#include <osg/Timer>
#include <iostream>

namespace View
{

    class OMVManager
    {
     public:
        OMVManager() = delete;

        OMVManager(double simTime, double realTime, double visTime, double hVisual, double startTime, double endTime);

        ~OMVManager() = default;
        OMVManager(const OMVManager& omvm) = delete;
        OMVManager& operator=(const OMVManager& omvm) = delete;

        void updateTick();
        bool simTimeReady();
        bool visTimeReady();
		/*! \brief Get a value between 0 and 99 that indicates how far we are between start and end of a simulation
		*/
		int getTimeProgress();

     public:
        /// \todo Can this attr. be private?
        double _simTime;//!< Brief Time of current simulation step.
        /// \todo Can this attr. be private?
        double _realTime;//!< Brief Current Real Time.
        /// \todo Can this attr. be private?
        double _visTime;//!< Brief Time of current scene update
        /// \todo Can this attr. be private?
        double _hVisual;//!< Brief Step size for the scene updates in milliseconds
        /// \todo Can this attr. be private?
        double _endTime;
		double _startTime;
		bool pause;

	public:
		osg::Timer* _visualTimer;
    };

}  // End namespace View

#endif /* INCLUDE_OMVMANAGER_HPP_ */
