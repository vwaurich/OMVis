/*
 * GUIController.cpp
 *
 *  Created on: 01.03.2016
 *      Author: mf
 */

#include <string>
#include <sys/stat.h>
#include "Model/OMVisualizerAbstract.hpp"
#include "Initialization/Factory.hpp"
#include "Util/Logger.hpp"
#include "Controller/GUIController.hpp"
#include "Util/Util.hpp"

namespace Controller
{

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

        return omv;
    }

    bool GUIController::checkForXMLFile(const std::string& path, const std::string& modelName)
    {
        struct stat buffer;
        std::string file = path + modelName + "_visual.xml";
        return (stat(file.c_str(), &buffer) == 0);
    }

}  // End namespace Controller
