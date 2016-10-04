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

#include "Model/ShapeObjectAttribute.hpp"

#include <read_matlab4.h>
#include <rapidxml.hpp>

#include <osg/Vec3f>
#include <osg/Matrix>
#include <osg/Uniform>


namespace OMVIS
{
    namespace Model
    {

        /*! \brief A \a ShapeObject contains all information to display a shape.
         *
         *
         */
        class ShapeObject
        {
         public:
            /*-----------------------------------------
             * CONSTRUCTORS
             *---------------------------------------*/

            ShapeObject();

            ~ShapeObject() = default;

            ShapeObject(const ShapeObject&) = default;

            ShapeObject& operator=(const ShapeObject&) = default;

            /*-----------------------------------------
             * METHODS
             *---------------------------------------*/

            /// Dumps the attributes to std::cout.
            void dumpVisAttributes() const;

         public:
            /*-----------------------------------------
             * MEMBERS
             *---------------------------------------*/

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

            /*! 4x4 */
            osg::Matrix _mat;

			ShapeObjectAttribute _extra;
        };

    } // End namespace Model
} // End namespace OMVIS

#endif /* INCLUDE_SHAPEOBJECT_HPP_ */
