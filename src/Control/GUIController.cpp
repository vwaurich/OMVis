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

#include <Initialization/Factory.hpp>
#include "Control/GUIController.hpp"
#include "Control/OMVisManager.hpp"
#include "Model/OMVisualizerFMU.hpp"
#include "Model/OMVisualizerFMUClient.hpp"
#include "Util/Logger.hpp"
#include "Util/Util.hpp"

#include <sys/stat.h>
#include <stdexcept>

namespace OMVIS
{
    namespace Control
    {

        /*-----------------------------------------
         * CONSTRUCTORS
         *---------------------------------------*/

        GUIController::GUIController()
                : _omVisualizer(nullptr)
        {
        }

        /*-----------------------------------------
         * INITIALIZATION METHODS
         *---------------------------------------*/

        void GUIController::loadModel(const Initialization::VisualizationConstructionPlan& cP, const int timeSliderStart, const int timeSliderEnd)
        {
            LOGGER_WRITE(std::string("GUIController::loadModel()"), Util::LC_CTR, Util::LL_DEBUG);

            // Check for XML description file.
            bool xmlExists = Util::checkForXMLFile(cP.modelFile, cP.path);
            if (!xmlExists)
            {
                std::string msg = "Visual XML file does not exist in the same model file directory.";
                LOGGER_WRITE(msg, Util::LC_LOADER, Util::LL_ERROR);
                throw std::runtime_error(msg);
            }

            // Some useful output for the user and developer.
            LOGGER_WRITE(std::string("Path to model: ") + cP.path, Util::LC_CTR, Util::LL_DEBUG);
            LOGGER_WRITE(std::string("Model file: ") + cP.modelFile, Util::LC_CTR, Util::LL_DEBUG);
            LOGGER_WRITE(std::string("XML file exists: ") + Util::boolToString(xmlExists), Util::LC_CTR, Util::LL_DEBUG);

            // Corner case: The chosen model is the very same that is already loaded. In case of FMUs this means unpacking an
            // already unpacked shared object, which leads to a segmentation fault. Thats why we test for this case.
            if (modelIsLoaded() && cP.path == _omVisualizer->getBaseData()->getPath() && cP.modelFile == _omVisualizer->getBaseData()->getModelName())
            {
                LOGGER_WRITE(std::string("You tried to load the same model that is already loaded in OMVis. The model will be initialized again."), Util::LC_LOADER, Util::LL_WARNING);
                initVisualization();
            }
            else
            {
                // Okay, do we already have a model loaded? If so, we keep this loaded model in case of the new model cannot be loaded.
                int isOk(0);

                // Ask the factory to create an appropriate OMVisualizer object.
                Initialization::Factory* factory = new Initialization::Factory();
                std::shared_ptr<Model::OMVisualizerAbstract> tmpOmVisualizer = factory->createVisualizationObject(cP);
                if (tmpOmVisualizer != nullptr)
                {
                    tmpOmVisualizer->getOMVisManager()->setSliderRange(timeSliderStart, timeSliderEnd);

                    // Initialize the OMVisualizer object.
                    isOk += tmpOmVisualizer->initialize();

                    // If everything went fine, we "copy" the created OMvisualizer object to _omVisualizer.
                    if (0 == isOk)
                        _omVisualizer = tmpOmVisualizer;
                }
                else
                    LOGGER_WRITE(std::string("Something went wrong in loading the model."), Util::LC_LOADER, Util::LL_ERROR);
            }
        }

