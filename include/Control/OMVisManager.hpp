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

#include <osg/Timer>

namespace OMVIS
{
    namespace Control
    {

        /*! Class that manages timing for OMVis.
         *
         * Since we have at least three different time levels, namely simulation time, visualization time and real time,
         * we need a manager to properly handle them.
         *
         * \todo More documentation.
         */
        class OMVisManager
        {
         public:
            OMVisManager() = delete;

            /*! \brief Constructs a OMVisManager from the given arguments.
             *
             * @param simTime Current simulation time. In general, this is equal to the simulation start time.
             * @param realTime
             * @param visTime Time of current scene update.
             * @param hVisual Step size for the scene updates.
             * @param startTime Simulation start time.
             * @param endTime Simulation end time.
             *
             * \todo Can we remove some arguments? E.g., visTime might be set to a default value all time.
             */
            OMVisManager(double simTime, double realTime, double realTimeFactor, double visTime, double hVisual, double startTime, double endTime);

            ~OMVisManager() = default;
            OMVisManager(const OMVisManager& omvm) = delete;
            OMVisManager& operator=(const OMVisManager& omvm) = delete;

            // 2016-04-19, MF: Currently unused method
            void updateTick();
            // 2016-04-19, MF: Currently unused method
            bool simTimeReady();
            // 2016-04-19, MF: Currently unused method
            bool visTimeReady();

            /*! \brief Calculates from the current visualization time (_visTime) the slider position. */
            int getTimeProgress();

            /*! Set the slider range. */
            void setSliderRange(const int min, const int max);

            double getEndTime() const;
            void setEndTime(const double endTime);

            double getStartTime() const;
            void setStartTime(const double startTime);

            double getSimTime() const;
            void setSimTime(const double simTime);

            double getVisTime() const;
            void setVisTime(const double visTime);

            double getHVisual() const;

            double getRealTime() const;

			double getRealTimeFactor() const;

			void setRealTimeFactor(double rtf);

            /*! Returns true, if the Visualization is currently paused and false otherwise. */
            bool isPaused() const;
            /*! Set pause status to new value. */
            void setPause(const bool status);

         private:
            //! Time of the current simulation step.
            double _simTime;
            //! Current real time.
            double _realTime;
			//! realtime factor
			double _realTimeFactor;
            //! Time of current scene update.
            double _visTime;
            //! Step size for the scene updates in milliseconds.
            double _hVisual;
            double _endTime;
            double _startTime;
            bool _pause;

            osg::Timer _visualTimer;

            //! Range of the slider widget.
            int _sliderRange;
        };

    }  // End namespace Control
}  // End namespace OMVIS

#endif /* INCLUDE_OMVISMANAGER_HPP_ */
/**
 * @}
 */
