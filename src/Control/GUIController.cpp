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

/*
 * GUIController.cpp
 *
 *  Created on: 01.03.2016
 *      Author: mf
 */

#include <string>
#include <sys/stat.h>
#include <Control/GUIController.hpp>
#include "Model/OMVisualizerAbstract.hpp"
#include "Initialization/Factory.hpp"
#include "Util/Logger.hpp"
#include "Util/Util.hpp"

namespace Control
{
//    GUIController::GUIController()
//            //: _viewer(new View::OMVisualViewer())
//    {
//    }

    Model::OMVisualizerAbstract* GUIController::loadModel(const std::string& modelNameIn)
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
        Model::OMVisualizerAbstract* omv = factory->createVisualizationObject(modelName, path, visFMU);

        // Initialize the OMVisualizer object.
        omv->initData();
        omv->setUpScene();
        omv->updateVisAttributes(0.0);  // set scene to initial position

        // Init viewer \todo: Create viewer::init(path, xmldoc) function
//        _viewer->_scene._path = path;
//        // build scene graph. omv::_baseData::_xmlDoc has to be initialized at this point.
//        _viewer->_scene.setUpScene(omv->_baseData->_xmlDoc.first_node());

//        omv->updateVisAttributes(0.0);  // set scene to initial position

        return omv;
    }

    bool GUIController::checkForXMLFile(const std::string& path, const std::string& modelName)
    {
        struct stat buffer;
        std::string file = path + modelName + "_visual.xml";
        return (stat(file.c_str(), &buffer) == 0);
    }

}  // End namespace Control
