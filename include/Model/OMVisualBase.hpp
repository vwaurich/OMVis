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

#include <string>
#include "VisAttributes.hpp"
#include "Util/rapidxml.hpp"

namespace Model
{

    /*! \brief Base class that encapsulates the information given in the XML file.
     *
     *
     */
    class OMVisualBase
    {
     public:
        OMVisualBase() = delete;
        OMVisualBase(std::string model, std::string dir);
        ~OMVisualBase() = default;
        OMVisualBase(const OMVisualBase& omvb) = delete;
        OMVisualBase& operator=(const OMVisualBase& omvb) = delete;

        /// Reads XML file and sets up osg::viewer.
        void initXMLDoc();

     public:
        /// \todo Can this attr. be private?
        std::string _modelName;
        /// \todo Can this attr. be private?
        std::string _dirName;
        /// \todo Can this attr. be private?
        /// The XML file containing the information about the visualization.
        rapidxml::xml_document<> _xmlDoc;
        /// \todo Can this attr. be private?
        /// Stores the current visualization data.
        VisAttributes _visAttr;

     private:
        std::string _xmlFileName;
    };

}  // End namespace Model

#endif /* INCLUDE_OMVISUALBASE_HPP_ */
