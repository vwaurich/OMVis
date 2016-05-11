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

#include"Util/ObjectAttribute.hpp"

ObjectAttribute::ObjectAttribute() :
	isConst(true),
	exp(0.0),
	cref("NONE")
{}

ObjectAttribute::ObjectAttribute(float value) :
	isConst(true),
	exp(value),
	cref("NONE")
{}


std::string ObjectAttribute::getValueString() const {
	if (isConst)
	{
		return std::to_string(exp);
	}
	else
	{
		return std::to_string(exp);
	}
};