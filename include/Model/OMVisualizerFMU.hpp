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

#ifndef INCLUDE_OMVISUALIZERFMU_HPP_
#define INCLUDE_OMVISUALIZERFMU_HPP_

#include "FMU.hpp"
#include "Model/SimSettings.hpp"
#include "Model/InputData.hpp"
#include "Model/OMVisualizerAbstract.hpp"
#include "Visualize.hpp"
#include "Control/JoystickDevice.hpp"
#include "Control/KeyboardEventHandler.hpp"

#include <string>

// Forward declaration
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
    class OMVisualizerFMU : public OMVisualizerAbstract
    {
     public:
        OMVisualizerFMU() = delete;

        OMVisualizerFMU(const std::string modelName, const std::string modelPath);

        virtual ~OMVisualizerFMU() = default;
        OMVisualizerFMU(const OMVisualizerFMU& omvf) = delete;
        OMVisualizerFMU& operator=(const OMVisualizerFMU& omvf) = delete;

        /*! \brief Loads and initializes FMU file.
         *
         * @param model Name of the FMU.
         * @param dir Path to the FMU file.
         * @return Error value.
         */
        int loadFMU(const std::string model, const std::string dir);

        /*! \brief Unloads the FMU and frees allocated memory. */
        virtual void unload();

        virtual void simulate(Control::OMVisManager& omvm);

        double simulateStep(const double time);

        /*! \todo Quick and dirty hack, move initialization of _simSettings to a more appropriate place!
         * @return Error value.
         */
        int initData();

        /*! \brief This methods resets the input values of a FMU to default ("zero") values.
         *
         */
        void resetInputs();

		/*! \brief This method updates the actual data for the visualization bodies by using variables from the FMU.
		 *
		 * \return Error value.
		 */
		int updateVisAttributes(const double time);

		/*! \brief Implementation for OMVisualizerAbstract::initializeVisAttributes to set the scene to initial position.
		 *
		 * \Remark: Parameter time is not used, just inherited from \ref OMVisualizerAbstract::initializeVisAttributes(const double).
		 */
		void initializeVisAttributes(const double time = 0.0);

		/*! \brief For FMU-based visualization, we have to simulate until the next visualization time step.
		 *
		 *  \Remark: Parameter time is not used, just inherited from \ref OMVisualizerAbstract::updateScene(const double).
		 */
		void updateScene(const double time = 0.0);

		/*! \brief Returns "fmu".
		*/
		std::string getType() const;

		/*! \brief initializes the attached joysticks
		 */
		void initJoySticks();


     private:
		FMU _fmu;
        SimSettings* _simSettings;
		int _numJoysticks;

     public:
        /// \ todo Make it private again.
        InputData _inputData;

        /// \todo Remove, we do not need it because we have inputData.
        std::vector<Control::JoystickDevice*> _joysticks;
    };

}  // End namespace Model

#endif /* INCLUDE_OMVISUALIZERFMU_HPP_ */
/**
 * @}
 */
