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
 * OMVisualViewer.cpp
 *
 *  Created on: 03.02.2016
 *      Author: mf
 */

#include <iostream>
#include <osgGA/TrackballManipulator>
#include "View/OMVisualViewer.hpp"
#include "Util/Logger.hpp"
#include "InfoVisitor.hpp"

namespace View
{

    OMVisualViewer::OMVisualViewer()
            : _viewer(),
              _scene()
    {
    }

    void OMVisualViewer::initViewer()
    {
        // set up osg::viewer
        LOGGER_WRITE(std::string("Set up Camera Manipulator."), Util::LC_LOADER, Util::LL_DEBUG);
        _viewer.setCameraManipulator(new osgGA::TrackballManipulator);

        LOGGER_WRITE(std::string("Set up View Window."), Util::LC_LOADER, Util::LL_DEBUG);
        _viewer.setUpViewInWindow(500, 500, 1000, 700);  //MF: This method is "depreacted, use view.apply(new osgViewer::SingleWindow(x,y,width,screenNum))."

        LOGGER_WRITE(std::string("Get Camera."), Util::LC_LOADER, Util::LL_DEBUG);
        _viewer.getCamera()->setClearColor((osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f)));

        LOGGER_WRITE(std::string("Call realize() method on viewer."), Util::LC_LOADER, Util::LL_DEBUG);
        _viewer.realize();
    }

	void OMVisualViewer::dumpOSGTreeDebug()
	{
		// The node traverser which dumps the tree
		InfoVisitor infoVisitor;
		_scene._rootNode->accept(infoVisitor);
	}


}  // End namespace View
