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
 * OMVisualBase.hpp
 *
 *  Created on: 03.02.2016
 *      Author: mf
 */

#ifndef INCLUDE_OMVISUALBASE_HPP_
#define INCLUDE_OMVISUALBASE_HPP_

#include <string>

#include "VisAttributes.hpp"
#include "ShapeAttributes.hpp"
#include "PrismAttributes.hpp"

#include "Visualize.hpp"

#include "Util/rapidxml.hpp"
#include "Util/Logger.hpp"

#include "Initialization/VisAttributesFactory.hpp"

namespace Model
{

    /*! \brief Base class that contains XML information.
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

		const int initVisAttributes();

     public:
        /// \todo Can this attr. be private?
        std::string _modelName;
        /// \todo Can this attr. be private?
        std::string _dirName;
        /// \todo Can this attr. be private?
        /// The XML containing the information about the visualization.
        rapidxml::xml_document<> _xmlDoc;
        /// \todo Can this attr. be private?

        /// Stores the current visualization data.
		std::vector<VisAttributes*> allVisAttr;
		/// Number of all visualization objects
		int numShapes;
		/// Factory to create visAttributes
		Initialization::VisAttributesFactory* visAttrFactory;
	
     private:
        std::string _xmlFileName;
    };

}  // End namespace Model

#endif /* INCLUDE_OMVISUALBASE_HPP_ */
