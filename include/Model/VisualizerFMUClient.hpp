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

#ifndef INCLUDE_MODEL_VISUALIZERFMUCLIENT_HPP_
#define INCLUDE_MODEL_VISUALIZERFMUCLIENT_HPP_

#include "Model/FMUWrapper.hpp"
#include "Model/VisualizerAbstract.hpp"
#include "Initialization/VisualizationConstructionPlans.hpp"
#include "Model/SimSettings.hpp"
#include "Model/InputData.hpp"
#include "Control/JoystickDevice.hpp"
#include "Control/KeyboardEventHandler.hpp"

// NetOff
#include <VariableList.hpp>
#include <SimulationClient.hpp>

#include <string>

namespace OMVIS
{
    namespace Model
    {

        /*! \brief Class that handles remote visualization of FMUs.
         *
         * This class is used in case the FMU is computed on a server.
         */
        class VisualizerFMUClient : public VisualizerAbstract
        {
         public:
            /*-----------------------------------------
             * CONSTRUCTORS
             *---------------------------------------*/

            VisualizerFMUClient() = delete;

            /*! \brief Constructs a OMVisualizerFMUClient object from the given construction plan for remote FMU
             *         visualization.
             *
             * The construction plan holds information about the server and the port to use, the path to the model
             * file and declaration of a local working directory (\ref RemoteVisualizationConstructionPlan).
             *
             * \param cP The construction plan which holds all necessary information.
             */
            VisualizerFMUClient(const Initialization::RemoteVisualizationConstructionPlan* cP);

            virtual ~VisualizerFMUClient() = default;
            VisualizerFMUClient(const VisualizerFMUClient& omvf) = delete;
            VisualizerFMUClient& operator=(const VisualizerFMUClient& omvf) = delete;

            /*-----------------------------------------
             * INITIALIZATION METHODS
             *---------------------------------------*/

            void loadFMU();

            /*! \brief Initializes OMVisualizerFMUClient object.
             *
             */
            void initData() override;

            /*! \brief This methods resets the input values of a FMU to default ("zero") values.
             *
             */
            void resetInputs();

            /*! \brief initializes the attached joysticks
             */
            void initJoySticks();

            /*! \brief Initializes the OMVisualizerFMUClient object.
             *
             * This methods overload the initialize method of the base class \ref OMVisualizerAbstract.
             * The connection to the remote visualization server is established and the simulation is initialized.
             */
            void initialize() override;

            /*! \brief Initializes the connection to the server.
             *
             * This methods throws a std::runtime_error exception, if the connection to the server cannot be
             * established.
             */
            void initializeConnectionToServer();

            /*! \brief Implementation for OMVisualizerAbstract::initializeVisAttributes to set the
             *         scene to initial position.
             *
             * @remark: Parameter time is not used, just inherited from
             *          \ref OMVisualizerAbstract::initializeVisAttributes(const double).
             */
            void initializeVisAttributes(const double time = 0.0) override;

            /*-----------------------------------------
             * GETTERS and SETTERS
             *---------------------------------------*/

            /*! \brief Sets the output variables for the simulation.
             *
             * The output variables are the ones which are necessary for the visualization. Thus,
             * their values have to be communicated from the server to the localhost. The output
             * variables are gathered from the visual XML file.
             *
             * @remark: For remote visualization, only real values are necessary as output
             *          variables.
             *
             * @return Container with the names of the output variables.
             */
            NetOff::VariableList getOutputVariables();

            /*! \brief Sets the input variables for the simulation.
             *
             * The input variables are the ones which allow steering the model and simulation,
             * respectively. Thus, their values have to be communicated from the localhost to the
             * server.
             *
             * \todo Todo Currently we return all possible input variables by asking NetworkOffloader.
             *       We need a GUI dialog to let the user define the variable which one want to control.
             *
             * @return Container with the names of the input variables.
             */
            NetOff::VariableList getInputVariables();

            std::shared_ptr<InputData> getInputData();

            int setVarReferencesInVisAttributes();

            // Todo pass by const ref
            fmi1_value_reference_t getVarReferencesForObjectAttribute(ShapeObjectAttribute* attr);

            /*-----------------------------------------
             * SIMULATION METHODS
             *---------------------------------------*/

            /*! \brief This methods performs the simulation of the FMU.
             *
             * The simulation is performed until the simulation end time is reached or the
             * simulation is paused.
             *
             * @param omvm
             */
            virtual void simulate(Control::TimeManager& omvm) override;

            /*! \brief Performs a simulation step to obtain data for next visualization frame.
             *
             * A single visualization step might consists of several time steps for the FMU.
             * This method sends the inputs to the server and receives the outputs afterwards.
             *
             * @param time The time to get output values for.
             */
            double simulateStep(const double time);

            /*! \brief This method updates the visualization attributes after a time step has been performed.
             *
             * The method updates the actual data for the visualization bodies by using variables from the FMU.
             *
             * \param time  The visualization time.
             */
            void updateVisAttributes(const double time) override;

            /*! \brief For FMU-based visualization, we have to simulate until the next
             *         visualization time step.
             *
             *  @remark: Parameter time is not used, just inherited from
             *           \ref OMVisualizerAbstract::updateScene(const double).
             */
            void updateScene(const double time = 0.0) override;

            void startVisualization() override;
            void pauseVisualization() override;

            void updateObjectAttributeFMU(Model::ShapeObjectAttribute* attr, double time, fmi1_import_t* fmu);

         private:
            /*-----------------------------------------
             * MEMBERS
             *---------------------------------------*/

            /*! Network simulation client. */
            NetOff::SimulationClient _noFC;

            /*! ID of the simulation. */
            int _simID;

            /*! Simulation settings, e.g., start and end time. */
            std::shared_ptr<SimSettings> _simSettings;

            /*! Names of all output variables. */
            NetOff::VariableList _outputVars;

            std::shared_ptr<InputData> _inputData;

         public:
            /// \todo Remove, we do not need it because we have inputData.
            std::vector<Control::JoystickDevice*> _joysticks;

         private:
            std::string _remotePathToModelFile;
        };

    }  // End namespace Model
}  // End namespace OMVIS

#endif /* INCLUDE_MODEL_VISUALIZERFMUCLIENT_HPP_ */
/**
 * @}
 */
