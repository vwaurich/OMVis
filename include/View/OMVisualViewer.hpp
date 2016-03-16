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
 * OMVisualViewer.hpp
 *
 *  Created on: 03.02.2016
 *      Author: mf
 */

#ifndef INCLUDE_OMVISUALVIEWER_HPP_
#define INCLUDE_OMVISUALVIEWER_HPP_

#include <osgViewer/Viewer>
#include "View/OSGScene.hpp"

namespace View
{

    /*! \brief Base class that contains xml infos and viewer.
     *
     * \todo: privatize members
     */
    class OMVisualViewer
    {
     public:
        OMVisualViewer();
        ~OMVisualViewer() = default;

        OMVisualViewer(const OMVisualViewer& omvv) = delete;
        OMVisualViewer& operator=(const OMVisualViewer& omvv) = delete;

		/*! \brief This initializes the viewer.
		*
		*/
        //X9 void initViewer();


		/*! \brief This method dumps a string representation of the scene graph to stdOut.
		*
		*/
		void dumpOSGTreeDebug();

     public:
        /// The osg viewer holds a single view to a single scene.
        //X9osgViewer::Viewer _osgViewer;
        /// \todo Can this attr. be private?
        OSGScene _scene;
    };


}  // End namespace View

#endif /* INCLUDE_OMVISUALVIEWER_HPP_ */
