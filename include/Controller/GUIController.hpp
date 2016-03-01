/*
 * GUIController.hpp
 *
 *  Created on: 01.03.2016
 *      Author: mf
 */

#ifndef INCLUDE_CONTROLLER_GUICONTROLLER_HPP_
#define INCLUDE_CONTROLLER_GUICONTROLLER_HPP_

// Forward declaration
class OMVisualizerAbstract;


namespace Controller
{

    /*! \brief This class is a controller class which handles user interaction via GUI.
     *
     */
    class GUIController
    {
     public:
        GUIController() = default;
        ~GUIController() = default;

        GUIController(const GUIController& gc) = delete;
        GUIController& operator=(const GUIController& gc) = delete;

        /*! \brief This method gets the name and path of a model which should be loaded into OMVIS.
         *
         * This method parses the given string for model name, path to the model file and whether it is a FMU or a MAT file and
         * asks the factory for creation of an appropriate OMVisualizer object. Furthermore, the OMVisualizer object is initialized.
         * The method checks, if the corresponding XML file is present/available.
         * \return Pointer to OMVisualizer object which is created by the factory.
         * \remark We assume that the XML file is located in the very same directory as the FMU or MAT file.
         */
        Model::OMVisualizerAbstract* loadModel(const std::string& modelName);

        /// Check if the XML file for a given path and model name can be accessed.
        bool checkForXMLFile(const std::string& path, const std::string& modelName);

     private:
    };

}  // End namespace Controller

#endif /* INCLUDE_CONTROLLER_GUICONTROLLER_HPP_ */
