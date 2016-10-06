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

#include "Model/Shapes/Pipecylinder.hpp"

#define _USE_MATH_DEFINES // for C++
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

            std::cout << "rI " << rI << "  rO " << rO << std::endl;

            //VERTICES
            osg::Vec3Array* vertices = new osg::Vec3Array;
            // inner base ring
            for (int i = 0; i < nEdges; i++)
            {
                vertices->push_back(osg::Vec3(sin(phi * i) * rI, cos(phi * i) * rI, 0));
            }
            // outer base ring
            for (int i = 0; i < nEdges; i++)
            {
                vertices->push_back(osg::Vec3(sin(phi * i) * rO, cos(phi * i) * rO, 0));
            }
            // inner end ring
            for (int i = 0; i < nEdges; i++)
            {
                vertices->push_back(osg::Vec3(sin(phi * i) * rI, cos(phi * i) * rI, l));
            }
            // outer end ring
            for (int i = 0; i < nEdges; i++)
            {
                vertices->push_back(osg::Vec3(sin(phi * i) * rO, cos(phi * i) * rO, l));
            }
            this->setVertexArray(vertices);

            //PLANES
            // base plane bottom
            osg::DrawElementsUInt* basePlane = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
            basePlane->push_back(0);
            basePlane->push_back(nEdges - 1);
            basePlane->push_back(2 * nEdges - 1);
            basePlane->push_back(nEdges);
            this->addPrimitiveSet(basePlane);

            for (int i = 0; i < (nEdges - 1); i++)
            {
                basePlane = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
                basePlane->push_back(0 + i);
                basePlane->push_back(1 + i);
                basePlane->push_back(nEdges + 1 + i);
                basePlane->push_back(nEdges + 0 + i);
                this->addPrimitiveSet(basePlane);
            }

            // base plane top
            basePlane = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
            basePlane->push_back(0 + 2 * nEdges);
            basePlane->push_back(nEdges - 1 + 2 * nEdges);
            basePlane->push_back(2 * nEdges - 1 + 2 * nEdges);
            basePlane->push_back(nEdges + 2 * nEdges);
            this->addPrimitiveSet(basePlane);

            for (int i = (2 * nEdges); i < (nEdges - 1 + (2 * nEdges)); i++)
            {
                basePlane = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
                basePlane->push_back(0 + i);
                basePlane->push_back(1 + i);
                basePlane->push_back(nEdges + 1 + i);
                basePlane->push_back(nEdges + 0 + i);
                this->addPrimitiveSet(basePlane);
            }

            //inner lateral planes
            basePlane = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
            basePlane->push_back(0);
            basePlane->push_back(nEdges - 1);
            basePlane->push_back(3 * nEdges - 1);
            basePlane->push_back(2 * nEdges);
            this->addPrimitiveSet(basePlane);

            for (int i = 0; i < (nEdges - 1); i++)
            {
                basePlane = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
                basePlane->push_back(i);
                basePlane->push_back(i + 1);
                basePlane->push_back(i + 1 + 2 * nEdges);
                basePlane->push_back(i + 2 * nEdges);
                this->addPrimitiveSet(basePlane);
            }
            //outer lateral planes
            basePlane = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
            basePlane->push_back(nEdges);
            basePlane->push_back(2 * nEdges - 1);
            basePlane->push_back(4 * nEdges - 1);
            basePlane->push_back(3 * nEdges);
            this->addPrimitiveSet(basePlane);

            //outer lateral planes
            for (int i = nEdges; i < (2 * nEdges - 1); i++)
            {
                basePlane = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
                basePlane->push_back(i);
                basePlane->push_back(i + 1);
                basePlane->push_back(i + 1 + 2 * nEdges);
                basePlane->push_back(i + 2 * nEdges);
                this->addPrimitiveSet(basePlane);
            }
        }

    }  // End namespace Model
}  // End namespace OMVIS
