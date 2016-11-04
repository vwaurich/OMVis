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

#ifndef INCLUDE_SHAPEOBJECTATTRIBUTE_HPP_
#define INCLUDE_SHAPEOBJECTATTRIBUTE_HPP_

#include "WrapperFMILib.hpp"

#include <string>

namespace OMVIS
{
    namespace Model
    {

        /*! \brief This class encapsulates the attributes for objects of class \ref ShapeObject.
         *
         * \todo Todo: Make members private.
         */
        class ShapeObjectAttribute
        {
         public:
            /*-----------------------------------------
             * CONSTRUCTORS
             *---------------------------------------*/

            ShapeObjectAttribute();

            ShapeObjectAttribute(const float value);

            ~ShapeObjectAttribute() = default;

            /*-----------------------------------------
             * GETTERS AND SETTERS
             *---------------------------------------*/

            //outputs a string representation for the attribute
            std::string getValueString() const;

         public:
            /*-----------------------------------------
             * MEMBERS
             *---------------------------------------*/

            //if true, check the exp, if wrong check the cref
            bool isConst;
            float exp;
            std::string cref; 			///< Only for MAT and CSV (future)
            fmi1_value_reference_t fmuValueRef; ///< For (all) FMI versions
        };

    }  // End namespace Util
}  // End namespace OMVIS

#endif // INCLUDE_SHAPEOBJECTATTRIBUTE_HPP_
