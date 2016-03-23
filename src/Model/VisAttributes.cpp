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

#include "Model/VisAttributes.hpp"
#include "Visualize.hpp"

namespace Model
{

    VisAttributes::VisAttributes()
            : _type("box"),
              _length(1.0),
              _width(0.1),
              _height(0.1),
              _r(osg::Vec3f(0.0, 0.0, 0.0)),
              _rShape(osg::Vec3f(0.0, 0.0, 0.0)),
              _lDir(osg::Vec3f(1.0, 0.0, 0.0)),
              _wDir(osg::Vec3f(0.0, 1.0, 0.0)),
              _color(osg::Vec3f(1.0, 1.0, 1.0)),
              _T(osg::Matrix3(0, 0, 1, 1, 0, 0, 0, 1, 0)),
              _mat(osg::Matrix(0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1.0))
    {
    }

    void VisAttributes::dumpVisAttributes() const
    {
        std::cout << "type " << _type << std::endl;
        std::cout << "length " << _length << std::endl;
        std::cout << "width " << _width << std::endl;
        std::cout << "height " << _height << std::endl;
        std::cout << "lDir " << _lDir[0] << ", " << _lDir[1] << ", " << _lDir[2] << ", " << std::endl;
        std::cout << "wDir " << _wDir[0] << ", " << _wDir[1] << ", " << _wDir[2] << ", " << std::endl;
        std::cout << "r " << _r[0] << ", " << _r[1] << ", " << _r[2] << ", " << std::endl;
        std::cout << "r_shape " << _rShape[0] << ", " << _rShape[1] << ", " << _rShape[2] << ", " << std::endl;
        std::cout << "lDir0 " << _lDir[0] << ", " << _lDir[1] << ", " << _lDir[2] << ", " << std::endl;
        std::cout << "wDir0 " << _wDir[0] << ", " << _wDir[1] << ", " << _wDir[2] << ", " << std::endl;
        std::cout << "T0 " << _T[0] << ", " << _T[1] << ", " << _T[2] << ", " << std::endl;
        std::cout << "   " << _T[3] << ", " << _T[4] << ", " << _T[5] << ", " << std::endl;
        std::cout << "   " << _T[6] << ", " << _T[7] << ", " << _T[8] << ", " << std::endl;
        std::cout << "mat " << _mat(0, 0) << ", " << _mat(0, 1) << ", " << _mat(0, 2) << ", " << _mat(0, 3) << ", " << std::endl;
        std::cout << "    " << _mat(1, 0) << ", " << _mat(1, 1) << ", " << _mat(1, 2) << ", " << _mat(1, 3) << ", " << std::endl;
        std::cout << "    " << _mat(2, 0) << ", " << _mat(2, 1) << ", " << _mat(2, 2) << ", " << _mat(2, 3) << ", " << std::endl;
        std::cout << "    " << _mat(3, 0) << ", " << _mat(3, 1) << ", " << _mat(3, 2) << ", " << _mat(3, 3) << ", " << std::endl;
        std::cout << "color " << _color[0] << ", " << _color[1] << ", " << _color[2] << ", " << std::endl;
    }

}  // End namespace Model
