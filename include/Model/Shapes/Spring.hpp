/*
 * Copyright (C) 2016, Volker Waurich
 *
 * This file is part of OMVIS.
 *
 * OMVIS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OMVIS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OMVIS.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INCLUDE_MODEL_SHAPES_SPRING_HPP_
#define INCLUDE_MODEL_SHAPES_SPRING_HPP_

/** \addtogroup Model
 *  \{
 *  \copyright TU Dresden. All rights reserved.
 *  \authors Volker Waurich, Martin Flehmig
 *  \date Feb 2016
 */

#include <osg/Geometry>
#include <osg/Vec3f>
#include <osg/ref_ptr>

namespace OMVIS
{
    namespace Model
    {

        class Spring : public osg::Geometry
        {
         public:
            /*-----------------------------------------
             * CONSTRUCTORS
             *---------------------------------------*/

            Spring(const float r, const float rCoil, const float nWindings, const float l);

            ~Spring() = default;

         private:
            /*-----------------------------------------
             * MATH FUNCTIONS
             *---------------------------------------*/

            osg::Vec3f getNormal(const osg::Vec3f& vec, float length = 1);
            osg::Vec3f normalize(const osg::Vec3f& vec);
            osg::Vec3f rotateX(const osg::Vec3f& vec, float phi);
            osg::Vec3f rotateY(const osg::Vec3f& vec, float phi);
            osg::Vec3f rotateZ(const osg::Vec3f& vec, float phi);
            osg::Vec3f rotateArbitraryAxes_expensive(const osg::Vec3f& vec, const osg::Vec3f& axes, float phi);
            osg::Vec3f rotateArbitraryAxes(const osg::Vec3f& vec, const osg::Vec3f& axes, float phi);
            float absoluteVector(const osg::Vec3f& vec);
            float angleBetweenVectors(const osg::Vec3f& vec1, osg::Vec3f vec2);

            /*-----------------------------------------
             * MEMBERS
             *---------------------------------------*/

            osg::ref_ptr<osg::Vec3Array> _outerVertices;
            osg::ref_ptr<osg::Vec3Array> _splineVertices;
        };

    }  // namespace Model
}  // namespace OMVIS

#endif /* INCLUDE_MODEL_SHAPES_SPRING_HPP_ */
/**
 * \}
 */
