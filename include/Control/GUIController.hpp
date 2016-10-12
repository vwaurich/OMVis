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

/** @addtogroup Control
 *  @{
 *  \copyright TU Dresden. All rights reserved.
 *  \authors Volker Waurich, Martin Flehmig
 *  \date Feb 2016
 */

#ifndef INCLUDE_GUICONTROLLER_HPP_
#define INCLUDE_GUICONTROLLER_HPP_

#include "Initialization/VisualizationConstructionPlans.hpp"
#include "Model/VisualizerAbstract.hpp"
#include "Model/InputData.hpp"
#include "Model/SimSettings.hpp"

#include <osg/Node>

#include <memory>


namespace OMVIS
{
    namespace Control
    {

        /*! \brief This is a controller class that handles the user interaction with OMVis via GUI.
         *
         * Thus, this class serves as the(!) interface between the user and OMVis. A GUIcontroller object receives the
         * user input (,e.g., clicking a menu point), processes it and induces the appropriate action. Therefore, the
         * GuiController class implements most of the slot functions of \ref Model::OMVisVisViewer.
         *
         * The GuiController class encapsulates the model data structures from the GUI elements and classes. It holds
         * the pointer to the concrete model visualization object.
         *
         * \remark: If we want to visualize multiple models in the future, _modelVisualizer will have to be a vector of
         *          pointers of type \ref Model::VisualizerAbstract to these models.
         *
         */
        class GUIController
        {
         public:
            /*-----------------------------------------
             * CONSTRUCTORS
             *---------------------------------------*/

            /*! \brief Default constructor with save initialization of members. */
            GUIController();

            ~GUIController() = default;

            GUIController(const GUIController& gc) = delete;

            GUIController& operator=(const GUIController& gc) = delete;

            /*-----------------------------------------
             * INITIALIZATION METHODS
             *---------------------------------------*/

            /*! \brief This method loads a model (FMU or MAT file) for visualization.
             *
             * This method gets a construction plan and asks the \ref Initialization::Factory to create an
             * appropriate Visualizer object. If this Visualizer object can be initialized without errors, it is
             * taken as new visualization model.
             * If a model is already loaded into OMVis and the new model cannot be loaded, for instance due to a
             * compatibility issue, than the old model is kept. If the already load and the new model are the very
             * same, no information/message is thrown. We just load it and destroy the current settings.
             *
             * \remark We assume that the visual XML file is located in the very same directory as the FMU or MAT file.
             *         Otherwise, the user is responsible to copy the visual XML file to this directory.
             *
             * \param cP                Construction plan for visualization.
             * \param timeSliderStart   Minimum value of the time slider object.
             * \param timeSliderEnd     Maximum value of the time slider object.
             * \return Pointer to the Visualizer object which is created by the factory or to the old one.
             */
            void loadModel(const Initialization::VisualizationConstructionPlan& cP, const int timeSliderStart,
                           const int timeSliderEnd);

            /*! \brief This method loads a FMU model for remote visualization.
             *
             * This method gets a construction plan for remote visualization and asks the \ref Initialization::Factory
             * to create an appropriate VisualizerClient object. If this OMVisualizerClient object can be initialized
             * without errors, it is taken as new visualization model.
             * If a model is already loaded into OMVis and the new model cannot be loaded, for instance due to a
             * compatibility issue, than the old model is kept. If the already load and the new model are the very
             * same, no information/message is thrown. We just load it and destroy the current settings.
             *
             * \remark Checks for XML description file. For remote visualization this file needs to be on the
             *         localhost. At the moment, the user is responsible for that. I.e., it might be necessary to copy
             *         this file explicitly from the remote server to the localhost.
             *
             * \param cP                Construction plan for remote visualization.
             * \param timeSliderStart   Minimum value of the time slider object.
             * \param timeSliderEnd     Maximum value of the time slider object.
             * \return Pointer to the Visualizer object which is created by the factory or to the old one.
             */
            void loadModel(const Initialization::RemoteVisualizationConstructionPlan& cP, const int timeSliderStart,
                           const int timeSliderEnd);



