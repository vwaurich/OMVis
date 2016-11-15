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

/** \addtogroup Model
 *  \{
 *  \copyright TU Dresden. All rights reserved.
 *  \authors Volker Waurich, Martin Flehmig
 *  \date Feb 2016
 */

#ifndef INCLUDE_PIPECYLINDER_HPP
#define INCLUDE_PIPECYLINDER_HPP

#include <osg/Geometry>

namespace OMVIS
{
    namespace Model
    {

        class Pipecylinder : public osg::Geometry
        {
         public:
            /*-----------------------------------------
             * CONSTRUCTORS
             *---------------------------------------*/

            Pipecylinder(const float rI, const float rO, const float l);

            ~Pipecylinder() = default;

        };

    }  // namespace Model
}  // namespace OMVIS

#endif /* INCLUDE_PIPECYLINDER_HPP */
/**
 * \}
 */
