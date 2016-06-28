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

        /*! The class manages the timing for OMVis.
         *
         * Since we have at least three different time levels, namely simulation time, visualization time and real time,
         * we need a manager to properly handle them.
         *
         * The OMVisManager object also stores the time slider range of the GUI and computes its position during
         * the simulation run. On the other hand, if the user moves the time slider, the OMVisManager computes the
         * simulation time from the new position.
         */
        class OMVisManager
        {
         public:
            /*-----------------------------------------
             * CONSTRUCTORS
             *---------------------------------------*/

            OMVisManager() = delete;

            /*! \brief Constructs a OMVisManager from the given arguments.
             *
             * \param simTime       Current simulation time. In general, this is equal to the simulation start time.
             * \param realTime      Current real time.
             * \param visTime       Time of current scene update.
             * \param hVisual       Step size for the scene updates.
             * \param startTime     Simulation start time.
             * \param endTime       Simulation end time.
             *
             * \todo Can we remove some arguments? E.g., visTime might be set to a default value all time.
             */
            OMVisManager(const double simTime, const double realTime, const double realTimeFactor, const double visTime,
                         const double hVisual, const double startTime, const double endTime);

            ~OMVisManager() = default;
            OMVisManager(const OMVisManager& omvm) = delete;
            OMVisManager& operator=(const OMVisManager& omvm) = delete;

            /*-----------------------------------------
             * SIMULATION METHODS
             *---------------------------------------*/

            void updateTick();
            // 2016-04-19, MF: Currently unused method
            bool simTimeReady();
            // 2016-04-19, MF: Currently unused method
            bool visTimeReady();

            /*-----------------------------------------
             * GETTERS and SETTERS
             *---------------------------------------*/

            /*! \brief Calculates from the current visualization time (_visTime) the position of the time slider. */
            int getSliderPosition() const;

            /*! \brief Sets the time slider range.
             *
             * \param min   The minimum value of the time slider. In most cases this is 0.
             * \param max   The maximum value of the time slider.
             * */
            void setSliderRange(const int min, const int max);

            /*! \brief Returns the end time of the simulation. */
            double getEndTime() const;
            /*! \brief Sets the end time of the simulation to the given value. */
            void setEndTime(const double endTime);

            /*! \brief Returns the start time of the simulation. */
            double getStartTime() const;
            /*! \brief Sets the start time of the simulation to the given value. */
            void setStartTime(const double startTime);

            /*! \brief Returns the current simulation time. */
            double getSimTime() const;
            /*! Stes the simulation time to the given value. */
            void setSimTime(const double simTime);

            /*! \brief Returns the current visualization time. */
            double getVisTime() const;
            /*! \brief Sets the visualization time to the given value. */
            void setVisTime(const double visTime);

            /*! \brief Returns the current step size of the simulation. */
            double getHVisual() const;

            /*! \brief Returns real time. */
            double getRealTime() const;

            /*! \brief Returns the real time factor. */
            double getRealTimeFactor() const;
            /*! \brief Sets the real time factor to the given value. */
            void setRealTimeFactor(const double rtf);

            /*! \brief Returns true, if the visualization is currently paused and false otherwise. */
            bool isPaused() const;
            /*! \brief Sets pause status to new value. */
            void setPause(const bool status);

         private:
            /*-----------------------------------------
             * MEMBERS
             *---------------------------------------*/

            //! Time of the current simulation step.
            double _simTime;
            //! Current real time.
            double _realTime;
            //! Real time factor.
            double _realTimeFactor;
            //! Time of current scene update.
            double _visTime;
            //! Step size for the scene updates in milliseconds.
            double _hVisual;
            double _startTime;
            double _endTime;
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
