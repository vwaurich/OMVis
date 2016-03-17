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
 * OMVisScene.cpp
 *
 *  Created on: 03.02.2016
 *      Author: mf
 */

#include <iostream>
#include <osgGA/TrackballManipulator>
#include "View/OMVisScene.hpp"
#include "Util/Logger.hpp"
#include "InfoVisitor.hpp"

namespace View
{

    OMVisScene::OMVisScene()
            : _scene()
    {
    }


	void OMVisScene::dumpOSGTreeDebug()
	{
		// The node traverser which dumps the tree
		InfoVisitor infoVisitor;
		_scene._rootNode->accept(infoVisitor);
	}


}  // End namespace View
