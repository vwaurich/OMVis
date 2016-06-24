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
#include "ShapeObjectAttribute.hpp"

#include <read_matlab4.h>
#include <rapidxml.hpp>

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
            ShapeObjectAttribute _length;
            ShapeObjectAttribute _width;
            ShapeObjectAttribute _height;
            ShapeObjectAttribute _r[3];
            ShapeObjectAttribute _rShape[3];
            ShapeObjectAttribute _lDir[3];
            ShapeObjectAttribute _wDir[3];
            ShapeObjectAttribute _color[3];
            ShapeObjectAttribute _T[9];
            ShapeObjectAttribute _specCoeff;

            osg::Matrix _mat;
        };

    } // End namespace Model;
} // End namespace OMVIS;

#endif /* INCLUDE_SHAPEOBJECT_HPP_ */
