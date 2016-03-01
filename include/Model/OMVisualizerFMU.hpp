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
 * OMVisualizerFMU.hpp
 *
 *  Created on: 17.02.2016
 *      Author: mf
 */

#ifndef INCLUDE_OMVISUALIZERFMU_HPP_
#define INCLUDE_OMVISUALIZERFMU_HPP_

#include <string>
#include <memory>
#include "Model/FMUSimulate.hpp"
#include "Controller/JoystickDevice.hpp"
#include "Controller/KeyboardEventHandler.hpp"
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
    class OMVisualizerFMU : public OMVisualizerAbstract
    {
     public:
        OMVisualizerFMU() = delete;

        OMVisualizerFMU(const std::string modelName, const std::string modelPath);

        virtual ~OMVisualizerFMU() = default;
        OMVisualizerFMU(const OMVisualizerFMU& omvf) = delete;
        OMVisualizerFMU& operator=(const OMVisualizerFMU& omvf) = delete;

        /// Loads and initializes FMU file.
        void loadFMU(const std::string model, const std::string dir);

        virtual void simulate(View::OMVManager& omvm);

        double simulateStep(const double time);

        /// \todo Quick and dirty hack, move initialization of _simSettings to a more appropriate place!
        void initData();

        /*! \brief This methods resets the input values of a FMU to default ("zero" )values.
         *
         */
        void resetInputs();

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

        /*! \brief The input values and the controller device are connected so that the user can control the simulation.
         *
         * \todo What about joystick support? I think we need here connection to a more general controller object/class.
         * \todo Find a more suitable name, maybe linkInputsToController()?
         */
        void linkInputsToEventHandler();

		/*! \brief Returns a 0 if we use mat-failes, 1 if we use fmus.
		*/
		int getDataTypeID();

     private:
        fmul_t _fmul;
        fmuData _fmuData;
        SimSettings* _simSettings;

     public:
        /// \ todo Make it private again.
        InputData _inputData;

        /// \todo Remove, we do not need it because we have inputData.
        Controller::JoystickDevice _joystick;
    };

}  // End namespace Model

#endif /* INCLUDE_OMVISUALIZERFMU_HPP_ */
