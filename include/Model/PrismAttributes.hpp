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

#ifndef INCLUDE_PRISMATTRIBUTES_HPP_
#define INCLUDE_PRISMATTRIBUTES_HPP_

#define _USE_MATH_DEFINES  //we need pi as a constant

#include "WrapperFMILib.hpp"
#include "Util/rapidxml.hpp"
#include "Util/read_matlab4.h"
#include "VisAttributes.hpp"
#include <math.h>"

namespace Model
{

    /*! \brief Contains all information to display a shape.
     *
     *
     */
    class PrismAttributes : public VisAttributes
    {
     public:
		 PrismAttributes();
        ~PrismAttributes() = default;

		PrismAttributes(const PrismAttributes& va) = delete;
		PrismAttributes& operator=(const PrismAttributes& va) = default;

        /// Dumps the attributes.
        void dumpVisAttributes();

		/*! \brief update the elements with FMU variables
		*/
		void updateVisAttributesFMU(rapidxml::xml_node<>* node, double time, fmi1_import_t* fmu);

		/*! \brief update the elements with mat-file variables
		*/
		void updateVisAttributesMAT(rapidxml::xml_node<>* node, double time, ModelicaMatReader matReader);

		/*! \brief Gets the transformation matrix
		*/
		osg::Matrix getTransformationMatrix();

		/*! \brief gets the type of the shape
		*/
		std::string getType();

		/*! \brief get the color of the shape
		*/
		osg::Vec3f getColor();

		/*! \brief Outputs true if tis a Shape Visualization Object
		*/
		bool isShapeVisualizer();

		/*! \brief Outputs true if its a Prism Visualization Object
		*/
		bool isPrismVisualizer();

		/*! \brief Updates the osg geode type and attributes
		*/
		void updateGeode(osg::Geode* node);

		/*! \brief Sets the Material for the stateset of a node
		*/
		void setMaterial(osg::ref_ptr<osg::StateSet> stateSet);


     public:
        /// \todo Can these attributes be private?
        std::string _type;
        /// \todo Can these attributes be private?
        double _length;
        /// \todo Can these attributes be private?
		double _radius;
		/// \todo Can these attributes be private?
		int _numEdges;
        /// \todo Can these attributes be private?
		osg::Vec3f _r;
		/// \todo Can these attributes be private?
        osg::Vec3f _rShape;
        /// \todo Can these attributes be private?
        osg::Vec3f _lDir;
        /// \todo Can these attributes be private?
        osg::Vec3f _color0;
		osg::Vec3f _color1;
        /// \todo Can these attributes be private?
        /// The original T
        osg::Matrix3 _T;
        /// \todo Can these attributes be private?
        /// The computed(adapted to the lenght direction)
        osg::Matrix _mat;
    };



}  // End namespace Model



#endif /* INCLUDE_PRISMATTRIBUTES_HPP_ */
