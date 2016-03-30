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

/** @addtogroup Control
 *  @{
 *  \copyright TU Dresden. All rights reserved.
 *  \authors Volker Waurich, Martin Flehmig
 *  \date Feb 2016
 */

#ifndef INCLUDE_OMVISMANAGER_HPP_
#define INCLUDE_OMVISMANAGER_HPP_

// Forward declaration
namespace osg {
   class Timer;
}

namespace Control
{

    class OMVisManager
    {
     public:
        OMVisManager() = delete;

        OMVisManager(double simTime, double realTime, double visTime, double hVisual, double startTime, double endTime);

        ~OMVisManager() = default;
        OMVisManager(const OMVisManager& omvm) = delete;
        OMVisManager& operator=(const OMVisManager& omvm) = delete;

        void updateTick();
        bool simTimeReady();
        bool visTimeReady();

        /*! \brief Calculates from the current visualization time (_visTime) the slider position. */
		int getTimeProgress();

		/*! Set the slider range. */
		void setSliderRange(const int min, const int max);

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
		bool _pause;

	public:
		osg::Timer* _visualTimer;

		/*! Range of the slider widget. */
		int _sliderRange;
    };

}  // End namespace Control

#endif /* INCLUDE_OMVISMANAGER_HPP_ */
/**
 * @}
 */
