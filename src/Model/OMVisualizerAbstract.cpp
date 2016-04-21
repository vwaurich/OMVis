/*
 * OMVisualizerAbstract.cpp
 *
 *  Created on: 01.03.2016
 *      Author: mf
 */

#include "Model/OMVisualizerAbstract.hpp"
#include "Control/OMVisManager.hpp"
#include "Util/Logger.hpp"

#include <string>
#include <stdlib.h>

namespace Model
{

    OMVisualizerAbstract::OMVisualizerAbstract()
            : _baseData(nullptr),
              _viewerStuff(nullptr),
              _nodeUpdater(nullptr),
              _omvManager(nullptr)
    {
    }

    OMVisualizerAbstract::OMVisualizerAbstract(const std::string modelName, const std::string path)
            : //X2 _baseData(new OMVisualBase(modelName, path)),
              _viewerStuff(new View::OMVisScene()),
              _nodeUpdater(new Model::UpdateVisitor()),
              _omvManager(new Control::OMVisManager(0.0, 0.0, 0.0, 0.1, 0.0, 100.0))
    {
        // We need the absolute path to the directory. Otherwise the FMUlibrary can not open the shared objects.
        char fullPathTmp[PATH_MAX];
        realpath(path.c_str(), fullPathTmp);
        std::string fullPath = std::string(fullPathTmp) + "/";

        // Now we can use the full path.
        _baseData = new OMVisualBase(modelName, fullPath),
        _viewerStuff->getScene().setPath(fullPath);
    }

    int OMVisualizerAbstract::setUpScene()
    {
        //build scene graph
        int isOk = _viewerStuff->getScene().setUpScene(_baseData->_xmlDoc.first_node());
        return isOk;
    }

    void OMVisualizerAbstract::startVisualization()
    {
        if (_omvManager->getVisTime() < _omvManager->getEndTime() - 1.e-6)
        {
            _omvManager->setPause(false);
            LOGGER_WRITE(std::string("Start visualization."), Util::LC_CTR, Util::LL_INFO);
        }
        else
            LOGGER_WRITE(std::string("There is nothing left to visualize. Initialize the model first."), Util::LC_CTR, Util::LL_INFO);
    }

    void OMVisualizerAbstract::pauseVisualization()
    {
        LOGGER_WRITE(std::string("Pause visualization at ") + std::to_string(_omvManager->getVisTime()) + std::string("."), Util::LC_CTR, Util::LL_INFO);
        _omvManager->setPause(true);
    }

    void OMVisualizerAbstract::initVisualization()
    {
        LOGGER_WRITE(std::string("Initialize visualization."), Util::LC_CTR, Util::LL_INFO);
        initializeVisAttributes(_omvManager->getStartTime());
        _omvManager->setVisTime(_omvManager->getStartTime());
        _omvManager->setPause(true);
    }

    void OMVisualizerAbstract::donationVisualization()
    {
        LOGGER_WRITE(std::string("Want to support Martin and Volker? Buy us a coffee."), Util::LC_CTR, Util::LL_INFO);
    }

    void OMVisualizerAbstract::sceneUpdate()
    {
        if (!_omvManager->isPaused())
        {
            updateScene(_omvManager->getVisTime());
            _omvManager->setVisTime(_omvManager->getVisTime() + _omvManager->getHVisual());

            LOGGER_WRITE(std::string("Update scene at ") + std::to_string(_omvManager->getVisTime()) + std::string(" simTime ")
                + std::to_string(_omvManager->getSimTime()) + std::string(" _visStepSize ") + std::to_string(_omvManager->getHVisual())
                , Util::LC_CTR, Util::LL_INFO);
            if (_omvManager->getVisTime() >= _omvManager->getEndTime() - 1.e-6)
            {
                LOGGER_WRITE(std::string("The End."), Util::LC_CTR, Util::LL_INFO);
                _omvManager->setPause(true);
            }
        }
    }

    std::string OMVisualizerAbstract::getType() const
    {
        return "abstract";
    }

}  // End namespace Model