            /*! \brief Unloads the currently loaded model and frees associated memory. */
            void unloadModel();

            /*-----------------------------------------
             * SIMULATION METHODS
             *---------------------------------------*/

            /*! \brief Asks the OMVisualizer object to start the simulation. */
            void startVisualization();

            /*! \brief Asks the OMVisualizer object to pause the simulation. */
            void pauseVisualization();

            /*! \brief Asks the OMVisualizer object to (re-)initialize the simulation.
             *
             * \todo: Check if input variables need to be specified in order to interact with the FMU. If so,
             *        open a GUI.
             */
            void initVisualization();

            /*! \brief Asks the OMVisualizer object to update the scene.
             *
             * This method is triggered by \ref View::OMVisViewer::sceneUpdate which is frequently called from Qt.
             */
            void sceneUpdate();

            /*-----------------------------------------
             * GETTERS AND SETTERS
             *---------------------------------------*/

            int getTimeProgress();

            /*! \brief Returns the root node of the scene.
             *
             * The visualization needs the root node of the scene to display the model.
             *
             * \return Pointer to scene root node.
             */
            osg::ref_ptr<osg::Node> getSceneRootNode() const;

            /*! \brief Sets the visualization time handled by the OMVisManager object.
             *
             * This method is called by \ref View::OMVisViewer if the user moves the time slider.
             *
             * \param val   The new value of the time slider.
             */
            void setVisTime(const int val);
            /*! \brief Returns the current visualization time. */
            double getVisTime();
            /*! \brief Returns the real time factor for the current visualization. */
            double getRealTimeFactor();
            /*! \brief Returns the simulation start time of the loaded model. */
            double getSimulationStartTime() const;
            /*! \brief Returns the visualization step size in milliseconds. */
            double getVisStepsize();

            bool visTypeIs(const Model::VisType visType) const;
            bool visTypeIsNone() const;
            bool visTypeIsFMU() const;
            bool visTypeIsFMURemote() const;
            bool visTypeIsMAT() const;
            bool visTypeIsMATRemote() const;

            /*! \brief Returns name of the model. */
            std::string getModelFile() const;

            /*! \brief Returns true, if a model is loaded into OMVis, i.e., the Visualizer object is not nullptr. */
            bool modelIsLoaded();

            /*! \brief Gets the InputData from the Visualizer object in case of FMU or remote FMU visualization.
             *
             * \remark If we visualize a MAT file, we return a proper nullptr and the caller method has to handle it!
             *
             * \return A pointer to the \ref Model::InputData object.
             */
            std::shared_ptr<Model::InputData> getInputData();

            /*! \brief Handles the simulation settings specified by the user via Simulation Settings dialogs.
             *
             * This method reinitializes the simulation in order to apply the settings.
             *
             * \param simSetFMU     The simulation settings for a FMU visualization
             */
            void handleSimulationSettings(const Model::UserSimSettingsFMU& simSetFMU);
            void handleSimulationSettings(const Model::UserSimSettingsMAT& simSetFMU);

         private:
            /*! \brief This is a helper method for the two \ref loadModel() methods. */
            void loadModelHelper(const Initialization::VisualizationConstructionPlan* cP, const int timeSliderStart,
                                 const int timeSliderEnd);

            /*-----------------------------------------
             * MEMBERS
             *---------------------------------------*/

            /*! \brief Pointer to the Visualizer object, which handles the model visualization.
             *
             * OMVis provides three concrete implementations of \ref Model::VisualizerAbstract class:
             *  1. \ref Model::VisualizerMat for visualization of simulation results present in MAT file format.
             *  2. \ref Model::VisualizerFMU for FMU simulations.
             *  3. \ref Model::VisualizerFMUClient for FMU simulations whereas the FMU is computed on a remote system.
             *
             */
            std::shared_ptr<Model::VisualizerAbstract> _modelVisualizer;
        };

    }  // End namespace Control
}  // End namespace OMVIS

#endif /* INCLUDE_GUICONTROLLER_HPP_ */
/**
 * @}
 */
