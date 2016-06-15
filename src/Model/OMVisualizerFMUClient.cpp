/*
 * OMVisualizerFMUClient.cpp
 *
 *  Created on: 13.05.2016
 *      Author: mf
 */

#include "Model/OMVisualizerFMUClient.hpp"
#include "Control/OMVisManager.hpp"

namespace OMVIS
{
    namespace Model
    {

        /*-----------------------------------------
         * CONSTRUCTORS
         *---------------------------------------*/

        OMVisualizerFMUClient::OMVisualizerFMUClient(const Initialization::RemoteVisualizationConstructionPlan& cP)
//                : OMVisualizerAbstract(cP.modelFile, cP.workingDirectory),
//                  _noFC(cP.ipAddress, cP.portNumber),
//                  _workingDir(cP.workingDirectory),
//                  _simID(-9)
        {
        }

        /*-----------------------------------------
         * INITIALIZATION METHODS
         *---------------------------------------*/

        int OMVisualizerFMUClient::initData()
        {
            int isOk(0);
            isOk = OMVisualizerAbstract::initData();
//                {
//                int isOk(0);
//                // In case of reloading, we need to make sure, that we have empty members
//                _baseData->clearXMLDoc();
//
//                // Initialize xml file and get visAttributes
//                isOk = _baseData->initXMLDoc();
//
//                isOk = _baseData->initVisObjects();
//                return isOk;
//                }

            // Todo: Catch exception if necessary.
            initializeConnectionToServer();

            setOutputVariables();

            _simSettings->setTend(_omvManager->getEndTime());
            _simSettings->setHdef(0.001);

            return isOk;
        }

        void OMVisualizerFMUClient::initializeConnectionToServer()
        {
            // Test if server can be reached.
            if (!_noFC.initializeConnection())
                throw std::runtime_error("Could not initialize connection with host server.");

            // Send server data about the simulation you want to calculate and get the ID of the simulation.
            _simID = _noFC.addSimulation(_baseData->getModelName());
        }

        void OMVisualizerFMUClient::setOutputVariables()
        {
            NetOff::VariableList allOutputs = _noFC.getPossibleOuputVariableNames(_simID);
            // Copy MODEL_visual.xml file from server to localhost.
            /// \todo Todo: Define a proper target location of the file.
            _noFC.getSimulationFile(_simID, _baseData->getXMLFileName(), "/home/mf/opt/HPCOM/OMVis.git_eclipse/test_visual.xml");
            // OutputVariablen in XML:
            // Welche Variablen brauchen wir fuer die Darstellung?
            // Auswahl der Variablen
        }

    }  // End namespace Model
}  // End namespace OMVIS
