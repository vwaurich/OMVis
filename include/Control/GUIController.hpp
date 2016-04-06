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

#include "View/OMVisScene.hpp"
#include "Model/InputData.hpp"

#include <osg/Vec4>

// Forward declarations
namespace Model
{
    class OMVisualizerAbstract;
}


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
        GUIController();
        ~GUIController() = default;

        GUIController(const GUIController& gc) = delete;
        GUIController& operator=(const GUIController& gc) = delete;

        //X13
        void unloadModel();

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

        /*! \brief Check if the XML file for a given path and model name can be accessed.
         *
         * The XML file name is created by "path + modelName + "_visual.xml"". Thus, the XML file has to be renamed
         * if it does not fit this convention.
         *
         * @param path Path to the XML file.
         * @param modelName Name of the model.
         * @return True, if the XML file is accessible. False, otherwise.
         */
        bool checkForXMLFile(const std::string& path, const std::string& modelName) const;

        void startVisualization();
        void pauseVisualization();
        void initVisualization();
        void donationVisualization();

        int getTimeProgress();
        osg::ref_ptr<osg::Node> getSceneRootNode();
        void sceneUpdate();
        double getVisTime();
        void setVisTime(const int val);
        bool modelIsMATFile();
        bool modelIsFMU();
        bool modelIsLoaded() const;
        void setModelLoaded(bool b);

        void setBackgroundColor(const osg::Vec4 colVec);

        /// Returns simulation start time of the loaded model.
        double getSimulationStartTime() const;

        /*! \brief Gets the visualization step size in milliseconds from the omvManager
         * @return The visualization step size in milliseconds
         */
		double getVisStepsize();

        /*! \brief Gets the InputData from the omVisualizer.
         *
         * If we visualize a MAT file, we return a proper nullptr.
         * @return The input data object pointer
         */
		Model::InputData* getInputData();

     private:
        Model::OMVisualizerAbstract* _omVisualizer;

        //X13
        /** This member is true, if a model is currently loaded and initialized. Otherwise it is false.
         * It can be used to determine, what user actions are allowed in the GUI. */
        bool _modelLoaded;
    };

}  // End namespace Control

#endif /* INCLUDE_GUICONTROLLER_HPP_ */
/**
 * @}
 */
