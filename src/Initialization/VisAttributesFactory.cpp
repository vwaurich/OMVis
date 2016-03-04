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
#include <string>
#include "Initialization/VisAttributesFactory.hpp"
#include "Visualize.hpp"
#include "Model/ShapeAttributes.hpp"
#include "Model/PrismAttributes.hpp"


namespace Initialization
{

	Model::VisAttributes* VisAttributesFactory::createVisAttributes(std::string typeName)
    {
		//default is a shape object
		Model::VisAttributes* visAttr = (Model::VisAttributes*) new Model::ShapeAttributes;

		//get a shape object
		if (isShapeAttrTypeFromString(typeName))
		{
			visAttr = (Model::VisAttributes*) new Model::ShapeAttributes;
		}

		//get a prism object
		else if (isPrismAttrTypeFromString(typeName))
		{
			visAttr = (Model::VisAttributes*) new Model::PrismAttributes;
		}

		return visAttr;
    }


}  // End namespace Initialization