        void GUIController::loadModel(const Initialization::RemoteVisualizationConstructionPlan& cP, const int timeSliderStart, const int timeSliderEnd)
        {
            LOGGER_WRITE(std::string("GUIController::loadModel()"), Util::LC_CTR, Util::LL_DEBUG);

            // Check for XML description file. For remote visualization this file needs to be on the localhost.
            bool xmlExists = Util::checkForXMLFile(cP.modelFile, cP.workingDirectory);
            if (!xmlExists)
            {
                std::string msg = "Visual XML file does not exist on localhost in working directory.";
                LOGGER_WRITE(msg, Util::LC_LOADER, Util::LL_ERROR);
                throw std::runtime_error(msg);
            }

            // Some useful output for the user and developer.
            LOGGER_WRITE(std::string("IP address: ") + cP.ipAddress, Util::LC_CTR, Util::LL_DEBUG);
            LOGGER_WRITE(std::string("Model file: ") + cP.modelFile, Util::LC_CTR, Util::LL_DEBUG);
            LOGGER_WRITE(std::string("Path of model file: ") + cP.path, Util::LC_CTR, Util::LL_DEBUG);
            LOGGER_WRITE(std::string("Port number: ") + std::to_string(cP.portNumber), Util::LC_CTR, Util::LL_DEBUG);
            LOGGER_WRITE(std::string("Local working directory: ") + cP.workingDirectory, Util::LC_CTR, Util::LL_DEBUG);
            LOGGER_WRITE(std::string("XML file exists: ") + Util::boolToString(xmlExists), Util::LC_CTR, Util::LL_DEBUG);

            // Corner case: The chosen model is the very same that is already loaded. In case of FMUs this means unpacking an
            // already unpacked shared object, which leads to a segmentation fault. Thats why we test for this case.
            if (modelIsLoaded() && cP.workingDirectory == _omVisualizer->getBaseData()->getPath() && cP.modelFile == _omVisualizer->getBaseData()->getModelName())
            {
                LOGGER_WRITE(std::string("You tried to load the same model that is already loaded in OMVis. "
                                         "The model will be initialized again."), Util::LC_LOADER, Util::LL_WARNING);
                initVisualization();
            }
            else
            {
                // Okay, do we already have a model loaded? If so, we keep this loaded model in case of the new model cannot be loaded.
                int isOk(0);

                // Ask the factory to create an appropriate OMVisualizer object.
                Initialization::Factory* factory = new Initialization::Factory();
                std::shared_ptr<Model::OMVisualizerAbstract> tmpOmVisualizer = factory->createVisualizationObject(cP);
                if (tmpOmVisualizer != nullptr)
                {
                    tmpOmVisualizer->getOMVisManager()->setSliderRange(timeSliderStart, timeSliderEnd);

                    // Initialize the OMVisualizer object.
                    isOk += tmpOmVisualizer->initialize();

                    // If everything went fine, we "copy" the created OMvisualizer object to _omVisualizer.
                    if (0 == isOk)
                        _omVisualizer = tmpOmVisualizer;
                }
                else
                    LOGGER_WRITE(std::string("Something went wrong in loading the model."), Util::LC_LOADER, Util::LL_ERROR);
            }
        }

        void GUIController::unloadModel()
        {
            // Implicitly calls destructors and thus frees memory handled by shared pointers.
            _omVisualizer = nullptr;
            /// \todo: What else has to be done in order to clean up data structures and free memory?
        }

        /*-----------------------------------------
         * SIMULATION METHODS
         *---------------------------------------*/

        void GUIController::startVisualization()
        {
            _omVisualizer->startVisualization();
        }

        void GUIController::pauseVisualization()
        {
            _omVisualizer->pauseVisualization();
        }

        void GUIController::initVisualization()
        {
            // Todo: Check if input variables need to be specified in order to interact with the FMU. If so,
            // open a Gui.
            _omVisualizer->initVisualization();
        }

        void GUIController::sceneUpdate()
        {
            _omVisualizer->sceneUpdate();
        }

        void GUIController::donationVisualization()
        {
            std::cout << "Want to support Martin and Volker? Buy us a coffee." << std::endl;
        }

        /*-----------------------------------------
         * GETTERS AND SETTERS
         *---------------------------------------*/

        int GUIController::getTimeProgress()
        {
            return _omVisualizer->getOMVisManager()->getTimeProgress();
        }

        osg::ref_ptr<osg::Node> GUIController::getSceneRootNode()
        {
            return _omVisualizer->getOMVisScene()->getScene().getRootNode();
        }

        void GUIController::setVisTime(const int val)
        {
            _omVisualizer->getOMVisManager()->setVisTime((_omVisualizer->getOMVisManager()->getEndTime() - _omVisualizer->getOMVisManager()->getStartTime()) * (float) (val / 100.0));
        }

        double GUIController::getVisTime()
        {
            return _omVisualizer->getOMVisManager()->getVisTime();
        }

        double GUIController::getRealTimeFactor()
        {
            return _omVisualizer->getOMVisManager()->getRealTimeFactor();
        }

        double GUIController::getSimulationStartTime() const
        {
            return _omVisualizer->getOMVisManager()->getStartTime();
        }

        double GUIController::getVisStepsize()
        {
            return _omVisualizer->getOMVisManager()->getHVisual() * 1000.0;
        }

        bool GUIController::modelIsMATFile()
        {
            return (_omVisualizer->getType() == "mat") ? true : false;
        }

        bool GUIController::modelIsFMU()
        {
            return (_omVisualizer->getType() == "fmu") ? true : false;
        }

        bool GUIController::modelIsFMUClient()
        {
            return (_omVisualizer->getType() == "fmuclient") ? true : false;
        }

        bool GUIController::modelIsLoaded()
        {
            return (nullptr != _omVisualizer);
        }

        std::shared_ptr<Model::InputData> GUIController::getInputData()
        {
            if (modelIsFMU())
            {
                std::shared_ptr<Model::OMVisualizerFMU> omVisFMU = std::dynamic_pointer_cast<Model::OMVisualizerFMU>(_omVisualizer);
                return omVisFMU->getInputData();
            }
            else if (modelIsFMUClient())
            {
                std::shared_ptr<Model::OMVisualizerFMUClient> omVisFMU = std::dynamic_pointer_cast<Model::OMVisualizerFMUClient>(_omVisualizer);
                return omVisFMU->getInputData();
            }
            else
                return nullptr;
        }

    }  // End namespace Control
}  // End namespace OMVIS
