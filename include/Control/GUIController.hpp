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
#include "View/OMVisScene.hpp"
#include "Model/InputData.hpp"

#include <osg/Vec4>

#include <memory>

// Forward declarations
namespace OMVIS
{
    namespace Model
    {
        class OMVisualizerAbstract;
    }
}

namespace OMVIS
{
    namespace Control
    {

        /*! \brief This class is a controller class which handles user interaction with OMVis via GUI.
         *
         * The GUIcontroller receives the user input (,e.g., clicking a menu point), process it and induces
         * the appropriate action.
         */
        class GUIController
        {
         public:
            /*-----------------------------------------
             * CONSTRUCTORS
             *---------------------------------------*/

            GUIController();
            ~GUIController() = default;

            GUIController(const GUIController& gc) = delete;
            GUIController& operator=(const GUIController& gc) = delete;

            /*-----------------------------------------
             * INITIALIZATION METHODS
             *---------------------------------------*/

            /*! \brief This method loads a model (FMU or MAT file) for visualization.
             *
             * This method parses the given string for model name, path to the model file and whether it is a FMU or a
             * MAT file and asks the factory for creation of an appropriate OMVisualizer object. Furthermore, the
             * OMVisualizer object is initialized.
             * The method checks, if the corresponding XML file is accessible.
             * If a model is already loaded into OMVis and the new model cannot be loaded, for instance due to a
             * compatibility issue, than the old model is kept. If the already load and the new model are the very same,
             * no information/message is "thrown". We just load it and destroy the current settings.
             *
             * \param modelName Path and name of the model to load.
             * \param timeSliderStart Minimum value of the time slider object.
             * \param timeSliderEnd Maximum value of the time slider object.
             * \return Pointer to OMVisualizer object which is created by the factory.
             *
             * \remark We assume that the XML file is located in the very same directory as the FMU or MAT file.
             */
            void loadModel(const std::string& modelName, const int timeSliderStart, const int timeSliderEnd);
            void loadModel(const Initialization::VisualizationConstructionPlan& cP, const int timeSliderStart, const int timeSliderEnd);

            /*! \brief
             *
             * \remark Checks for XML description file. For remote visualization this file needs to be on the
             *         localhost. At the moment, the user is responsible for that. I.e., it might be necessary to copy
             *         this file explicitly from the remote server to the localhost.
             */
            void loadModel(const Initialization::RemoteVisualizationConstructionPlan& cP, const int timeSliderStart, const int timeSliderEnd);

            /*! \brief Unloads the currently loaded model and frees associated memory. */
            void unloadModel();

            /*! \brief Check if the XML file for a given path and model name can be accessed.
             *
             * The XML file name is created by "path + modelName + "_visual.xml"". Thus, the XML file has to be renamed
             * if it does not fit this convention.
             *
             * @param path Path to the XML file.
             * @param modelName Name of the model.
             * @return True, if the XML file is accessible. False, otherwise.
             */
            bool checkForXMLFile(const std::string& modelName, const std::string& path) const;

            /*-----------------------------------------
             * SIMULATION METHODS
             *---------------------------------------*/

            void startVisualization();
            void pauseVisualization();
            void initVisualization();
            void donationVisualization();

            void sceneUpdate();

            /*-----------------------------------------
             * GETTERS AND SETTERS
             *---------------------------------------*/

            int getTimeProgress();
            osg::ref_ptr<osg::Node> getSceneRootNode();

            void setVisTime(const int val);
            double getVisTime();

            double getRealTimeFactor();

            /*! \brief Returns true, if the currently loaded model is a MAT result file simulation. */
            bool modelIsMATFile();
            /*! \brief Returns true, if the currently loaded model is a FMU simulation. */
            bool modelIsFMU();
            /*! \brief Returns true, if the currently loaded model is a FMU in remote visualization mode. */
            bool modelIsFMUClient();

            /*! \brief Returns true, if a model is loaded into OMVis, i.e., the OMVisualizer object is not nullptr. */
            bool modelIsLoaded();

            /*! \brief Returns simulation start time of the loaded model. */
            double getSimulationStartTime() const;

            /*! \brief Gets the visualization step size in milliseconds from the omvManager
             * @return The visualization step size in milliseconds
             */
            double getVisStepsize();

            /*! \brief Gets the InputData from the OMVisualizer.
             *
             * If we visualize a MAT file, we return a proper nullptr.
             * @return The input data object pointer
             */
            std::shared_ptr<Model::InputData> getInputData();

         private:
            /*-----------------------------------------
             * MEMBERS
             *---------------------------------------*/

            /*! \brief Pointer to the OMVisualizer object, which handles the model visualization.
             *
             * OMVis provides three concrete implementations of OMVisualizerAbstract class:
             *  1. OMVisualizerMat for visualization of simulation results present in MAT file format.
             *  2. OMVisualizerFMU for FMU simulations.
             *  3. OMVisualizerFMUClient for FMU simulations whereas the FMU is computed on a remote system (server).
             *
             */
            std::shared_ptr<Model::OMVisualizerAbstract> _omVisualizer;
        };

    }  // End namespace Control
}  // End namespace OMVIS

#endif /* INCLUDE_GUICONTROLLER_HPP_ */
/**
 * @}
 */
