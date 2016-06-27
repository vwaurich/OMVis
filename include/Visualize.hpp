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

/** @addtogroup OMVis
 *  @{
 *  \copyright TU Dresden. All rights reserved.
 *  \authors Volker Waurich, Martin Flehmig
 *  \date Feb 2016
 *
 *  This file comprises declarations of methods that have two manifestations: One serves for the \ref OMvisualizerFMU
 *  and the other one for \ref OMVisualizerMat objects.
 */

#ifndef INCLUDE_VISUALIZE_HPP_
#define INCLUDE_VISUALIZE_HPP_

#include "WrapperFMILib.hpp"
#include "Model/ShapeObjectAttribute.hpp"

#include <read_matlab4.h>
#include <rapidxml.hpp>

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

        /******************************
         Calculate Transformations
         *******************************/

        /*! \brief Modelica.Math.Vectors.normalize
         * @param
         * @return
         */
        osg::Vec3f normalize(osg::Vec3f vec);

        /*! \brief Cross
         *
         * @param
         * @return
         */
        osg::Vec3f cross(osg::Vec3f vec1, osg::Vec3f vec2);

        /*! \brief Bla Bla
         *
         * @param
         * @return
         */
        Directions fixDirections(osg::Vec3f lDir, osg::Vec3f wDir);

        //-----------------------
        // FUNCTIONS
        //-----------------------

        /*! \brief Gets the vector of the indicated node exp of the shape.
         */
        //X13 unused?
        //osg::Matrix3 getShapeMatrixMAT(char* attr, rapidxml::xml_node<>* node, double time, ModelicaMatReader reader);

        /*! \brief Gets the vector of the indicated node exp of the shape.
         *
         * \todo Currently unused.
         */
        osg::Matrix3 getShapeMatrixFMU(char* attr, rapidxml::xml_node<>* node, double time, fmi1_import_t* fmu);

        /*! \brief Gets the vector of the indicated node exp of the shape.
         *
         * \todo Currently unused.
         */
        //X13 unused?
        //osg::Vec3f getShapeVectorMAT(char* attr, rapidxml::xml_node<>* node, double time, ModelicaMatReader reader);

        /*! \brief Gets the vector of the indicated node exp of the shape.
         *
         * \todo Currently unused.
         */
        osg::Vec3f getShapeVectorFMU(char* attr, rapidxml::xml_node<>* node, double time, fmi1_import_t* fmu);

        /*! \brief Gets the value of the indicated node exp.
         *
         * \todo Currently unused.
         */
        //X13 unused
        //X13 double getShapeAttrMAT(const char* attr, rapidxml::xml_node<>* node, double time, ModelicaMatReader reader);

//        /*! \brief Update the attribute of the Object using a mat-file result
//         */
//        void updateObjectAttributeMAT(Model::ShapeObjectAttribute* attr, double time, ModelicaMatReader reader);

        /*! \brief Update the attribute of the Object using a mat-file result
         */
        void updateObjectAttributeFMU(Model::ShapeObjectAttribute* attr, double time, fmi1_import_t* fmu);

        /*! \brief Update the attribute of the Object using remote FMU visualization.
         */
        void updateObjectAttributeFMUClient(Model::ShapeObjectAttribute& attr, const NetOff::ValueContainer& _outputCont);

        /*! \brief Gets the value of the indicated node exp.
         */
        double getShapeAttrFMU(const char* attr, rapidxml::xml_node<>* node, double time, fmi1_import_t* fmu);

        /*! \brief Gets the ObjectAttribute for a certain node
         */
        Model::ShapeObjectAttribute getObjectAttributeForNode(rapidxml::xml_node<>*);

        osg::Matrix assemblePokeMatrix(osg::Matrix M, osg::Matrix3 T, osg::Vec3f r);

        /*! \brief Updates r and T to cope with the directions.
         *
         * @param
         * @return
         */
        rAndT rotation(osg::Vec3f r, osg::Vec3f r_shape, osg::Matrix3 T, osg::Vec3f lDirIn, osg::Vec3f wDirIn, float length, float width, float height, std::string type);

    }  // End namespace Util
}  // End namespace OMVIS

#endif /* INCLUDE_VISUALIZE_HPP_ */
/**
 * @}
 */
