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

#include "Model/ShapeObject.hpp"
#include "Util/Visualize.hpp"

#include <iostream>

namespace OMVIS
{
    namespace Model
    {

        /*-----------------------------------------
         * CONSTRUCTORS
         *---------------------------------------*/

        ShapeObject::ShapeObject()
                : _id("noID"),
                  _type("box"),
                  _fileName("noFile"),
                  _length(0.1),
                  _width(0.1),
                  _height(0.1),
                  _r { 0.1, 0.1, 0.1 },
                  _rShape { 0.0, 0.0, 0.0 },
                  _lDir { 1.0, 0.0, 0.0 },
                  _wDir { 0.0, 1.0, 0.0 },
                  _color { 255.0, 255.0, 255.0 },
                  _T { 0.0, 0.0, 1.0,
                       1.0, 0.0, 0.0,
                       0.0, 1.0, 0.0 },
                  _specCoeff(0.7),
                  _mat(0.0, 0.0, 0.0, 0.0,
                       0.0, 0.0, 0.0, 0.0,
                       0.0, 0.0, 0.0, 0.0,
                       0.0, 0.0, 0.0, 0.0),
                  _extra(0.0)
        {
        }

        /*-----------------------------------------
         * PRINT METHODS
         *---------------------------------------*/

        void ShapeObject::dumpVisAttributes() const
        {
            std::cout << "Dumping VisAttributes ...\n";
            std::cout << "  id: \t\t" << _id
                      << "\n  type: \t" << _type
                      << "\n  fileName: \t" << _fileName
                      << "\n  length: \t" << _length.getValueString()
                      << "\n  width: \t" << _width.getValueString()
                      << "\n  height: \t" << _height.getValueString()
                      << "\n  lDir: \t" << '[' << _lDir[0].getValueString() << ", " << _lDir[1].getValueString() << ", "
                                   << _lDir[2].getValueString()  << ']'
                      << "\n  wDir: \t" << '[' << _wDir[0].getValueString() << ", " << _wDir[1].getValueString() << ", "
                                   << _wDir[2].getValueString()  << ']'
                      << "\n  r: \t\t" << '[' <<_r[0].getValueString() << ", " << _r[1].getValueString() << ", "
                                   << _r[2].getValueString()  << ']'
                      << "\n  r_shape: \t" <<  '[' <<_rShape[0].getValueString() << ", " << _rShape[1].getValueString() << ", "
                                         << _rShape[2].getValueString() << ']'
                      << "\n  T0: \t\t" << '[' << _T[0].getValueString() << ", " << _T[1].getValueString() << ", "
                                 << _T[2].getValueString() << ", "
                      << "\n \t\t " << _T[3].getValueString() << ", " << _T[4].getValueString() << ", "
                               << _T[5].getValueString() << ", "
                      << "\n \t\t " << _T[6].getValueString() << ", " << _T[7].getValueString() << ", "
                               << _T[8].getValueString() << ']'
                      << "\n  color: \t" << '[' << _color[0].getValueString() << ", " << _color[1].getValueString() << ", "
                                    << _color[2].getValueString() << ']'
                      << "\n  mat: \t\t" << '[' << _mat(0, 0) << ", " << _mat(0, 1) << ", " << _mat(0, 2) << ", " << _mat(0, 3)
                      << "    " << _mat(1, 0) << ", " << _mat(1, 1) << ", " << _mat(1, 2) << ", " << _mat(1, 3)
                      << "    " << _mat(2, 0) << ", " << _mat(2, 1) << ", " << _mat(2, 2) << ", " << _mat(2, 3)
                      << "    " << _mat(3, 0) << ", " << _mat(3, 1) << ", " << _mat(3, 2) << ", " << _mat(3, 3) << ']'
                      << "\n  extra: \t" << _extra.getValueString() << std::endl;
        }

    }  // namespace Model
}  // namespace OMVIS
