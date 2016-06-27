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
#include <memory>

// Forward declaration
namespace OMVIS
{
    namespace View
    {
        class OMVManager;
    }
}

namespace OMVIS
{
    namespace Model
    {

        /*! \brief Class that handles visualization of FMUs.
         *
         *
         * In contrast to \ref OMVisualizationMAT, this class provides user interaction via joystick devices.
         */
        class OMVisualizerFMU : public OMVisualizerAbstract
        {
         public:
            /*-----------------------------------------
             * CONSTRUCTORS
             *---------------------------------------*/

            OMVisualizerFMU() = delete;

            /*! \brief Constructs a OMVisualizerFMU object from the given arguments.
             *
             * Essentially, a FMU file and its path need to be specified.
             *
             * @param[in] fileName Model file name without path.
             * @param[in] path Path to the model file.
             */
            OMVisualizerFMU(const std::string& modelFile, const std::string& path);

            virtual ~OMVisualizerFMU() = default;
            OMVisualizerFMU(const OMVisualizerFMU& omvf) = delete;
            OMVisualizerFMU& operator=(const OMVisualizerFMU& omvf) = delete;

            /*-----------------------------------------
             * INITIALIZATION METHODS
             *---------------------------------------*/

            /*! \brief Loads and initializes the FMU file.
             *
             * @param[in] modelFile Name of the FMU file.
             * @param[in] path Path to the model file.
             * @return Error value.
             */
            int loadFMU(const std::string& modelFile, const std::string& path);

            /*! \todo Quick and dirty hack, move initialization of _simSettings to a more appropriate place!
             * @return Error value.
             */
            int initData() override;

            /*! \brief This methods resets the input values of a FMU to default ("zero") values.
             *
             */
            void resetInputs();

            /*! \brief initializes the attached joysticks
             */
            void initJoySticks();

            /*! \brief Implementation for OMVisualizerAbstract::initializeVisAttributes to set the scene to initial position.
             *
             * \Remark: Parameter time is not used, just inherited from \ref OMVisualizerAbstract::initializeVisAttributes(const double).
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
            std::shared_ptr<InputData> getInputData();

            /*! \brief Helper function for OMVisualizerAbstract::setVarReferencesInVisAttributes
            */
            fmi1_value_reference_t getVarReferencesForObjectAttribute(ShapeObjectAttribute* attr);

            /*-----------------------------------------
             * SIMULATION METHODS
             *---------------------------------------*/

            void simulate(Control::OMVisManager& omvm) override;

            double simulateStep(const double time);

            /*! \brief This method updates the visualization attributes after a time step has been performed.
             *
             * The method updates the actual data for the visualization bodies by using variables from the FMU.
             *
             * \param time The visualization time.
             * \return Error value.
             */
            int updateVisAttributes(const double time) override;

			/*! \brief Implementation for OMVisualizerAbstract::setVarReferencesInVisAttributes to set the variable references in the visattributes
			*/
			int setVarReferencesInVisAttributes();

            /*! \brief For FMU-based visualization, we have to simulate until the next visualization time step.
             *
             *  \remark: Parameter time is not used, just inherited from \ref OMVisualizerAbstract::updateScene(const double).
             */
            void updateScene(const double time = 0.0) override;

            /*! \brief Update the attribute of the Object using a mat-file result. */
            void updateObjectAttributeFMU(Model::ShapeObjectAttribute* attr, double time, fmi1_import_t* fmu);

         private:
            /*-----------------------------------------
             * MEMBERS
             *---------------------------------------*/

            /*! The encapsulated FMU data. */
            std::shared_ptr<FMU> _fmu;
            /*! Simulation settings, e.g., start and end time. */
            std::shared_ptr<SimSettings> _simSettings;

            /*! Number of attached joysticks. */
            int _numJoysticks;

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
