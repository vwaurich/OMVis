/*
 * Copyright (C) 2016, Volker Waurich
 *
 * This file is part of OMVIS.
 *
 * OMVIS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OMVIS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OMVIS.  If not, see <http://www.gnu.org/licenses/>.
 */

/** \addtogroup Model
 *  \{
 *  \copyright TU Dresden. All rights reserved.
 *  \authors Volker Waurich, Martin Flehmig
 *  \date Feb 2016
 */

#ifndef INCLUDE_VISUALIZERABSTRACT_HPP_
#define INCLUDE_VISUALIZERABSTRACT_HPP_

#include <Model/OMVISScene.hpp>
#include <Model/VisualBase.hpp>
#include "Control/TimeManager.hpp"
#include "Model/UpdateVisitor.hpp"
#include "Model/VisualizationTypes.hpp"
#include "Model/SimSettings.hpp"
#include "Util/Visualize.hpp"
#include "ShapeObjectAttribute.hpp"

#include <memory>

namespace OMVIS
{
    namespace Model
    {

        /*! \brief This class serves as abstract basis for data encapsulation of the visualization model.
         *
         * It provides basic methods for visualization.
         *
         * Currently, there are three concrete implementations:
         *      - \ref VisualizerFMU for visualization of models encapsulated as FMU
         *      - \ref VisualizerMAT for visualization of simulation result files in MAT format
         *      - \ref VisualizerFMUClient for remote visualization of models encapsulated as FMU
         *
         * The VisualizerAbstract class holds a shared pointer to the Control::TimeManager object. Moreover, the
         * VisualizerAbstract constructs this TimeManager object and sets the default end time to 100.
         *
         */
        class VisualizerAbstract
        {
         public:
            /*-----------------------------------------
             * CONSTRUCTORS
             *---------------------------------------*/

            /// The default constructor.
            VisualizerAbstract();

            /*! \brief Constructs a VisualizerAbstract object from arguments.
             *
             * \remark The model file and its corresponding visual XML file need to be in the same directory.
             *
             * \param modelFile  Name of the model file.
             * \param path       Path to the FMU or result file and corresponding XML file.
             * \param visType    The visualization type.
             */
            VisualizerAbstract(const std::string& modelFile, const std::string& path, const VisType visType =
                                       VisType::NONE);

            virtual ~VisualizerAbstract() = default;

            VisualizerAbstract(const VisualizerAbstract& rhs) = delete;

            VisualizerAbstract& operator=(const VisualizerAbstract& rhs) = delete;

            /*-----------------------------------------
             * INITIALIZATION METHODS
             *---------------------------------------*/

            /*! \brief This methods initializes a Visualizer object.
             *
             * Encapsulates the three stages/methods of initialization process into one single method.
             */
            virtual void initialize();

            /*! \brief Sets the scene to start position.
             */
            void initVisualization();

            /*-----------------------------------------
             * GETTERS and SETTERS
             *---------------------------------------*/

            /*! \brief Returns VisType::NONE. */
            VisType getVisType() const;

            std::shared_ptr<VisualBase> getBaseData() const;

            std::shared_ptr<Control::TimeManager> getTimeManager() const;

            std::shared_ptr<OMVISScene> getOMVISScene() const;

            std::string getModelFile() const;

            /*-----------------------------------------
             * SIMULATION METHODS
             *---------------------------------------*/

            /*! \brief In case of FMU visualization, this methods performs a simulation step.
             *
             * \remark All classes that derive from VisualizerAbstract
             * \param omvm
             */
            virtual void simulate(Control::TimeManager& omvm) = 0;

            /*! \brief Starts the visualization.
             *
             * If the simulation end time is not yet reached, the TimeManager object is set  to "unpause". Thus, the
             * simulation is continued.
             */
            virtual void startVisualization();

            /*! \brief Pauses the visualization.
             *
             * The TimeManager is set to "pause" and the simulation stops.
             */
            virtual void pauseVisualization();

            /*! \brief Calls for a scene update. */
            void sceneUpdate();

         protected:
            /*-----------------------------------------
             * MEMBERS
             *---------------------------------------*/

            /** \brief Every Visualizer object knows its type, i.e., the visualization mode. */
            const VisType _visType;
            std::shared_ptr<VisualBase> _baseData;
            std::shared_ptr<OMVISScene> _viewerStuff;
            std::shared_ptr<UpdateVisitor> _nodeUpdater;
            std::shared_ptr<Control::TimeManager> _timeManager;

            /*-----------------------------------------
             * PROTECTED METHODS
             *---------------------------------------*/

            /*-----------------------------------------
             * INITIALIZATION METHODS
             *---------------------------------------*/

            /*! \brief Initializes the VisualBase object.
             *
             * The visual XML file is parsed and the values of the attributes are set.
             * The viewer is initialized and the scene is set up.
             * The FMU is loaded if used, or the MAT file is read.
             *
             * This method calls \ref VisualBase::clearXMLDoc, \ref VisualBase::initXMLDoc and
             * \ref VisualBase::initVisObjects. Each of this functions throws a std::runtime_error in case of failure.
             */
            virtual void initData();

            /*! \brief Sets up the scene. */
            void setUpScene();

            /*! \brief Initializes the scene.
             *
             * This method is implemented either by using FMU or MAT file.
             *
             * \remark All classes that derive from VisualizerAbstract
             */
            virtual void initializeVisAttributes(const double time) = 0;

            /*-----------------------------------------
             * SIMULATION METHODS
             *---------------------------------------*/

            /*! \brief Updates the visualization attributes after a time step has been performed.
             *
             * This method is pure virtual and needs to be implemented by derived classes, e.g., \ref VisualizerFMU
             * and \ref VisualizerMAT.
             *
             * \param time  The visualization time.
             */
            virtual void updateVisAttributes(const double time) = 0;

            /*! \brief Prepares everything to make the correct visualization attributes available for that time step (i.e. simulate the FMU).
             *
             * \remark All classes that derive from VisualizerAbstract
             */
            virtual void updateScene(const double time) = 0;

        };

    }  // namespace Model
}  // namespace OMVIS

#endif /* INCLUDE_VISUALIZERABSTRACT_HPP_ */
/**
 * \}
 */
