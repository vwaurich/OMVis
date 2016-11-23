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

#include "Model/ShapeObjectAttribute.hpp"
#include "Util/Util.hpp"

namespace OMVIS
{
    namespace Model
    {

        ShapeObjectAttribute::ShapeObjectAttribute()
                : isConst(true),
                  exp(0.0),
                  cref("NONE"),
                  fmuValueRef(0)
        {
        }

        ShapeObjectAttribute::ShapeObjectAttribute(double value)
                : isConst(true),
                  exp((float)value),
                  cref("NONE"),
                  fmuValueRef(0)
        {
        }

        std::string ShapeObjectAttribute::getValueString() const
        {
            return std::to_string(exp) + " (" + std::to_string(fmuValueRef) + ") Is const: "
                    + Util::boolToString(isConst);
//                    std::to_string(static_cast<int>(isConst)) + " ";
        }

    }  // namespace Model
}  // namespace OMVIS
