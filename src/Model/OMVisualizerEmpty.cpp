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


#include <iostream>
#include "Util/Logger.hpp"
#include "Model/OMVisualizerEmpty.hpp"
#include "View/OMVisualViewer.hpp"
#include "Control/OMVisManager.hpp"

namespace Model
{

	OMVisualizerEmpty::OMVisualizerEmpty (const std::string modelName, const std::string modelPath)
            : OMVisualizerAbstract(modelName, modelPath)
    {
    }
	
    void OMVisualizerEmpty::initData()
    {
        OMVisualizerAbstract::initData();
    }

    void OMVisualizerEmpty::updateVisAttributes(const double time)
    {
    }  //end function

    void OMVisualizerEmpty::initializeVisAttributes(const double time)
    {
        updateVisAttributes(_omvManager->_visTime);
    }

    void OMVisualizerEmpty::updateScene(const double time)
    {

    }

    int OMVisualizerEmpty::getDataTypeID()
    {
        return -1;
    }

}  // End namespace Model

