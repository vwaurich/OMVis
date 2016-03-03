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

/**
 *
 *
 * Purpose: Calculates the total of 6 checks
 *
 * @author Volker Waurich
 * @version 0.1
 *
 *
 * General todos:
 * \todo Pass arguments to functions by reference
 * \todo Const correctness
 * \todo Attributes private if possible
 * \todo Use forward declaration where possible
 * \todo Remove unused includes.
 */

/**
 *
 *
 * \todo What can be reused of the Parallel FMU program? Or, what classes and functions can
 * Parallel FMU Program and OMVIS share in the sense of unique file location?
 *
 */

#include <stdexcept>
//#include <memory>
#include <iostream>
#include "OMVIS.hpp"

int main(int argc, char* argv[])
{
    std::cout << "Hello" << std::endl;
    try
    {
        // Parse command line arguments
        Util::CommandLineArgs cLArgs = Util::getCommandLineArguments(argc, argv);
        // Init. logger
        Util::Logger::initialize(cLArgs._logSettings);

        std::string modelname = cLArgs._modelName;
        bool useFMU = cLArgs._useFMU;
        std::string path = cLArgs._modelPath;

        LOGGER_WRITE(std::string("Hei, modelname = ") + cLArgs._modelName + std::string(", path = ") + cLArgs._modelPath + " and useFMU = " + Util::boolToString(cLArgs._useFMU), Util::LC_OTHER, Util::LL_ERROR);

        // Render frames
        bool makeLoop = true;

        // Create visualization
        Initialization::Factory* factory = new Initialization::Factory();
        Model::OMVisualizerAbstract* omv = factory->createVisualizationFromCLargs(cLArgs);

        // Init visualization
        omv->initData();
        omv->setUpScene();
		omv->createVisAttributes();
		omv->updateVisAttributes(0.0);  // set scene to initial position 

        QApplication app(argc, argv);
        MainWidget* mainWidget = new MainWidget(0, Qt::Widget, osgViewer::CompositeViewer::SingleThreaded, omv);
        mainWidget->setGeometry(100, 100, 800, 600);
        mainWidget->show();
        app.exec();

        //while (!omv->_viewerStuff->_viewer.done())
//        while (false)
//        {
//            // render everything, even when tend is reached
//            omv->_viewerStuff->_viewer.frame();
//            // restart from the beginning after tend
//            if (omvManager._simTime > omvManager._endTime && makeLoop)
//            {
//                omvManager._timer.setStartTick();
//            }
//
//            // simulation and animation loop
//            while (omvManager._realTime < omvManager._endTime)
//            {
//                //update real time counter
//                omvManager.updateTick();
//                //cout<<"realTime "<<omvManager.realTime<<endl;
//
//                // In case of FMU, simulate FMU
//                omv->simulate(omvManager);
//
////                if (useFMU)
////                {
////                    while (omvManager._simTime < omvManager._realTime + omvManager._hVisual && omvManager._simTime < omvManager._endTime)
////                    {
////                        //<<"simulate "<<omvManager.simTime<<endl;
////                        //omv.fmuData.inputs.printValues();
////                        omvManager._simTime = omv->_fmuData->simulateStep(omvManager._simTime);
////                    }
////                }
//
//                //visualization step
//                if (omvManager._realTime >= omvManager._visTime)
//                {
//                    LOGGER_WRITE(std::string("Render ") + std::to_string(omvManager._visTime), Util::LC_VIEWER, Util::LL_INFO);
//                    // update all shapes
//                    rapidxml::xml_node<> * rootNode = omv->_baseData->_xmlDoc.first_node();
//                    unsigned int shapeIdx = 0;
//
//                    for (rapidxml::xml_node<> * shapeNode = rootNode->first_node("shape"); shapeNode; shapeNode = shapeNode->next_sibling())
//                    {
//                        // get all values for the attributes at this time
//                        omv->fetchVisAttributes(shapeNode, omvManager._visTime);
//                        //omv.resetInputs();
//                        //update the shapes
//                        updater._visAttr = omv->_baseData->_visAttr;
//                        //updater.visAttr.dumpVisAttributes();
//
//                        //get the scene graph nodes and stuff
//                        osg::ref_ptr<osg::Node> child = omv->_viewerStuff->_scene._rootNode->getChild(shapeIdx);  // the transformation
//                        child->accept(updater);
//                        shapeIdx++;
//                    }
//                    //update visualization time in omvManager
//                    omvManager._visTime = omvManager._visTime + omvManager._hVisual;
//                }
//                //render a new frame
//                omv->_viewerStuff->_viewer.frame();
//            }
//            if (makeLoop)
//                omvManager._timer.setStartTick();
//        }
//
    }
    catch (std::exception &ex)
    {
        LOGGER_WRITE(std::string("Execution failed. Error: ") + std::string(ex.what()), Util::LC_OTHER, Util::LL_ERROR);
        return -1;
    }
//
//
//    // Test of Logger Settings
//    // LOGGER_WRITE(std::string("Hei, LC_LOADER, LL_ERROR"),  Util::LC_LOADER, Util::LL_ERROR);
//    // LOGGER_WRITE(std::string("Hei, LC_LOADER, LL_WARNING"),  Util::LC_LOADER, Util::LL_WARNING);
//    // LOGGER_WRITE(std::string("Hei, LC_LOADER, LL_INFO"),  Util::LC_LOADER, Util::LL_INFO);
//    // LOGGER_WRITE(std::string("Hei, LC_LOADER, LL_DEBUG"),  Util::LC_LOADER, Util::LL_DEBUG);
//
//    // Create visualizer
//    OMVisualizer* omv = new OMVisualizer(useFMU, modelname, path);
//
//
//    omv->initData();
//    omv->linkInputsToEventHandler();
//    omv->setUpScene();
//
//    //the node traverser which dumps the tree
//    InfoVisitor infoVisitor;
//    omv->_viewerStuff->_scene._rootNode->accept(infoVisitor);
//
//    //render frames
//    bool makeLoop = true;
//    OMVManager omvManager;
//    omvManager._endTime = 10;
//    omvManager._hVisual = 0.1;
//    if (omv->_useFMU)
//    {
//        omv->_fmuData->_settings->set_tend(omvManager._endTime);
//        omv->_fmuData->_settings->set_hdef(0.001);
//    }
//    omvManager._timer.setStartTick();
//    UpdateVisitor updater;
//

    return 0;
}
