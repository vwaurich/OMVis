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

/*
 * OSGScene.hpp
 *
 *  Created on: 03.02.2016
 *      Author: mf
 */

#ifndef INCLUDE_OSGSCENE_HPP_
#define INCLUDE_OSGSCENE_HPP_

#include "Util/rapidxml.hpp"
#include <osg/Group>
#include <osgDB/ReadFile>

namespace View
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
         */
        void setUpScene(rapidxml::xml_node<>* xmlRoot);

     public:
        /// \todo Can this attr. be private?
        osg::ref_ptr<osg::Group> _rootNode;
		std::string _path;
    };

}  // End namespace View

#endif /* INCLUDE_OSGSCENE_HPP_ */
