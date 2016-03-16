/*
 * OMVisualizerAbstract.cpp
 *
 *  Created on: 01.03.2016
 *      Author: mf
 */

#include "Model/OMVisualizerAbstract.hpp"
#include "Control/OMVisManager.hpp"

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
              _viewerStuff(new View::OMVisualViewer),
              _nodeUpdater(new Model::UpdateVisitor),
              _omvManager(new Control::OMVisManager(0.0, 0.0, 0.0, 0.1, 0.0, 10.0))
    {
        _viewerStuff->_scene._path = dir;
    }

    void OMVisualizerAbstract::setUpScene()
    {
        //build scene graph
        _viewerStuff->_scene.setUpScene(_baseData->_xmlDoc.first_node());
    }

    void OMVisualizerAbstract::startVisualization()
    {
        if (_omvManager->_visTime < _omvManager->_endTime - 1.e-6)
        {
            _omvManager->_pause = false;
            std::cout << "start visualization" << std::endl;
        }
        else
            std::cout << "There is nothing left to visualize. Initialize first." << std::endl;
    }

    void OMVisualizerAbstract::pauseVisualization()
    {
        std::cout << "pause visualization" << std::endl;
        _omvManager->_pause = true;

    }

    void OMVisualizerAbstract::initVisualization()
    {
        std::cout << "init visualization" << std::endl;
        initializeVisAttributes(_omvManager->_startTime);
        _omvManager->_visTime = _omvManager->_startTime;
        _omvManager->_pause = true;
    }

    void OMVisualizerAbstract::donationVisualization()
    {
        std::cout << "Want to support Martin and Volker? Buy us a coffee." << std::endl;
    }

    void OMVisualizerAbstract::sceneUpdate()
    {
        if (!_omvManager->_pause)
        {
            updateScene(_omvManager->_visTime);
            _omvManager->_visTime = _omvManager->_visTime + _omvManager->_hVisual;
            std::cout << "update scene at " << (_omvManager->_visTime) << " simTime " << (_omvManager->_simTime) << " _visStepSize " << (_omvManager->_hVisual) << std::endl;
            if (_omvManager->_visTime >= _omvManager->_endTime - 1.e-6)
            {
                std::cout << "PASUE!!!!!!!!!!!!!!" << std::endl;
                _omvManager->_pause = true;
            }
        }
    }

}  // End namespace Model
