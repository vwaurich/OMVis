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

/** \file
 *
 *  \copyright TU Dresden. All rights reserved.
 *  \authors Volker Waurich, Martin Flehmig
 *  \date Feb 2016
 *
 *  This file comprises declarations of methods that have two manifestations: One serves for the
 *  \ref OMVIS::Model::VisualizerFMU and the other one for \ref OMVIS::Model::VisualizerMAT objects.
 */

#ifndef INCLUDE_VISUALIZE_HPP_
#define INCLUDE_VISUALIZE_HPP_

#include "WrapperFMILib.hpp"
#include "Model/ShapeObjectAttribute.hpp"

#include <rapidxml.hpp>

// NetOff
#include <ValueContainer.hpp>

#include <osg/Geode>
#include <osg/Vec3f>

namespace OMVIS
{
    namespace Util
    {

        //-----------------------
        // CLASSES
        //-----------------------

        /**
         * struct to handle position vector and rotationMatrix
         * \todo Find a more suitable name for this struct and better place.
         */
        struct rAndT
        {
            rAndT()
                    : _r(osg::Vec3f()),
                      _T(osg::Matrix3())
            {
            }
            osg::Vec3f _r;
            osg::Matrix3 _T;
        };

        /**
         length and width direction
         */
        struct Directions
        {
            Directions()
                    : _lDir(osg::Vec3f()),
                      _wDir(osg::Vec3f())
            {
            }
            osg::Vec3f _lDir;
            osg::Vec3f _wDir;
        };

        /******************************
         Calculate Transformations
         *******************************/

        /*! \brief Modelica.Math.Vectors.normalize
         * \param
         * \return
         */
        osg::Vec3f normalize(const osg::Vec3f& vec);

        /*! \brief Calculates the cross product of two three-dimensional vectors.
         *
         * \param vec1  First vector
         * \param vec2  Second vector
         * \return Cross product of first and second vector
         */
        osg::Vec3f cross(const osg::Vec3f& vec1, const osg::Vec3f& vec2);

        Directions fixDirections(const osg::Vec3f& lDir, const osg::Vec3f& wDir);

        //-----------------------
        // FUNCTIONS
        //-----------------------

        /*! \brief Update the attribute of the object using a MAT file result. */
        void updateObjectAttributeFMU(Model::ShapeObjectAttribute* attr, double time, fmi1_import_t* fmu);

        /*! \brief Update the attribute of the object using remote FMU visualization. */
        void updateObjectAttributeFMUClient(Model::ShapeObjectAttribute& attr, const NetOff::ValueContainer& _outputCont);

        /*! \brief Gets the value of the indicated node exp. */
        double getShapeAttrFMU(const char* attr, rapidxml::xml_node<>* node, double time, fmi1_import_t* fmu);

        /*! \brief Gets the ObjectAttribute for a certain node. */
        Model::ShapeObjectAttribute getObjectAttributeForNode(const rapidxml::xml_node<>*);

        //osg::Matrix
        void assemblePokeMatrix(osg::Matrix& M, const osg::Matrix3& T, const osg::Vec3f& r);

        /*! \brief Updates r and T to cope with the directions. */
        rAndT rotation(const osg::Vec3f& r, const osg::Vec3f& r_shape, const osg::Matrix3& T,
                       const osg::Vec3f& lDirIn, const osg::Vec3f& wDirIn,
                       const float length, const std::string& type);

    }  //  namespace Util
}  //  namespace OMVIS

#endif /* INCLUDE_VISUALIZE_HPP_ */
