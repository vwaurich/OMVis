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

#include "Util/Algebra.hpp"

#include <iostream>

namespace OMVIS
{
    namespace Util
    {

        osg::Vec3f Mat3mulV3(const osg::Matrix3& M, const osg::Vec3f& V)
        {
            return osg::Vec3f(M[0] * V[0] + M[1] * V[1] + M[2] * V[2], M[3] * V[0] + M[4] * V[1] + M[5] * V[2], M[6] * V[0] + M[7] * V[1] + M[8] * V[2]);
        }

        osg::Vec3f V3mulMat3(const osg::Vec3f& V, const osg::Matrix3& M)
        {
            return osg::Vec3f(M[0] * V[0] + M[3] * V[1] + M[6] * V[2], M[1] * V[0] + M[4] * V[1] + M[7] * V[2], M[2] * V[0] + M[5] * V[1] + M[8] * V[2]);
        }

        osg::Matrix3 Mat3mulMat3(const osg::Matrix3& M1, const osg::Matrix3& M2)
        {
            osg::Matrix3 M3;
            float val;
            for (int i = 0; i < 3; ++i)
            {
                for (int j = 0; j < 3; ++j)
                {
                    //cout<<" i and j "<<i<<" "<<j<<endl;
                    val = 0.0f;
                    for (int k = 0; k < 3; ++k)
                    {
                        //cout<<M1[i*3+k]<<" * "<<M2[k*3+j]<<" = "<<M1[i*3+k]*M2[k*3+j]<<endl;
                        val += M1[i * 3 + k] * M2[k * 3 + j];
                    }
                    M3[i * 3 + j] = val;
                }
            }

            return M3;
        }

    }  // End namespace Util
}  // End namespace OMVIS

/**
 * @}
 */
