/*
 * OMVisualizerAbstract.cpp
 *
 *  Created on: 01.03.2016
 *      Author: mf
 */

#include "Model/OMVisualizerAbstract.hpp"

namespace Model
{

    OMVisualizerAbstract::OMVisualizerAbstract()
            : _baseData(nullptr),
              _viewerStuff(nullptr),
              nodeUpdater(nullptr),
              omvManager(nullptr)
    {
    }

    OMVisualizerAbstract::OMVisualizerAbstract(const std::string modelName, const std::string dir)
            : _baseData(new OMVisualBase(modelName, dir)),
              _viewerStuff(new View::OMVisualViewer),
              nodeUpdater(new Model::UpdateVisitor),
              omvManager(new View::OMVManager(0.0, 0.0, 0.0, 0.1, 0.0, 10.0))
    {
        _viewerStuff->_scene.path = dir;
    }

    void OMVisualizerAbstract::setUpScene()
    {
        //build scene graph
        _viewerStuff->_scene.setUpScene(_baseData->_xmlDoc.first_node());
    }

    void OMVisualizerAbstract::startVisualization()
    {
        if (omvManager->_visTime < omvManager->_endTime - 1.e-6)
        {
            omvManager->pause = false;
            std::cout << "start visualization" << std::endl;
        }
        else
            std::cout << "There is nothing left to visualize. Initialize first." << std::endl;
    }

    void OMVisualizerAbstract::pauseVisualization()
    {
        std::cout << "pause visualization" << std::endl;
        omvManager->pause = true;

    }

    void OMVisualizerAbstract::initVisualization()
    {
        std::cout << "init visualization" << std::endl;
        initializeVisAttributes(omvManager->_startTime);
        omvManager->_visTime = omvManager->_startTime;
        omvManager->pause = true;
    }

    void OMVisualizerAbstract::donationVisualization()
    {
        std::cout << "Want to support Martin and Volker? Buy us a coffee." << std::endl;
    }

    void OMVisualizerAbstract::sceneUpdate()
    {
        if (!omvManager->pause)
        {
            updateScene(omvManager->_visTime);
            omvManager->_visTime = omvManager->_visTime + omvManager->_hVisual;
            std::cout << "update scene at " << (omvManager->_visTime) << " simTime " << (omvManager->_simTime) << " _visStepSize " << (omvManager->_hVisual) << std::endl;
            if (omvManager->_visTime >= omvManager->_endTime - 1.e-6)
            {
                std::cout << "PASUE!!!!!!!!!!!!!!" << std::endl;
                omvManager->pause = true;
            }
        }
    }

}  // End namespace Model
