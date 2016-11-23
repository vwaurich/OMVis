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

#include "Model/Shapes/Pipecylinder.hpp"

#define _USE_MATH_DEFINES // for C++
#include <cmath>
#include <math.h>

#include <iostream>

namespace OMVIS
{
    namespace Model
    {

        Pipecylinder::Pipecylinder(const float rI, const float rO, const float l)
                : osg::Geometry()
        {
            const int nEdges = 20;
            double phi = 2 * M_PI / nEdges;

            //VERTICES
            osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array(4*nEdges);

            for (int i = 0; i < nEdges; ++i)
            {
                // inner base ring
                (*vertices)[i] = osg::Vec3(sin(phi * i) * rI, cos(phi * i) * rI, 0);

                // outer base ring
                (*vertices)[i+nEdges] = osg::Vec3(sin(phi * i) * rO, cos(phi * i) * rO, 0);

                // inner end ring
                (*vertices)[i+2*nEdges] = osg::Vec3(sin(phi * i) * rI, cos(phi * i) * rI, l);

                // outer end ring
                (*vertices)[i+3*nEdges] = osg::Vec3(sin(phi * i) * rO, cos(phi * i) * rO, l);
            }
            this->setVertexArray(vertices);

            //PLANES
            // base plane bottom
            osg::ref_ptr<osg::DrawElementsUInt> basePlane = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 4);
            (*basePlane)[0] = 0;
            (*basePlane)[1] = nEdges - 1;
            (*basePlane)[2] = (2 * nEdges - 1);
            (*basePlane)[3] = (nEdges);
            this->addPrimitiveSet(basePlane);

            for (int i = 0; i < (nEdges - 1); ++i)
            {
                basePlane = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 4);
                (*basePlane)[0] = i;
                (*basePlane)[1] = i + 1;
                (*basePlane)[2] = nEdges + 1 + i;
                (*basePlane)[3] = nEdges + i;
                this->addPrimitiveSet(basePlane);
            }

            // base plane top
            basePlane = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 4);
            (*basePlane)[0] = 2 * nEdges;
            (*basePlane)[1] = nEdges - 1 + 2 * nEdges;
            (*basePlane)[2] = 2 * nEdges - 1 + 2 * nEdges;
            (*basePlane)[3] = nEdges + 2 * nEdges;
            this->addPrimitiveSet(basePlane);

            for (int i = (2 * nEdges); i < (nEdges - 1 + (2 * nEdges)); ++i)
            {
                basePlane = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 4);
                (*basePlane)[0] = i;
                (*basePlane)[1] = i + 1;
                (*basePlane)[2] = nEdges +1 + i;
                (*basePlane)[3] = nEdges + i;
                this->addPrimitiveSet(basePlane);
            }

            //inner lateral planes
            basePlane = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 4);
            (*basePlane)[0] = 0;
            (*basePlane)[1] = nEdges - 1;
            (*basePlane)[2] = 3 * nEdges - 1;
            (*basePlane)[3] = 2 * nEdges;
            this->addPrimitiveSet(basePlane);

            for (int i = 0; i < (nEdges - 1); ++i)
            {
                basePlane = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 4);
                (*basePlane)[0] = i;
                (*basePlane)[1] = i + 1;
                (*basePlane)[2] = i + 1 + 2 * nEdges;
                (*basePlane)[3] = i + 2 * nEdges;
                this->addPrimitiveSet(basePlane);
            }

            //outer lateral planes
            basePlane = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 4);
            (*basePlane)[0] = nEdges;
            (*basePlane)[1] = 2 * nEdges - 1;
            (*basePlane)[2] = 4 * nEdges - 1;
            (*basePlane)[3] = 3 * nEdges;
            this->addPrimitiveSet(basePlane);

            //outer lateral planes
            for (int i = nEdges; i < (2 * nEdges - 1); ++i)
            {
                basePlane = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 4);
                (*basePlane)[0] = i;
                (*basePlane)[1] = i + 1;
                (*basePlane)[2] = i + 1 + 2 * nEdges;
                (*basePlane)[3] = i + 2 * nEdges;
                this->addPrimitiveSet(basePlane);
            }
        }

    }  // namespace Model
}  // namespace OMVIS
