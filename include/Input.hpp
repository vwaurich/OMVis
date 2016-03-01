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

#ifndef INCLUDE_INPUT_HPP_
#define INCLUDE_INPUT_HPP_

#include "WrapperFMILib.hpp"

/// \todo: Wo werden die Methoden causalityEqual und co benutzt? Wo sind sie besser aufgehoben?

//-----------------------
//FUNCTIONS
//-----------------------

/*! \brief Checks the causality of the var and outputs 1 if they are equal, 0 otherwise.
 *
 */
int causalityEqual(fmi1_import_variable_t* var, void* enumIdx);

/*! \brief Checks the type of the var and outputs 1 if they are equal.
 *
 */
int baseTypeEqual(fmi1_import_variable_t* var, void* refBaseType);

#endif /* INCLUDE_INPUT_HPP_ */
