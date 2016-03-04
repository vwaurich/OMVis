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
 * VisAttributes.hpp
 *
 *  Created on: 03.02.2016
 *      Author: mf
 */

#ifndef INCLUDE_VISATTRIBUTES_HPP_
#define INCLUDE_VISATTRIBUTES_HPP_

#include <osg/Vec3f>
#include <osg/Matrix>
#include <osg/Drawable>
#include <osg/Uniform>
#include "WrapperFMILib.hpp"
#include "Util/rapidxml.hpp"
#include "Util/read_matlab4.h"

namespace Model
{
    /*! \brief Abstract class for a visualization object
     */
    class VisAttributes
    {
     public:
		 VisAttributes() {};
        ~VisAttributes() = default;

        VisAttributes(const VisAttributes& va) = delete;
        VisAttributes& operator=(const VisAttributes& va) = default;

        /// Dumps the attributes.
        virtual void dumpVisAttributes() = 0;

		/*! \brief update the elements with FMU variables
		*/
		virtual void updateVisAttributesFMU(rapidxml::xml_node<>* node, double time , fmi1_import_t* fmu) = 0;

		/*! \brief update the elements with mat-file variables
		*/
		virtual void updateVisAttributesMAT(rapidxml::xml_node<>* node, double time, ModelicaMatReader matReader) = 0;

		/*! \brief Gets the transformation matrix
		*/
		virtual osg::Matrix getTransformationMatrix() = 0;

		/*! \brief gets the type of the shape
		*/
		virtual std::string getType() = 0;

		/*! \brief Outputs true if its a Shape Visualization Object
		*/
		virtual bool isShapeVisualizer() = 0;

		/*! \brief Updates the osg geode type and attributes
		*/
		virtual void updateGeode(osg::Geode* node) = 0;
		/*! \brief Sets the material (color as well) for the stateset of a node
		*/
		virtual void setMaterial(osg::ref_ptr<osg::StateSet> stateSet) = 0;

    };

}  // End namespace Model

#endif /* INCLUDE_VISATTRIBUTES_HPP_ */
