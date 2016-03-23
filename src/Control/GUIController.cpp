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

#include <string>
#include <sys/stat.h>
#include "Control/GUIController.hpp"
#include "Model/OMVisualizerAbstract.hpp"
#include "Model/OMVisualizerFMU.hpp"
#include "Initialization/Factory.hpp"
#include "Util/Logger.hpp"
#include "Util/Util.hpp"
#include "Control/OMVisManager.hpp"

namespace Control
{
    GUIController::GUIController()
            : _omVisualizer(nullptr)
    {
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

        // Ask the factory to create an appropriate OMVisualizer object.
        Initialization::Factory* factory = new Initialization::Factory();
        _omVisualizer = factory->createVisualizationObject(modelName, path, visFMU);
        _omVisualizer->_omvManager->setSliderRange(timeSliderStart, timeSliderEnd);

        // Initialize the OMVisualizer object.
        _omVisualizer->initData();
        _omVisualizer->setUpScene();
        _omVisualizer->updateVisAttributes(0.0);  // set scene to initial position
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
        return _omVisualizer->_viewerStuff->_scene._rootNode;
    }

    void GUIController::sceneUpdate()
    {
        _omVisualizer->sceneUpdate();
    }

    double GUIController::getVisTime()
    {
        return _omVisualizer->_omvManager->_visTime;
    }

    void GUIController::setVisTime(const int val)
    {
        _omVisualizer->_omvManager->_visTime = (_omVisualizer->_omvManager->_endTime - _omVisualizer->_omvManager->_startTime) * (float) (val / 100.0);
    }

    bool GUIController::modelIsMATFile()
    {
        return (_omVisualizer->getDataTypeID() == 0) ? true : false;
    }

    bool GUIController::modelIsFMU()
    {
        return (_omVisualizer->getDataTypeID() == 1) ? true : false;
    }

    double GUIController::getSimulationStartTime() const
    {
        return _omVisualizer->_omvManager->_startTime;
    }

	double GUIController::getVisStepsize()
	{
		return _omVisualizer->_omvManager->_hVisual * 1000.0;
	}

	Model::InputData* GUIController::getInputData()
	{
		if (modelIsFMU())
		{
			Model::OMVisualizerFMU* omVisFMU = (Model::OMVisualizerFMU*)_omVisualizer;
			return &omVisFMU->_inputData;
		}
		else
			return nullptr;
	}


}  // End namespace Control
