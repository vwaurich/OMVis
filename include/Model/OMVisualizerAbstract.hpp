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
 * OMVisualizerBase.hpp
 *
 *  Created on: 17.02.2016
 *      Author: mf
 */

#ifndef INCLUDE_OMVISUALIZERABSTRACT_HPP_
#define INCLUDE_OMVISUALIZERABSTRACT_HPP_

namespace View
{
    class OMVManager;
}

#include <memory>
#include "View/OMVisualViewer.hpp"
#include "Model/OMVisualBase.hpp"
#include "Model/UpdateVisitor.hpp"
#include "View/OMVManager.hpp"

namespace Model
{

    /*! \brief This class serves as abstract class for visualization.
     *
     * It provides basic methods for visualization.
     * Concrete implementations are \ref OMVisualizerMat and \ref OMVisualizerFMU.
     */
    class OMVisualizerAbstract
    {
     public:
        /// The default constructor is forbidden.
        OMVisualizerAbstract()  //= delete;
                : _baseData(nullptr),
                  _viewerStuff(nullptr),
                  nodeUpdater(nullptr),
                  omvManager(nullptr)
        {
        }

        /*! \brief Constructs OMVisualizer object from arguments.
         *
         * @param model Name of the model.
         * @param dir Path to the FMU or result file and corresponding XML-file.
         */
        OMVisualizerAbstract(std::string modelName, std::string dir)
                : _baseData(new OMVisualBase(modelName, dir)),
                  _viewerStuff(new View::OMVisualViewer),
                  nodeUpdater(new Model::UpdateVisitor),
                  omvManager(new View::OMVManager(0.0, 0.0, 0.0, 0.1, 0.0, 10.0))
        {
            _viewerStuff->_scene.path = dir;
        }

        /// Destructs OMVisualizer object.
        virtual ~OMVisualizerAbstract() = default;

        /// The copy constructor is forbidden.
        OMVisualizerAbstract(const OMVisualizerAbstract& omv) = delete;

        /// The assignment operator is forbidden.
        OMVisualizerAbstract& operator=(const OMVisualizerAbstract& omv) = delete;

        /*! \brief Initializes OMVisualizer object.
         *
         * XML-file is parsed and the values of the attributes are set.
         * The viewer is initialized and the scene is set up.
         * FMU is loaded if used, or the Matfile is read.
         * @return 1
         *
         * \todo Quick and dirty hack by passing OMVmanager, \see OMVisualizerFMU.
         *
         */
        virtual void initData()
        {
            // init xml file and get visAttributes
            _baseData->initXMLDoc();
        }

        void setUpScene()
        {
            //build scene graph
            _viewerStuff->_scene.setUpScene(_baseData->_xmlDoc.first_node());
        }

        /*! \brief In case of FMU visualization, this methods performs a simulation step.
         *
         * \remark All classes that derive from OMVisualizerAbstract
         * @param omvm
         */
        virtual void simulate(View::OMVManager& omvm) = 0;

        /*! \brief Virtual Method to update the scene. Is implemented either by usign FMU or mat-file.
         *
         * \remark All classes that derive from OMVisualizerAbstract
         * @param omvm
         */
        virtual void updateVisAttributes(double time) = 0;

        /*! \brief Virtual Method to initialize the scene. Is implemented either by usign FMU or mat-file.
         *
         * \remark All classes that derive from OMVisualizerAbstract
         */
        virtual void initializeVisAttributes(double time) = 0;

        /*! \brief Virtual Prepares everything to make the correct visualization attributes available for that time step (i.e. simulate the fmu)
         *
         * \remark All classes that derive from OMVisualizerAbstract
         */
        virtual void updateScene(double time) = 0;

		/*! \brief Returns a 0 if we use mat-failes, 1 if we use fmus.
		*/
		virtual int getDataTypeID() = 0;

        /*! \brief Starts the visualization.
         */
        void startVisualization()
        {
            if (omvManager->_visTime < omvManager->_endTime - 1.e-6)
            {
                omvManager->pause = false;
                std::cout << "start visualization" << std::endl;
            }
            else
                std::cout << "There is nothing left to visualize. Initialize first." << std::endl;
        }

        /*! \brief Pauses the visualization.
         */
        void pauseVisualization()
        {
            std::cout << "pause visualization" << std::endl;
            omvManager->pause = true;

        }

        /*! \brief Sets the scene to start position.
         */
        void initVisualization()
        {
            std::cout << "init visualization" << std::endl;
            initializeVisAttributes(omvManager->_startTime);
            omvManager->_visTime = omvManager->_startTime;
            omvManager->pause = true;
        }

        /*! \brief Prints a message which tells you to buy us a coffee.
         */
        void donationVisualization()
        {
            std::cout << "Want to support Martin and Volker? Buy us a coffee." << std::endl;
        }

        /*! \brief Calls for a scene update.
         */
        void sceneUpdate()
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


     public:
        /// \todo: Can this attr. be private?
        OMVisualBase* _baseData;
        /// \todo: Can this attr. be private?
        View::OMVisualViewer* _viewerStuff;
        UpdateVisitor* nodeUpdater;
        View::OMVManager* omvManager;
    };

}  // End namespace Model

#endif /* INCLUDE_OMVISUALIZERABSTRACT_HPP_ */
