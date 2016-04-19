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

namespace Model
{

    OMVisualizerAbstract::OMVisualizerAbstract()
            : _baseData(nullptr),
              _viewerStuff(nullptr),
              _nodeUpdater(nullptr),
              _omvManager(nullptr)
    {
    }

    OMVisualizerAbstract::OMVisualizerAbstract(const std::string modelName, const std::string dir)
            : _baseData(new OMVisualBase(modelName, dir)),
              _viewerStuff(new View::OMVisScene()),
              _nodeUpdater(new Model::UpdateVisitor()),
              _omvManager(new Control::OMVisManager(0.0, 0.0, 0.0, 0.1, 0.0, 100.0))
    {
        _viewerStuff->getScene().setPath(dir);
    }

    int OMVisualizerAbstract::setUpScene()
    {
        //build scene graph
        int isOk = _viewerStuff->getScene().setUpScene(_baseData->_xmlDoc.first_node());
        return isOk;
    }

    void OMVisualizerAbstract::startVisualization()
    {
        if (_omvManager->_visTime < _omvManager->_endTime - 1.e-6)
        {
            _omvManager->_pause = false;
            LOGGER_WRITE(std::string("Start visualization."), Util::LC_CTR, Util::LL_INFO);
        }
        else
            LOGGER_WRITE(std::string("There is nothing left to visualize. Initialize the model first."), Util::LC_CTR, Util::LL_INFO);
    }

    void OMVisualizerAbstract::pauseVisualization()
    {
        LOGGER_WRITE(std::string("Pause visualization at ") + std::to_string(_omvManager->_visTime) + std::string("."), Util::LC_CTR, Util::LL_INFO);
        _omvManager->_pause = true;
    }

    void OMVisualizerAbstract::initVisualization()
    {
        LOGGER_WRITE(std::string("Initialize visualization."), Util::LC_CTR, Util::LL_INFO);
        initializeVisAttributes(_omvManager->_startTime);
        _omvManager->_visTime = _omvManager->_startTime;
        _omvManager->_pause = true;
    }

    void OMVisualizerAbstract::donationVisualization()
    {
        LOGGER_WRITE(std::string("Want to support Martin and Volker? Buy us a coffee."), Util::LC_CTR, Util::LL_INFO);
    }

    void OMVisualizerAbstract::sceneUpdate()
    {
        if (!_omvManager->_pause)
        {
            updateScene(_omvManager->_visTime);
            _omvManager->_visTime = _omvManager->_visTime + _omvManager->_hVisual;

            LOGGER_WRITE(std::string("Update scene at ") + std::to_string(_omvManager->_visTime) + std::string(" simTime ")
                + std::to_string(_omvManager->_simTime) + std::string(" _visStepSize ") + std::to_string(_omvManager->_hVisual)
                , Util::LC_CTR, Util::LL_INFO);
            if (_omvManager->_visTime >= _omvManager->_endTime - 1.e-6)
            {
                LOGGER_WRITE(std::string("The End."), Util::LC_CTR, Util::LL_INFO);
                _omvManager->_pause = true;
            }
        }
    }

    std::string OMVisualizerAbstract::getType() const
    {
        return "abstract";
    }

}  // End namespace Model
