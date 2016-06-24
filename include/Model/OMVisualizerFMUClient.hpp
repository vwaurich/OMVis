/*
 * OMVisualizerFMUClient.hpp
 *
 *  Created on: 13.05.2016
 *      Author: mf
 */

#ifndef INCLUDE_MODEL_OMVISUALIZERFMUCLIENT_HPP_
#define INCLUDE_MODEL_OMVISUALIZERFMUCLIENT_HPP_

#include "FMU.hpp"
#include "Model/SimSettings.hpp"
#include "Model/InputData.hpp"
#include "Model/OMVisualizerAbstract.hpp"
#include "Visualize.hpp"
#include "Util/Logger.hpp"

#include "Control/JoystickDevice.hpp"
#include "Control/KeyboardEventHandler.hpp"
#include "Initialization/OMVisFactory.hpp"
#include "Initialization/VisualizationConstructionPlan.hpp"

// NetworkOffloader
#include <SimulationClient.hpp>
#include <VariableList.hpp>

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

        /*! \brief Class that handles remote visualization of FMUs.
         *
         * This class is used in case the FMU is computed on a server.
         */
        class OMVisualizerFMUClient : public OMVisualizerAbstract
        {
         public:
            /*-----------------------------------------
             * CONSTRUCTORS
             *---------------------------------------*/

            OMVisualizerFMUClient() = delete;

            /*! \brief Constructs a OMVisualizerFMUClient object from the given construction plan for remote FMU
             *         visualization.
             *
             * The construction plan holds information about the server and the port to use, the path to the model
             * file and declaration of a local working directory (\ref RemoteVisualizationConstructionPlan).
             *
             * @param[in] cP The construction plan which holds all necessary information.
             */
            OMVisualizerFMUClient(const Initialization::RemoteVisualizationConstructionPlan& cP);

            virtual ~OMVisualizerFMUClient() = default;
            OMVisualizerFMUClient(const OMVisualizerFMUClient& omvf) = delete;
            OMVisualizerFMUClient& operator=(const OMVisualizerFMUClient& omvf) = delete;

            /*-----------------------------------------
             * INITIALIZATION METHODS
             *---------------------------------------*/

            int loadFMU();

            /*! \brief Initializes OMVisualizerFMUClient object.
             *
             */
            int initData();

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
             *
             * @return
             */
            virtual int initialize()
            {
                // If visual XML file is not present, we need to copy it from server to localhost
                // (before calling OMVisualizerAbstract::initialize().

                int isOk(0);
                try
                {
                    initializeConnectionToServer();
                }
                catch (std::exception& ex)
                {
                    LOGGER_WRITE(std::string("OMVisualizerFMUClient exception: ") + ex.what(), Util::LC_LOADER, Util::LL_ERROR);
                    isOk += 1;
                }

                OMVisualizerAbstract::initialize();

                //initializeSimulation();

                return isOk;
            }

            /*! \brief Initializes the connection to the server. */
            void initializeConnectionToServer();

            //! This is done in loadFMU().
            /*! \brief Initializes remote simulation.
             *
             * The simulation is added to the NetworkOffloader client and the simulation ID is set.
             * Input and output variables are defined and communicated to the server.
             */
            void initializeSimulation();

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

            /*! \brief Returns "fmuclient".
             */
            std::string getType() const;

            std::shared_ptr<InputData> getInputData();

            int setVarReferencesInVisAttributes();

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
            virtual void simulate(Control::OMVisManager& omvm);

            /*! \brief Performs a simulation step to obtain data for next visualization frame.
             *
             * A single visualization step might consists of several time steps for the FMU.
             * This method sends the inputs to the server and receives the outputs afterwards.
             *
             * @param time The time to get output values for.
             */
            double simulateStep(const double time);

            /*! \brief This method updates the actual data for the visualization bodies by using
             *         variables from the FMU.
             *
             * @return Error value.
             */
            int updateVisAttributes(const double time);

            /*! \brief Implementation for OMVisualizerAbstract::initializeVisAttributes to set the
             *         scene to initial position.
             *
             * @remark: Parameter time is not used, just inherited from
             *          \ref OMVisualizerAbstract::initializeVisAttributes(const double).
             */
            void initializeVisAttributes(const double time = 0.0);

            /*! \brief For FMU-based visualization, we have to simulate until the next
             *         visualization time step.
             *
             *  @remark: Parameter time is not used, just inherited from
             *           \ref OMVisualizerAbstract::updateScene(const double).
             */
            void updateScene(const double time = 0.0);

            void startVisualization();
            void pauseVisualization();

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

            /*! Number of attached joysticks. */
            int _numJoysticks;

         public:
            /// \todo Remove, we do not need it because we have inputData.
            std::vector<Control::JoystickDevice*> _joysticks;

         private:
            std::string _remotePathToModelFile;
        };

    }  // End namespace Model
}  // End namespace OMVIS

#endif /* INCLUDE_MODEL_OMVISUALIZERFMUCLIENT_HPP_ */
