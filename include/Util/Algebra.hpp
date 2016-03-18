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

/** @addtogroup Util
 *  @{
 *  \copyright TU Dresden. All rights reserved.
 *  \authors Volker Waurich, Martin Flehmig
 *  \date Feb 2016
 */

#ifndef INCLUDE_ALGEBRA_HPP_
#define INCLUDE_ALGEBRA_HPP_

#include <osg/Geometry>

namespace Util
{

    /*! \brief Multiplication of 3x3 Matrix and 3-Vector.
     *
     @param
     @return
     */
    osg::Vec3f Mat3mulV3(osg::Matrix3 M, osg::Vec3f V);

    /*! \brief Multiplication of 3x3 Matrix and 3x3 Matrix.
     *
     @param
     @return
     */
    osg::Matrix3 Mat3mulMat3(osg::Matrix3 M1, osg::Matrix3 M2);

    /*! \brief Multiplication of 3-col-Vector and a 3x3 Matrix.
     *
     * @param
     @return
     */
    osg::Vec3f V3mulMat3(osg::Vec3f V, osg::Matrix3 M);

}  // End namespace Util

#endif /* INCLUDE_ALGEBRA_HPP_ */
/**
 * @}
 */
