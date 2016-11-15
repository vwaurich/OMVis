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


#ifndef INCLUDE_VISUALIZATIONTYPES_HPP_
#define INCLUDE_VISUALIZATIONTYPES_HPP_

namespace OMVIS
{
    namespace Model
    {

        /*! This enum class holds the possible visualization modes of OMVIS. */
        enum class VisType
        {
            NONE = 0,
            FMU = 1,
            FMU_REMOTE = 2,
            MAT = 3,
            MAT_REMOTE = 4
        };

    }  // namespace Model
}  // namespace OMVIS

#endif /* INCLUDE_VISUALIZATIONTYPES_HPP_ */
