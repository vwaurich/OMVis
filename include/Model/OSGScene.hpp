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

/** @addtogroup View
 *  @{
 *  \copyright TU Dresden. All rights reserved.
 *  \authors Volker Waurich, Martin Flehmig
 *  \date Feb 2016
 */

#ifndef INCLUDE_OSGSCENE_HPP_
#define INCLUDE_OSGSCENE_HPP_

#include "Model/ShapeObject.hpp"

#include <rapidxml.hpp>
#include <osg/Group>
#include <osgDB/ReadFile>

namespace OMVIS
{
    namespace Model
    {

        /*! \brief Class that stores the pointer to the root node of the models OSG scene.
         *
         * \todo This class handles access to the root node. Encapsulate access to the pointer.
         */
        class OSGScene
        {
         public:
            OSGScene();
            ~OSGScene() = default;
            OSGScene(const OSGScene& osgs) = delete;
            OSGScene& operator=(const OSGScene& osgs) = delete;

            /*! \brief Sets up all nodes initially.
             *
             * @param xmlRoot
             * \return Error value.
             */
            int setUpScene(std::vector<Model::ShapeObject> allShapes);

            /*! \brief Return shared pointer to rootNode. */
            osg::ref_ptr<osg::Group> getRootNode();

            /*! \brief Return path to the scene file. */
            std::string getPath() const;

            /*! \brief Set path to the scene file. */
            void setPath(const std::string path);

         private:
            /*! Root node of the scene. */
            osg::ref_ptr<osg::Group> _rootNode;

            /*! Path to the scene file. */
            std::string _path;
        };

    }  // End namespace Model
}  // End namespace OMVIS

#endif /* INCLUDE_OSGSCENE_HPP_ */
/**
 * @}
 */
