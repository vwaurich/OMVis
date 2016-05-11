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

#ifndef INCLUDE_OBJECTATTRIBUTE_HPP_
#define INCLUDE_OBJECTATTRIBUTE_HPP_

#include <string>

class ObjectAttribute
{
public:
	ObjectAttribute();

	ObjectAttribute(float value);

	~ObjectAttribute() = default;

	//outputs a string representation for the attribute
	std::string getValueString() const;

public:
	//if true, check the exp, if wrong check the cref
	bool isConst;
	float exp;
	std::string cref;
};


#endif // INCLUDE_OBJECTATTRIBUTE_HPP_
