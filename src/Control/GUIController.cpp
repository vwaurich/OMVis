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

#include "Control/GUIController.hpp"
#include "Control/OMVisManager.hpp"
#include "Model/OMVisualizerAbstract.hpp"
#include "Model/OMVisualizerFMU.hpp"
#include "Initialization/Factory.hpp"
#include "Util/Logger.hpp"
#include "Util/Util.hpp"

#include <string>
#include <exception>
#include <sys/stat.h>

namespace Control
{
    GUIController::GUIController()
            : _omVisualizer(nullptr),
              _modelLoaded(false)
    {
    }

    void GUIController::unloadModel()
    {
        _omVisualizer->unload();
        _modelLoaded = false;
        /// \todo: What else has to be done in order to clean up data structures and free memory?
    }

    void GUIController::loadModel(const std::string& modelNameIn, const int timeSliderStart, const int timeSliderEnd)
    {
        LOGGER_WRITE(std::string("GUIController::loadModel()"), Util::LC_CTR, Util::LL_DEBUG);

        // Get file name and path from the users selection.
        std::size_t pos = modelNameIn.find_last_of("/");
        std::string path = modelNameIn.substr(0, pos + 1);
        std::string modelName = modelNameIn.substr(pos + 1, modelNameIn.length());

        // Do we visualize fmu or mat file?
        bool visFMU(false);
        std::size_t fmu = modelName.find(".fmu");
        if (fmu != std::string::npos)
        {
            visFMU = true;
            modelName = modelName.substr(0, fmu);
        }
        else
        {
            visFMU = false;
            // Remove '_res' from the model file name, because the XML file for MODEL_res.mat it named MODEL_visual.xml.
            modelName = modelName.substr(0, modelName.find("_res"));
        }

        // Check for XML description file.
        bool xmlExists = checkForXMLFile(path, modelName);

        // Some useful output for the user and developer.
        LOGGER_WRITE(std::string("Path to model: ") + path, Util::LC_CTR, Util::LL_DEBUG);
        LOGGER_WRITE(std::string("Model file: ") + modelName, Util::LC_CTR, Util::LL_DEBUG);
        LOGGER_WRITE(std::string("XML file exists: ") + Util::boolToString(xmlExists), Util::LC_CTR, Util::LL_DEBUG);

        // Corner case: The chosen model is the very same that is already loaded. In case of FMUs this means unpacking an
        // already unpacked shared object, which leads to a seg fault. Thats why we test for this case.
        // if (_modelLoaded && path.compare(_omVisualizer->_baseData->_dirName) &&  modelName.compare(_omVisualizer->_baseData->_modelName))
        if (_modelLoaded && path == _omVisualizer->_baseData->_dirName && modelName == _omVisualizer->_baseData->_modelName)
        {
            LOGGER_WRITE(std::string("You tried to load the same model that is already loaded in OMVis. The model will be initialized again."),
                         Util::LC_LOADER, Util::LL_WARNING);
            initVisualization();
        }
        else
        {
            // Okay, do we already have a model loaded? If so, we keep this loaded model in case of the new model cannot be loaded.
            int isOk(0);

            // Ask the factory to create an appropriate OMVisualizer object.
            Initialization::Factory* factory = new Initialization::Factory();
            Model::OMVisualizerAbstract* tmpOmVisualizer = factory->createVisualizationObject(modelName, path, visFMU);
            if (tmpOmVisualizer != nullptr)
            {
                tmpOmVisualizer->_omvManager->setSliderRange(timeSliderStart, timeSliderEnd);

                // Initialize the OMVisualizer object.
                isOk += tmpOmVisualizer->initialize();

                // If everything went fine, we "copy" the created OMvisualizer object to _omVisualizer.
                if (0 == isOk)
                {
                    _omVisualizer = tmpOmVisualizer;
                    _modelLoaded = true;
                }
            }
            else
            {
                LOGGER_WRITE(std::string("Something went wrong in loading the model."), Util::LC_LOADER, Util::LL_ERROR);
                _modelLoaded = false;
            }
        }
    }

    bool GUIController::checkForXMLFile(const std::string& path, const std::string& modelName) const
    {
        struct stat buffer;
        std::string file = path + modelName + "_visual.xml";
        return (stat(file.c_str(), &buffer) == 0);
    }

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
        _omVisualizer->initVisualization();
    }

    void GUIController::donationVisualization()
    {
        std::cout << "Want to support Martin and Volker? Buy us a coffee." << std::endl;
    }

    int GUIController::getTimeProgress()
    {
        return _omVisualizer->_omvManager->getTimeProgress();
    }

    osg::ref_ptr<osg::Node> GUIController::getSceneRootNode()
    {
        return _omVisualizer->_viewerStuff->getScene().getRootNode();
    }

    void GUIController::sceneUpdate()
    {
        _omVisualizer->sceneUpdate();
    }

    double GUIController::getVisTime()
    {
        return _omVisualizer->_omvManager->getVisTime();
    }

    void GUIController::setVisTime(const int val)
    {
        _omVisualizer->_omvManager->setVisTime((_omVisualizer->_omvManager->getEndTime() - _omVisualizer->_omvManager->getStartTime()) * (float) (val / 100.0));
    }

    bool GUIController::modelIsMATFile()
    {
        return (_omVisualizer->getType() == "mat") ? true : false;
    }

    bool GUIController::modelIsFMU()
    {
        return (_omVisualizer->getType() == "fmu") ? true : false;
    }

    double GUIController::getSimulationStartTime() const
    {
        return _omVisualizer->_omvManager->getStartTime();
    }

    double GUIController::getVisStepsize()
    {
        return _omVisualizer->_omvManager->getHVisual() * 1000.0;
    }

    Model::InputData* GUIController::getInputData()
    {
        if (modelIsFMU())
        {
            Model::OMVisualizerFMU* omVisFMU = (Model::OMVisualizerFMU*) _omVisualizer;
            return omVisFMU->getInputData();
        }
        else
            return nullptr;
    }

    bool GUIController::modelIsLoaded() const
    {
        return _modelLoaded;
    }

    void GUIController::setModelLoaded(bool b)
    {
        _modelLoaded = b;
    }

}  // End namespace Control
