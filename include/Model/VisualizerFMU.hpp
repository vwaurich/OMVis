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

#include "Model/VisualizerAbstract.hpp"
#include "FMU.hpp"
#include "Model/SimSettings.hpp"
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
            VisualizerFMU(const VisualizerFMU& omvf) = delete;
            VisualizerFMU& operator=(const VisualizerFMU& omvf) = delete;

            /*-----------------------------------------
             * INITIALIZATION METHODS
             *---------------------------------------*/

            /*! \brief Loads and initializes the FMU file.
             *
             * \param modelFile     Name of the FMU file.
             * \param path          Path to the model file.
             * \return Error value.
             */
            int loadFMU(const std::string& modelFile, const std::string& path);

            /*! \todo Quick and dirty hack, move initialization of _simSettings to a more appropriate place!
             *
             * \return Error value.
             */
            int initData() override;

            /*! \brief This methods resets the input values of a FMU to default ("zero") values.
             *
             */
            void resetInputs();

            /*! \brief Initializes the attached joysticks.
             */
            void initJoySticks();

            /*! \brief Implementation for OMVisualizerAbstract::initializeVisAttributes to set the scene to initial
             *         position.
             *
             * \remark The parameter \a time is not used, just inherited from
             *         \ref OMVisualizerAbstract::initializeVisAttributes(const double).
             */
            void initializeVisAttributes(const double time = 0.0) override;

            /*-----------------------------------------
             * GETTERS and SETTERS
             *---------------------------------------*/

            /*! \brief Returns "fmu".
             */
            std::string getType() const override;

            /*! Returns const pointer to \ref FMU member. */
            const FMU* getFMU() const;

            // const InputData* getInputData() const;
            std::shared_ptr<InputData> getInputData() const;

            /*! \brief Helper function for OMVisualizerAbstract::setVarReferencesInVisAttributes
            */
            fmi1_value_reference_t getVarReferencesForObjectAttribute(ShapeObjectAttribute* attr);

            /*! \brief Implementation for OMVisualizerAbstract::setVarReferencesInVisAttributes to set the variable references in the visattributes
            */
            int setVarReferencesInVisAttributes();

            /*-----------------------------------------
             * SIMULATION METHODS
             *---------------------------------------*/

            void simulate(Control::TimeManager& omvm) override;

            double simulateStep(const double time);

            /*! \brief This method updates the visualization attributes after a time step has been performed.
             *
             * The method updates the actual data for the visualization bodies by using variables from the FMU.
             *
             * \param time The visualization time.
             * \return Error value.
             */
            int updateVisAttributes(const double time) override;

            /*! \brief This method does a scene update, i.e., the FMU is integrated/simulated up to the given time.
             *
             * For FMU-based visualization, we have to simulate until the next visualization time step. This method is called
             * by the method \ref VisualizerAbstract::sceneUpdate, which does the time handling (visTime, simTime) around.
             *
             * \remark: Parameter time is not used, just inherited from \ref OMVisualizerAbstract::updateScene(const double).
             */
            void updateScene(const double time = 0.0) override;

            /*! \brief Update the attributes of a shape. */
            void updateObjectAttributeFMU(Model::ShapeObjectAttribute* attr, fmi1_import_t* fmu);

         private:
            /*-----------------------------------------
             * MEMBERS
             *---------------------------------------*/

            /*! The encapsulated FMU data. */
            std::shared_ptr<FMU> _fmu;
            /*! Simulation settings, e.g., start and end time. */
            std::shared_ptr<SimSettings> _simSettings;

            std::shared_ptr<InputData> _inputData;

         public:
            /// \todo Remove, we do not need it because we have inputData.
            std::vector<Control::JoystickDevice*> _joysticks;
        };

    }  // End namespace Model
}  // End namespace OMVIS

#endif /* INCLUDE_OMVISUALIZERFMU_HPP_ */
/**
 * @}
 */
