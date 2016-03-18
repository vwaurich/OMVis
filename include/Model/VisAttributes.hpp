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

#ifndef INCLUDE_VISATTRIBUTES_HPP_
#define INCLUDE_VISATTRIBUTES_HPP_

#include <osg/Vec3f>
#include <osg/Matrix>
#include <osg/Uniform>
#include "WrapperFMILib.hpp"
#include "Util/rapidxml.hpp"
#include "Util/read_matlab4.h"

namespace Model
{

    /*! \brief Contains all information to display a shape.
     *
     *
     */
    class VisAttributes
    {
     public:
        VisAttributes();
        ~VisAttributes() = default;

        VisAttributes(const VisAttributes& va) = delete;
        VisAttributes& operator=(const VisAttributes& va) = default;

        /// Dumps the attributes.
        void dumpVisAttributes();

        /// Computes the attributes of the shape for a certain time.
        void fetchVisAttributes(rapidxml::xml_node<>* node, ModelicaMatReader matReader, fmi1_import_t* fmu, double time, bool useFMU);

     public:
        /// \todo Can these attributes be private?
        std::string _type;
        /// \todo Can these attributes be private?
        double _length;
        /// \todo Can these attributes be private?
        double _width;
        /// \todo Can these attributes be private?
        double _height;
        /// \todo Can these attributes be private?
        osg::Vec3f _r;
        /// \todo Can these attributes be private?
        osg::Vec3f _rShape;
        /// \todo Can these attributes be private?
        osg::Vec3f _lDir;
        /// \todo Can these attributes be private?
        osg::Vec3f _wDir;
        /// \todo Can these attributes be private?
        osg::Vec3f _color;
        /// \todo Can these attributes be private?
        /// The original T
        osg::Matrix3 _T;
        /// \todo Can these attributes be private?
        /// The computed(adapted to the lenght direction)
        osg::Matrix _mat;
    };

}  // End namespace Model

#endif /* INCLUDE_VISATTRIBUTES_HPP_ */
/**
 * @}
 */
