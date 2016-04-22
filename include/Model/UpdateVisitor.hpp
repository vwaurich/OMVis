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

#ifndef INCLUDE_UPDATEVISITOR_HPP_
#define INCLUDE_UPDATEVISITOR_HPP_

#include "VisAttributes.hpp"

#include <osg/NodeVisitor>
#include <osg/Geode>
#include <osg/MatrixTransform>

namespace OMVIS
{
    namespace Model
    {

        /*! \brief  Upatevisitor to update the osg tree.
         */
        class UpdateVisitor : public osg::NodeVisitor
        {
         public:
            UpdateVisitor();
            virtual ~UpdateVisitor() = default;

            UpdateVisitor(const UpdateVisitor& uv) = delete;
            UpdateVisitor& operator=(const UpdateVisitor& uv) = delete;

            virtual void apply(osg::Geode& node);
            virtual void apply(osg::MatrixTransform& node);

         public:
            /// \todo Can this attr. be private
            VisAttributes _visAttr;
        };

    }  // End namespace Model
}  // End namespace OMVIS

#endif /* INCLUDE_UPDATEVISITOR_HPP_ */
/**
 * @}
 */
