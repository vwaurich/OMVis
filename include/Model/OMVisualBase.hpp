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

#ifndef INCLUDE_OMVISUALBASE_HPP_
#define INCLUDE_OMVISUALBASE_HPP_

#include "Visualize.hpp"
#include "Model/ShapeObject.hpp"

#include <rapidxml.hpp>
#include <string>

namespace OMVIS
{
    namespace Model
    {

        /*! \brief Base class that encapsulates the information given in the XML file.
         *
         *
         */
        class OMVisualBase
        {
         public:
            /*-----------------------------------------
             * CONSTRUCTORS
             *---------------------------------------*/

            OMVisualBase() = delete;

            /*! \brief Constructs OMVisualBase object from arguments.
             *
             * \remark The path needs to be absolute.
             * \remark The model file and its corresponding visual XML file need to be in the same directory.
             *
             * @param[in] modelFile Name of the model file.
             * @param[in] path Absolute path to the FMU or result file and corresponding XML-file.
             */
            OMVisualBase(const std::string& modelFile, const std::string& path);

            ~OMVisualBase() = default;
            OMVisualBase(const OMVisualBase& omvb) = delete;
            OMVisualBase& operator=(const OMVisualBase& omvb) = delete;

            /*-----------------------------------------
             * INITIALIZATION METHODS
             *---------------------------------------*/

            /*! \brief Reads XML file and sets up osg::viewer.
             *
             * Returns 0 if every thing went fine.
             * \return Error value.
             */
            int initXMLDoc();

            void clearXMLDoc();

            /*! \brief Get all visual objects from the XML file.
             *
             * \return Error value.
             */
            int initVisObjects();

            /*-----------------------------------------
             * GETTERS and SETTERS
             *---------------------------------------*/

			/*! \brief Get all variable names which are needed for the visualization.
			*
			* \return Vector of strings containing the variable names.
			*/
			std::vector<std::string> getVisualizationVariables();

            /*! \brief Returns name of the model. */
            const std::string getModelName() const;

            /*! \brief Returns directory path of the model file and the visual XML file. */
            const std::string getPath() const;

            /*! \brief Returns pointer to first node of the XML document. */
            rapidxml::xml_node<>* getFirstXMLNode();

            /*! \brief Returns name of the XML document. */
            const std::string getXMLFileName() const;

         private:
            /*-----------------------------------------
             * MEMBERS
             *---------------------------------------*/

            /*! Name of the model file without path but with prefix, e.g., modelFoo.fmu . */
            std::string _modelFile;
            /*! Absolute path to the model file, e.g., /home/user/models/ . */
            std::string _path;
            /*! The XML file containing the information about the visualization. */
            rapidxml::xml_document<> _xmlDoc;

         public:
            /// Stores all visualization objects.
            std::vector<ShapeObject> _shapes;

         private:
            /// Name (incl. path) of the XML file which holds necessary information for visualization.
            std::string _xmlFileName;
        };

    }  // End namespace Model
}  // End namespace OMVIS

#endif /* INCLUDE_OMVISUALBASE_HPP_ */
