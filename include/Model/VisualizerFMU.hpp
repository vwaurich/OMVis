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

#ifndef INCLUDE_VISUALIZERFMU_HPP_
#define INCLUDE_VISUALIZERFMU_HPP_

#include "Model/SimSettings.hpp"
#include "Model/SimSettingsFMU.hpp"
#include "Model/FMUWrapper.hpp"
#include "Model/VisualizerAbstract.hpp"
#include "Model/InputData.hpp"
#include "Control/JoystickDevice.hpp"
#include "Control/KeyboardEventHandler.hpp"

#include <string>

namespace OMVIS
{
    namespace Model
    {

        /*! \brief This class handles the visualization of FMUs.
         *
         * In contrast to \ref OMVisualizerMAT, this class provides user interaction via joystick devices to enable
         * steering the model.
         *
         * The end time for FMU visualization is 100. This is set while allocation of the TimeManager object.
         */
        class VisualizerFMU : public VisualizerAbstract
        {
         public:
            /*-----------------------------------------
             * CONSTRUCTORS
             *---------------------------------------*/

            VisualizerFMU() = delete;

            /*! \brief Constructs a OMVisualizerFMU object from the given arguments.
             *
             * Essentially, a FMU file and its path need to be specified.
             *
             * \param fileName  Model file name without path.
             * \param path      Path to the model file.
             */
            VisualizerFMU(const std::string& modelFile, const std::string& path);

            virtual ~VisualizerFMU() = default;

            VisualizerFMU(const VisualizerFMU& rhs) = delete;

            VisualizerFMU& operator=(const VisualizerFMU& rhs) = delete;

            /*-----------------------------------------
             * INITIALIZATION METHODS
             *---------------------------------------*/

            /*! \brief Loads and initializes the FMU file.
             *
             * \param modelFile     Name of the FMU file.
             * \param path          Path to the model file.
             */
            void loadFMU(const std::string& modelFile, const std::string& path);

            void setSimulationSettings(const UserSimSettingsFMU& simSetFMU);

            /*-----------------------------------------
             * GETTERS and SETTERS
             *---------------------------------------*/

            /*! Returns const. pointer to \ref FMU member. */
            const FMUWrapper* getFMU() const;

            std::shared_ptr<InputData> getInputData() const;

            UserSimSettingsFMU getCurrentSimSettings() const;

         private:
            /*-----------------------------------------
             * MEMBERS
             *---------------------------------------*/

            /*! The encapsulated FMU data. */
            std::shared_ptr<FMUWrapper> _fmu;
            /*! Simulation settings, e.g., start and end time. */
            std::shared_ptr<SimSettingsFMU> _simSettings;

            std::shared_ptr<InputData> _inputData;

         public:
            /// \todo Remove, we do not need it because we have inputData.
            std::vector<Control::JoystickDevice*> _joysticks;

         private:
            /*-----------------------------------------
             * PRIVATE METHODS
             *---------------------------------------*/

            /*-----------------------------------------
             * SIMULATION METHODS
             *---------------------------------------*/

            void simulate(Control::TimeManager& omvm) override;

            double simulateStep(const double time);

            /*! \brief This method updates the visualization attributes after a time step has been performed.
             *
             * The method updates the actual data for the visualization bodies by using variables from the FMU.
             *
             * \param time  The visualization time.
             */
            void updateVisAttributes(const double time) override;

            /*! \brief This method does a scene update, i.e., the FMU is integrated/simulated up to the given time.
             *
             * For FMU-based visualization, we have to simulate until the next visualization time step. This method is
             * called by the method \ref VisualizerAbstract::sceneUpdate, which does the time handling (visTime,
             * simTime) around.
             *
             * \remark Parameter time is not used, just inherited from \ref VisualizerAbstract::updateScene(const double).
             */
            void updateScene(const double time = 0.0) override;

            /*! \todo Quick and dirty hack, move initialization of _simSettings to a more appropriate place! */
            void initData() override;

            /*! \brief This methods resets the input values of a FMU to default ("zero") values. */
            void resetInputs();

            /*! \brief Initializes the attached joysticks. */
            void initJoySticks();

            /*! \brief Implementation for VisualizerAbstract::initializeVisAttributes to set the scene to initial position.
             *
             * \remark The parameter \a time is not used, just inherited from
             *         \ref VisualizerAbstract::initializeVisAttributes(const double).
             */
            void initializeVisAttributes(const double time = 0.0) override;

            /*! \brief Helper function for OMVisualizerAbstract::setVarReferencesInVisAttributes
             */
            fmi1_value_reference_t getVarReferencesForObjectAttribute(ShapeObjectAttribute* attr);

            /*! \brief Sets the variable references in the visualization attributes.
             *
             * \remark The vis. attributes are encapsulated in the inherited member _baseData of class type OMVisualBase.
             */
            int setVarReferencesInVisAttributes();

            /*! \brief Update the attributes of a shape. */
            void updateObjectAttributeFMU(ShapeObjectAttribute* attr, fmi1_import_t* fmu);
        };

    }  // namespace Model
}  // namespace OMVIS

#endif /* INCLUDE_VISUALIZERFMU_HPP_ */
/**
 * @}
 */
