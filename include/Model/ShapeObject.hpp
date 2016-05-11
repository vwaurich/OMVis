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


#ifndef INCLUDE_SHAPEOBJECT_HPP_
#define INCLUDE_SHAPEOBJECT_HPP_

#include "WrapperFMILib.hpp"
#include <read_matlab4.h>
#include <rapidxml.hpp>
#include "Util/ObjectAttribute.hpp"

#include <osg/Vec3f>
#include <osg/Matrix>
#include <osg/Uniform>

#include <string>

namespace OMVIS
{
	namespace Model
	{
		/*! \brief Contains all information to display a shape.
		*
		*
		*/
		class ShapeObject
		{
		public:
			ShapeObject();
			~ShapeObject() = default;

			ShapeObject& operator=(const ShapeObject& va) = default;

			/// Dumps the attributes to std::cout.
			void dumpVisAttributes() const;

			/// Computes the attributes of the shape for a certain time.
			void fetchVisAttributes(rapidxml::xml_node<>* node, ModelicaMatReader matReader, fmi1_import_t* fmu, double time, bool useFMU);

		public:
			std::string _id;
			std::string _type;
			ObjectAttribute _length;
			ObjectAttribute _width;
			ObjectAttribute _height;
			ObjectAttribute _r[3];
			ObjectAttribute _rShape[3];
			ObjectAttribute _lDir[3];
			ObjectAttribute _wDir[3];
			ObjectAttribute _color[3];
			ObjectAttribute _T[9];
			ObjectAttribute	_specCoeff;

			osg::Matrix _mat;
		};

	}; //namespace Model;
}; //namespace OMVVIS;
#endif
