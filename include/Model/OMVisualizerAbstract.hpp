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

/** @addtogroup Model
 *  @{
 *  \copyright TU Dresden. All rights reserved.
 *  \authors Volker Waurich, Martin Flehmig
 *  \date Feb 2016
 */

#ifndef INCLUDE_OMVISUALIZERABSTRACT_HPP_
#define INCLUDE_OMVISUALIZERABSTRACT_HPP_

namespace Control
{
    class OMVisManager;
}

#include <memory>
#include "View/OMVisScene.hpp"
#include "Model/OMVisualBase.hpp"
#include "Model/UpdateVisitor.hpp"

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
        /// The default constructor.
        OMVisualizerAbstract();

        /*! \brief Constructs OMVisualizer object from arguments.
         *
         * @param model Name of the model.
         * @param dir Path to the FMU or result file and corresponding XML-file.
         */
        OMVisualizerAbstract(const std::string modelName, const std::string dir);

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

        void setUpScene();

        /*! \brief In case of FMU visualization, this methods performs a simulation step.
         *
         * \remark All classes that derive from OMVisualizerAbstract
         * @param omvm
         */
        virtual void simulate(Control::OMVisManager& omvm) { };

        /*! \brief Virtual Method to update the scene. Is implemented either by using FMU or mat-file.
         *
         * \remark All classes that derive from OMVisualizerAbstract
         * @param omvm
         */
        virtual void updateVisAttributes(const double time) { };

        /*! \brief Virtual Method to initialize the scene. Is implemented either by using FMU or mat-file.
         *
         * \remark All classes that derive from OMVisualizerAbstract
         */
        virtual void initializeVisAttributes(const double time) { };

        /*! \brief Virtual Prepares everything to make the correct visualization attributes available for that time step (i.e. simulate the fmu)
         *
         * \remark All classes that derive from OMVisualizerAbstract
         */
        virtual void updateScene(const double time) { };

		/*! \brief Returns a 0 if we use mat-failes, 1 if we use fmus.
		*/
		virtual int getDataTypeID() = 0;

        /*! \brief Starts the visualization.
         */
        void startVisualization();

        /*! \brief Pauses the visualization.
         */
        void pauseVisualization();
        /*! \brief Sets the scene to start position.
         */
        void initVisualization();

        /*! \brief Prints a message which tells you to buy us a coffee.
         */
        void donationVisualization();

        /*! \brief Calls for a scene update.
         */
        void sceneUpdate();

     public:
        /// \todo: Can this attr. be private?
        OMVisualBase* _baseData;
        /// \todo: Can this attr. be private?
        View::OMVisScene* _viewerStuff;
        UpdateVisitor* _nodeUpdater;
        Control::OMVisManager* _omvManager;
    };

}  // End namespace Model

#endif /* INCLUDE_OMVISUALIZERABSTRACT_HPP_ */
/**
 * @}
 */
