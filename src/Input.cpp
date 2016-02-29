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
#include <SDL.h>
#include "Input.hpp"
//#include "InputData.hpp"
//#include "JoystickDevice.hpp"


int causalityEqual(fmi1_import_variable_t* var, void* enumIdx)
{
    fmi1_causality_enu_t causality = fmi1_import_get_causality(var);
    int isEqual = 0;

    /// \todo Check this cast: void * to an int
    //MF: You cast an void * to an int? Can this work???
	//VW: For me it works. Its the definition of the callback used in fmi1_import_filter_variables(). Feel free to bring in some type-control.
    int enumIdxInt = (int) enumIdx;
    if (enumIdxInt == causality)
    {
    		isEqual = 1;
    		//std::cout<<"the input var: "<<fmi1_import_get_variable_name(var)<<std::endl;
    }
    //std::cout<<"check the causality: "<<causality<<" is it an input "<<isEqual<<std::endl;
    return isEqual;
}

int baseTypeEqual(fmi1_import_variable_t* var, void* refBaseType)
{
    fmi1_base_type_enu_t baseType = fmi1_import_get_variable_base_type(var);
    int isEqual = 0;

    /// \todo Check this cast: void * to an int
    //MF: You cast an void * to an int? Can this work???
	//VW: For me it works. Its the definition of the callback used in fmi1_import_filter_variables(). Feel free to bring in some type-control.

    int refBaseType1 = (int)(refBaseType);
    if (refBaseType1 == baseType)
    {
    		isEqual = 1;
    		//std::cout<<"the input var: "<<fmi1_import_get_variable_name(var)<<std::endl;
    }

    return isEqual;
}
