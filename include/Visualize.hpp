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

/**
 OMVis
 visualize.h
 Purpose: Class definitions for the visualization

 @author Volker Waurich
 @version 0.1
 */

#ifndef INCLUDE_VISUALIZE_HPP_
#define INCLUDE_VISUALIZE_HPP_

#include <osg/Geode>
#include <osg/Material>
#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>
#include <osg/Vec3f>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>
#include "WrapperFMILib.hpp"
#include "Util/read_matlab4.h"
#include <rapidxml.hpp>


//-----------------------
//CLASSES
//-----------------------

/**
 * struct to handle position vector and rotationMatrix
 * \todo Find a more suitable name for this struct and better place.
 */
struct rAndT
{
    osg::Vec3f _r;
    osg::Matrix3 _T;
};

/**
 length and width direction
 */
struct Directions
{
    osg::Vec3f _lDir;
    osg::Vec3f _wDir;
};

//-----------------------
//FUNCTIONS
//-----------------------

bool isCADType(std::string typeName);

std::string extractCADFilename(std::string s);

bool exists(const std::string& name);

std::string getShapeType(rapidxml::xml_node<>* node);


osg::Matrix3 getShapeMatrixMAT(char* attr, rapidxml::xml_node<>* node, double time, ModelicaMatReader reader);

osg::Matrix3 getShapeMatrixFMU(char* attr, rapidxml::xml_node<>* node, double time, fmi1_import_t* fmu);


std::string getShapeIdent(rapidxml::xml_node<>* node);


osg::Vec3f getShapeVectorMAT(char* attr, rapidxml::xml_node<>* node, double time, ModelicaMatReader reader);

osg::Vec3f getShapeVectorFMU(char* attr, rapidxml::xml_node<>* node, double time, fmi1_import_t* fmu);


double getShapeAttrMAT(const char* attr, rapidxml::xml_node<>* node, double time, ModelicaMatReader reader);

double getShapeAttrFMU(const char* attr, rapidxml::xml_node<>* node, double time, fmi1_import_t* fmu);


osg::Matrix assemblePokeMatrix(osg::Matrix M, osg::Matrix3 T, osg::Vec3f r);

rAndT staticRotation(osg::Vec3f r, osg::Vec3f r_shape, osg::Matrix3 T, osg::Vec3f lDirIn, osg::Vec3f wDirIn, double length, double width, double height, std::string type);

#endif /* INCLUDE_VISUALIZE_HPP_ */
