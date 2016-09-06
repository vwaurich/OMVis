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


#ifndef INCLUDE_ADDITIONALSHAPES_HPP
#define INCLUDE_ADDITIONALSHAPES_HPP

#define _USE_MATH_DEFINES // for C++
#include <math.h>

#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Shape>

class Pipecylinder : public osg::Geometry
{
public:
	Pipecylinder(float rI, float rO, float l);
	~Pipecylinder() {};
};

class Spring : public osg::Geometry
{
public:
	Spring(float r, float rCoil, float nWindings,  float l);
	~Spring() {};
private:
   osg::Vec3f getNormal(osg::Vec3f vec, float length = 1);
   osg::Vec3f normalize(osg::Vec3f vec);
   osg::Vec3f rotateX(osg::Vec3f vec, float phi);
   osg::Vec3f rotateY(osg::Vec3f vec, float phi);
   osg::Vec3f rotateZ(osg::Vec3f vec, float phi);
   osg::Vec3f rotateArbitraryAxes_expensive(osg::Vec3f vec, osg::Vec3f axes, float phi);
   osg::Vec3f rotateArbitraryAxes(osg::Vec3f vec, osg::Vec3f axes, float phi);
   float absoluteVector(osg::Vec3f vec);
   float angleBetweenVectors(osg::Vec3f vec1, osg::Vec3f vec2);

   osg::Vec3Array* outerVertices;
   osg::Vec3Array* splineVertices;
};


#endif