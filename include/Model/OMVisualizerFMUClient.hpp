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

            OMVisualizerFMUClient(const Initialization::RemoteVisualizationConstructionPlan& in);

            virtual ~OMVisualizerFMUClient() = default;
            OMVisualizerFMUClient(const OMVisualizerFMUClient& omvf) = delete;
            OMVisualizerFMUClient& operator=(const OMVisualizerFMUClient& omvf) = delete;

            /*-----------------------------------------
             * INITIALIZATION METHODS
             *---------------------------------------*/

            virtual int initialize()
            {
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

                return isOk;
            }

            /*! \brief Initializes the connection to the server. */
            void initializeConnectionToServer();

//            /*! \brief Loads and initializes FMU file.
//             *
//             * @param model Name of the FMU.
//             * @param dir Path to the FMU file.
//             * @return Error value.
//             */
//            int loadFMU(const std::string& model, const std::string& dir);
//
            /*! \brief Initializes OMVisualizerFMUClient object.
             *
             * ??
             */
            int initData();

            void setOutputVariables();

//            /*! \brief This methods resets the input values of a FMU to default ("zero") values.
//             *
//             */
//            void resetInputs();
//
//            /*! \brief initializes the attached joysticks
//             */
//            void initJoySticks();
//
//            /*-----------------------------------------
//             * GETTERS and SETTERS
//             *---------------------------------------*/
//
            /*! \brief Returns "fmu".
             */
            std::string getType() const
            {
                return "FMUCLient";
            }
//
//            /*! Returns const pointer to \ref FMU member. */
//            const FMU* getFMU() const;
//
//            // const InputData* getInputData() const;
//            std::shared_ptr<InputData> getInputData();
//
            /*-----------------------------------------
             * SIMULATION METHODS
             *---------------------------------------*/

            virtual void simulate(Control::OMVisManager& omvm)
            {  //Implement me
            }

            double simulateStep(const double time);

            /*! \brief This method updates the actual data for the visualization bodies by using variables from the FMU.
             *
             * \return Error value.
             */
            int updateVisAttributes(const double time)
            {
                // Implement me!
            }

            /*! \brief Implementation for OMVisualizerAbstract::initializeVisAttributes to set the scene to initial position.
             *
             * \Remark: Parameter time is not used, just inherited from \ref OMVisualizerAbstract::initializeVisAttributes(const double).
             */
            void initializeVisAttributes(const double time = 0.0)
            {
                // Implement me!
            }

            /*! \brief For FMU-based visualization, we have to simulate until the next visualization time step.
             *
             *  \Remark: Parameter time is not used, just inherited from \ref OMVisualizerAbstract::updateScene(const double).
             */
            void updateScene(const double time = 0.0)
            {
                // Implement me!
            }

         private:
            /*-----------------------------------------
             * MEMBERS
             *---------------------------------------*/

            NetOff::SimulationClient _noFC;

            /*! ID of the simulation. */
            int _simID;

            /*! Simulation settings, e.g., start and end time. */
            std::shared_ptr<SimSettings> _simSettings;

            /// \todo Todo: [MF] It might be useful to set a working directory also for local
            ///                  simulations. I.e., move this variable to BaseData class.
            std::string _workingDir;

            //            /*! The encapsulated FMU data. */
            //            std::shared_ptr<FMU> _fmu;
//
//            /*! Number of attached joysticks. */
//            int _numJoysticks;
//
//            std::shared_ptr<InputData> _inputData;
//
//         public:
//            /// \todo Remove, we do not need it because we have inputData.
//            std::vector<Control::JoystickDevice*> _joysticks;
        };

    }  // End namespace Model
}  // End namespace OMVIS

#endif /* INCLUDE_MODEL_OMVISUALIZERFMUCLIENT_HPP_ */
