/*
 * Copyright (C) 2016, Volker Waurich, Martin Flehmig
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

#include "Model/Shapes/Spring.hpp"

#define _USE_MATH_DEFINES // for C++
#include <math.h>

namespace OMVIS
{
    namespace Model
    {

        /*-----------------------------------------
         * CONSTRUCTORS
         *---------------------------------------*/

        Spring::Spring(const float r, const float rCoil, const float nWindings, const float l)
                : osg::Geometry(),
                  _outerVertices(nullptr),
                  _splineVertices(nullptr)
        {
            const int ELEMENTS_WINDING = 10;
            const int ELEMENTS_CONTOUR = 6;

            //the inner line points
            int numSegments = (ELEMENTS_WINDING * nWindings) + 1;
            _splineVertices = new osg::Vec3Array(numSegments);

            double c1 = 2.0 * M_PI / (double) ELEMENTS_WINDING;
            double c2 = l / numSegments;
            float x, y, z;
            for (int segIdx = 0; segIdx < numSegments; ++segIdx)
            {
                x = std::sin(c1 * segIdx) * r;
                y = std::cos(c1 * segIdx) * r;
                z = c2 * segIdx;
                (*_splineVertices)[segIdx].set(osg::Vec3(x, y, z));
            }

            //the outer points for the facets
            int numVertices = (numSegments + 1) * ELEMENTS_CONTOUR;
            _outerVertices = new osg::Vec3Array(numVertices);
            osg::Vec3f normal;
            osg::Vec3f v1;
            osg::Vec3f v2;
            int vertIdx = 0;
            osg::Vec3f vec0, a1;
            float angle;
            float c3 = M_PI * 2 / ELEMENTS_CONTOUR;
            for (int i = 0; i < numSegments - 1; ++i)
            {
                v1 = (*_splineVertices)[i];
                v2 = (*_splineVertices)[i + 1];
                normal = osg::Vec3f(v2[0] - v1[0], v2[1] - v1[1], v2[2] - v1[2]);
                vec0 = normal;
                normal = getNormal(normal, rCoil);
                for (int i1 = 0; i1 < ELEMENTS_CONTOUR; ++i1)
                {
                    angle = c3 * i1;
                    a1 = rotateArbitraryAxes(normal, vec0, angle);
                    (*_outerVertices)[vertIdx].set(osg::Vec3f((v1[0] + a1[0]), (v1[1] + a1[1]), (v1[2] + a1[2])));
                    ++vertIdx;
                }
            }

            // pass the created vertex array to the points geometry object.
            this->setVertexArray(_outerVertices);

            //PLANES
            // base plane bottom
            osg::DrawElementsUInt* basePlane;  // = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
            int numFacettes = ELEMENTS_CONTOUR * (numSegments - 2);
            for (int i = 0; i < numFacettes; ++i)
            {
                basePlane = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 4);
                (*basePlane)[0] = i;
                (*basePlane)[1] = i + 1;
                (*basePlane)[2] = i + ELEMENTS_CONTOUR;
                (*basePlane)[3] = i + ELEMENTS_CONTOUR - 1;

                this->addPrimitiveSet(basePlane);
            }
            //std::cout << "NUM " << outerVertices->size() << std::endl;
            //this->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, outerVertices->size()));
        }


        /*-----------------------------------------
         * MATH FUNCTIONS
         *---------------------------------------*/

        osg::Vec3f Spring::normalize(const osg::Vec3f& vec)
        {
            float abs = absoluteVector(vec);
            return osg::Vec3f(vec[0] / abs, vec[1] / abs, vec[2] / abs);
        }

        osg::Vec3f Spring::getNormal(const osg::Vec3f& vec, float length)
        {
            osg::Vec3f vecN = normalize(vec);
            osg::Vec3f vecN_Abs = osg::Vec3f(std::abs(vecN[0]), std::abs(vecN[1]), std::abs(vecN[2]));

            //Get max value in vec
            float maxVal = std::fmaxf(std::fmaxf(vecN_Abs[0], vecN_Abs[1]), vecN_Abs[2]);
            int imax = 0;
            if (vecN_Abs[0] == maxVal)
                imax = 0;
            else if (vecN_Abs[1] == maxVal)
                imax = 1;
            else
                imax = 2;

            int rest[2] = { 0, 0 };

            switch (imax)
            {
                case (0):
                    rest[0] = 1;
                    rest[1] = 2;
                    break;
                case (1):
                    rest[0] = 0;
                    rest[1] = 2;
                    break;
                case (2):
                    rest[0] = 0;
                    rest[1] = 1;
                    break;
            }
            //calc a normal vector
            osg::Vec3f n = osg::Vec3f(0, 0, 0);
            n[rest[0]] = 1;
            n[rest[1]] = 1;
            n[imax] = -(vecN[rest[0]] + vecN[rest[1]]) / vecN[imax];
            n = normalize(n);
            n[0] = n[0] * length;
            n[1] = n[1] * length;
            n[2] = n[2] * length;
            return n;
        }

        float Spring::absoluteVector(const osg::Vec3f& vec)
        {
            return std::sqrt(std::pow(vec[0], 2) + std::pow(vec[1], 2) + std::pow(vec[2], 2));
        }

        float Spring::angleBetweenVectors(const osg::Vec3f& vec1, osg::Vec3f vec2)
        {
            float scalarProduct = vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2];
            return (std::acos(scalarProduct / (absoluteVector(vec1) * absoluteVector(vec2)))/* / M_PI * 180*/);
        }

        osg::Vec3f Spring::rotateX(const osg::Vec3f& vec, float phi)
        {
            return osg::Vec3f(vec[0], vec[1] * std::cos(phi) - vec[2] * std::sin(phi), vec[1] * std::sin(phi) + vec[2] * std::cos(phi));
        }

        osg::Vec3f Spring::rotateY(const osg::Vec3f& vec, float phi)
        {
            return osg::Vec3f(vec[2] * std::sin(phi) + vec[0] * std::cos(phi), vec[1], vec[2] * std::cos(phi) - vec[0] * std::sin(phi));
        }

        osg::Vec3f Spring::rotateZ(const osg::Vec3f& vec, float phi)
        {
            return osg::Vec3f(vec[0] * std::cos(phi) - vec[1] * std::sin(phi), vec[0] * std::sin(phi) + vec[1] * std::cos(phi), vec[2]);
        }

        osg::Vec3f Spring::rotateArbitraryAxes_expensive(const osg::Vec3f& vec, const osg::Vec3f& axes, float phi)
        {
            //this is how I would do it by hand.  Check out rotateArbitraryAxes, thats the shortest formula.
            //There is also still something wrong in here
            osg::Vec3f axesN = normalize(axes);
            osg::Vec3f aux = vec;
            //angle between vec and x, rotate in xz-plane
            float phiX = angleBetweenVectors(axesN, osg::Vec3f(1, 0, 0));
            aux = rotateX(aux, phiX);
            //angle between vec and x, rotate in z axes
            float phiY = angleBetweenVectors(axesN, osg::Vec3f(0, 1, 0));
            aux = rotateY(aux, phiY);
            // rotate around z
            aux = rotateZ(aux, phi);
            // and reverse
            aux = rotateY(aux, -phiY);
            aux = rotateX(aux, -phiX);
            return aux;
        }

        osg::Vec3f Spring::rotateArbitraryAxes(const osg::Vec3f& vec, const osg::Vec3f& axes, float phi)
        {
            osg::Vec3f axesN = normalize(axes);
            float M1_1 = (1 - std::cos(phi)) * axesN[0] * axesN[0] + std::cos(phi) * 1 + std::sin(phi) * 0;
            float M1_2 = (1 - std::cos(phi)) * axesN[0] * axesN[1] + std::cos(phi) * 0 + std::sin(phi) * (-axesN[2]);
            float M1_3 = (1 - std::cos(phi)) * axesN[0] * axesN[2] + std::cos(phi) * 0 + std::sin(phi) * (axesN[1]);
            float M2_1 = (1 - std::cos(phi)) * axesN[0] * axesN[1] + std::cos(phi) * 0 + std::sin(phi) * (axesN[2]);
            float M2_2 = (1 - std::cos(phi)) * axesN[1] * axesN[1] + std::cos(phi) * 1 + std::sin(phi) * 0;
            float M2_3 = (1 - std::cos(phi)) * axesN[1] * axesN[2] + std::cos(phi) * 0 + std::sin(phi) * (-axesN[0]);
            float M3_1 = (1 - std::cos(phi)) * axesN[0] * axesN[2] + std::cos(phi) * 0 + std::sin(phi) * (-axesN[1]);
            float M3_2 = (1 - std::cos(phi)) * axesN[1] * axesN[2] + std::cos(phi) * 0 + std::sin(phi) * (axesN[0]);
            float M3_3 = (1 - std::cos(phi)) * axesN[2] * axesN[2] + std::cos(phi) * 1 + std::sin(phi) * 0;
            return osg::Vec3f(M1_1 * vec[0] + M1_2 * vec[1] + M1_3 * vec[2],
                              M2_1 * vec[0] + M2_2 * vec[1] + M2_3 * vec[2],
                              M3_1 * vec[0] + M3_2 * vec[1] + M3_3 * vec[2]);
        }

    }  // End namespace Model
}  // End namespace OMVIS



