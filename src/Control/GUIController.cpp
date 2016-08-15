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

#include "Model/VisualizerFMU.hpp"
#include "Model/VisualizerMAT.hpp"
#include "Model/VisualizerFMUClient.hpp"
#include "Control/TimeManager.hpp"
#include "Initialization/Factory.hpp"
#include "Control/GUIController.hpp"
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
                : _modelVisualizer(nullptr)
        {
        }

        /*-----------------------------------------
         * INITIALIZATION METHODS
         *---------------------------------------*/

        void GUIController::loadModel(const Initialization::VisualizationConstructionPlan& cP, const int timeSliderStart,
                                      const int timeSliderEnd)
        {
            LOGGER_WRITE(std::string("GUIController::loadModel()"), Util::LC_CTR, Util::LL_DEBUG);

            // Check for XML description file.
            bool xmlExists = Util::checkForXMLFile(cP.modelFile, cP.path);
            if (!xmlExists)
            {
                std::string msg = "Could not find the visual XML file "+ Util::getXMLFileName(cP.modelFile, cP.path) +" in the model file directory "+ cP.path;
                LOGGER_WRITE(msg, Util::LC_LOADER, Util::LL_ERROR);
                throw std::runtime_error(msg);
            }

            // Some useful output for the user and developer.
            LOGGER_WRITE(std::string("Path to model: ") + cP.path, Util::LC_CTR, Util::LL_DEBUG);
            LOGGER_WRITE(std::string("Model file: ") + cP.modelFile, Util::LC_CTR, Util::LL_DEBUG);
            LOGGER_WRITE(std::string("XML file exists: ") + Util::boolToString(xmlExists), Util::LC_CTR, Util::LL_DEBUG);

            // Corner case: The chosen model is the very same that is already loaded. In case of FMUs this means
            // unpacking an already unpacked shared object, which leads to a segmentation fault. Thats why we test for
            // this case. If the model is already loaded, we initialize it again.
            if (modelIsLoaded() && cP.path == _modelVisualizer->getBaseData()->getPath()
                                && cP.modelFile == _modelVisualizer->getBaseData()->getModelFile())
            {
                LOGGER_WRITE(std::string("You tried to load the same model that is already loaded in OMVis. "
                                         "The model will be initialized again."),
                             Util::LC_LOADER, Util::LL_WARNING);
                initVisualization();
            }
            else
                loadModelHelper(&cP, timeSliderStart, timeSliderEnd);
        }

        void GUIController::loadModel(const Initialization::RemoteVisualizationConstructionPlan& cP, const int timeSliderStart,
                                      const int timeSliderEnd)
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

            // Corner case: The chosen model is the very same that is already loaded. In case of FMUs this means
            // unpacking an already unpacked shared object, which leads to a segmentation fault. Thats why we test for
            // this case. If the model is already loaded, we initialize it again.
            if (modelIsLoaded() && cP.workingDirectory == _modelVisualizer->getBaseData()->getPath()
                                && cP.modelFile == _modelVisualizer->getBaseData()->getModelFile())
            {
                LOGGER_WRITE(std::string("You tried to load the same model that is already loaded in OMVis. "
                                         "The model will be initialized again."),
                             Util::LC_LOADER, Util::LL_WARNING);
                initVisualization();
            }
            else
                loadModelHelper(dynamic_cast<const Initialization::VisualizationConstructionPlan*>(&cP), timeSliderStart,
                                timeSliderEnd);
        }

        void GUIController::loadModelHelper(const Initialization::VisualizationConstructionPlan* cP, const int timeSliderStart,
                                            const int timeSliderEnd)
        {
            // Okay, do we already have a model loaded? If so, we keep this loaded model in case of the new model
            // cannot be loaded.

            // Ask the factory to create an appropriate Visualizer object.
            Initialization::Factory* factory = new Initialization::Factory();
            std::shared_ptr<Model::VisualizerAbstract> tmpVisualizer = factory->createVisualizerObject(cP);

            if (tmpVisualizer != nullptr)
            {
                tmpVisualizer->getTimeManager()->setSliderRange(timeSliderStart, timeSliderEnd);

                // Initialize the visualizer object.
                tmpVisualizer->initialize();
            }
            else
            {
                std::string msg = "Could not load model. Factory returned nullptr.";
                LOGGER_WRITE(msg, Util::LC_LOADER, Util::LL_ERROR);
                throw std::runtime_error(msg);
            }

            // If everything went fine, we "copy" the created Visualizer object to _omVisualizer.
            _modelVisualizer = tmpVisualizer;
        }

        void GUIController::unloadModel()
        {
            // Implicitly calls destructors and thus frees memory handled by shared pointers.
            _modelVisualizer = nullptr;
            /// \todo: What else has to be done in order to clean up data structures and free memory?
        }

        /*-----------------------------------------
         * SIMULATION METHODS
         *---------------------------------------*/

        void GUIController::startVisualization()
        {
            if (_modelVisualizer != nullptr)
                _modelVisualizer->startVisualization();
        }

        void GUIController::pauseVisualization()
        {
            if (_modelVisualizer != nullptr)
                _modelVisualizer->pauseVisualization();
        }

        void GUIController::initVisualization()
        {
            // Todo: Check if input variables need to be specified in order to interact with the FMU. If so,
            // open a Gui.
            if (_modelVisualizer != nullptr)
                _modelVisualizer->initVisualization();
        }

        void GUIController::sceneUpdate()
        {
            _modelVisualizer->sceneUpdate();
        }

        /*-----------------------------------------
         * GETTERS AND SETTERS
         *---------------------------------------*/

        int GUIController::getTimeProgress()
        {
            return _modelVisualizer->getTimeManager()->getSliderPosition();
        }

        osg::ref_ptr<osg::Node> GUIController::getSceneRootNode() const
        {
            return _modelVisualizer->getOMVisScene()->getScene().getRootNode();
        }

        void GUIController::setVisTime(const int val)
        {
            _modelVisualizer->getTimeManager()->setVisTime((_modelVisualizer->getTimeManager()->getEndTime()
                                                            - _modelVisualizer->getTimeManager()->getStartTime())
                                                            * (float) (val / 100.0));
        }

        double GUIController::getVisTime()
        {
            return _modelVisualizer->getTimeManager()->getVisTime();
        }

        double GUIController::getRealTimeFactor()
        {
            return _modelVisualizer->getTimeManager()->getRealTimeFactor();
        }

        double GUIController::getSimulationStartTime() const
        {
            return _modelVisualizer->getTimeManager()->getStartTime();
        }

        double GUIController::getVisStepsize()
        {
            return _modelVisualizer->getTimeManager()->getHVisual() * 1000.0;
        }

        bool GUIController::visTypeIs(const Model::VisType visType) const
        {
            return (_modelVisualizer->getVisType() == visType) ? true : false;
        }

        bool GUIController::visTypeIsNone() const
        {
            return visTypeIs(Model::VisType::NONE);
        }

        bool GUIController::visTypeIsFMU() const
        {
            return visTypeIs(Model::VisType::FMU);
        }

        bool GUIController::visTypeIsFMURemote() const
        {
            return visTypeIs(Model::VisType::FMU_REMOTE);
        }

        bool GUIController::visTypeIsMAT() const
        {
            return visTypeIs(Model::VisType::MAT);
        }

        bool GUIController::visTypeIsMATRemote() const
        {
            return visTypeIs(Model::VisType::MAT_REMOTE);
        }

        std::string GUIController::getModelFile() const
        {
            return _modelVisualizer->getModelFile();
        }

        bool GUIController::modelIsLoaded()
        {
            return (nullptr != _modelVisualizer);
        }

        std::shared_ptr<Model::InputData> GUIController::getInputData()
        {
            if (visTypeIsFMU())
            {
                std::shared_ptr<Model::VisualizerFMU> omVisFMU =
                                                     std::dynamic_pointer_cast<Model::VisualizerFMU>(_modelVisualizer);
                return omVisFMU->getInputData();
            }
            else if (visTypeIsFMURemote())
            {
                std::shared_ptr<Model::VisualizerFMUClient> omVisFMU =
                                               std::dynamic_pointer_cast<Model::VisualizerFMUClient>(_modelVisualizer);
                return omVisFMU->getInputData();
            }
            else
                return nullptr;
        }

        void GUIController::handleSimulationSettings(const Model::UserSimSettingsFMU& simSetFMU)
        {
            if (visTypeIsFMU())
            {
                _modelVisualizer->setSimulationSettings(simSetFMU);
                initVisualization();
            }
            else if (visTypeIsFMURemote())
                LOGGER_WRITE("Not yet implemented for FMU Remote visualization.", Util::LC_CTR, Util::LL_INFO);
            else
                throw std::runtime_error("Wrong function called");
        }

        void GUIController::handleSimulationSettings(const Model::UserSimSettingsMAT& simSetMAT)
        {
            if (visTypeIsMAT())
            {
                std::dynamic_pointer_cast<Model::VisualizerMAT>(_modelVisualizer)->setSimulationSettings(simSetMAT);
                //initVisualization();
            }
            else if (visTypeIsMATRemote())
                LOGGER_WRITE("Not yet implemented for MAT Remote visualization.", Util::LC_CTR, Util::LL_INFO);
            else
                throw std::runtime_error("Wrong function called");
        }

    }  // End namespace Control
}  // End namespace OMVIS
