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

/** @addtogroup Model
 *  @{
 *  \copyright TU Dresden. All rights reserved.
 *  \authors Volker Waurich, Martin Flehmig
 *  \date Feb 2016
 */

#ifndef INCLUDE_OMVISUALIZEREMPTY_HPP_
#define INCLUDE_OMVISUALIZEREMPTY_HPP_

#include <string>
#include <memory>
#include "Model/OMVisualizerFMU.hpp"
#include "Control/JoystickDevice.hpp"
#include "Control/KeyboardEventHandler.hpp"
#include "Model/SimSettings.hpp"
#include "Model/InputData.hpp"
#include "Model/OMVisualizerAbstract.hpp"
#include "Visualize.hpp"


namespace View
{
    class OMVManager;
}

namespace Model
{

    /*! \brief Class that handles visualization of FMUs.
     *
     *
     * In contrast to \ref OMVisualizationMAT, this class provides user interaction via joystick device.
     */
    class OMVisualizerEmpty : public OMVisualizerAbstract
    {
     public:
		 OMVisualizerEmpty() = delete;

		 OMVisualizerEmpty(const std::string modelName, const std::string modelPath);

        virtual ~OMVisualizerEmpty() = default;
		OMVisualizerEmpty(const OMVisualizerEmpty& omvf) = delete;
		OMVisualizerEmpty& operator=(const OMVisualizerEmpty& omvf) = delete;

        /// \todo Quick and dirty hack, move initialization of _simSettings to a more appropriate place!
        void initData();

		/*! \brief This method updates the actual data for the visuaslization bodies by using variables from the fmu.
		*
		*/
		void updateVisAttributes(const double time);

		/*! \brief Implementation for OMVisualizerAbstract::initializeVisAttributes to set the scene to initial position.
		*/
		void initializeVisAttributes(const double time);

		/*! \brief For FMU-based visualization, we have to simulate until the next visualization time step.
		*/
		void updateScene(const double time);

		/*! \brief Returns a 0 if we use mat-failes, 1 if we use fmus.
		*/
		int getDataTypeID();

    };

}  // End namespace Model

#endif /* INCLUDE_OMVISUALIZEREMPTY_HPP_ */
