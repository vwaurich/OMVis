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
#include "Util/Logger.hpp"

int causalityEqual(fmi1_import_variable_t* var, void* enumIdx)
{
    // Get causality attribute, i.e., fmi1_causality_enu_input, fmi1_causality_enu_output, etc.
    fmi1_causality_enu_t causality = fmi1_import_get_causality(var);
    // Use static cast to "convert" void* to fmi1_causality_enu_t*. With that, we can compare the attributes.
    fmi1_causality_enu_t* toComp = static_cast<fmi1_causality_enu_t*>(enumIdx);
    //int enumIdxInt = (int) enumIdx;

    // LOGGER_WRITE(std::string("causalityEqual: fmi1_causality_enu_t causality is ") + std::to_string(causality) + " and enumIdx has attribute " + std::to_string(*toComp) + std::string("."), Util::LC_INIT, Util::LL_INFO);
    if (*toComp == causality)
    {
        //LOGGER_WRITE(std::string("They are equal!"), Util::LC_INIT, Util::LL_INFO);
        //std::cout << "the input var: " << fmi1_import_get_variable_name(var) << std::endl;
        return 1;
    }
    else
    {
        //LOGGER_WRITE(std::string("They are not equal!"), Util::LC_INIT, Util::LL_INFO);
        return 0;
    }
}

int baseTypeEqual(fmi1_import_variable_t* var, void* refBaseType)
{
    fmi1_base_type_enu_t baseType = fmi1_import_get_variable_base_type(var);
    fmi1_base_type_enu_t* toComp = static_cast<fmi1_base_type_enu_t*>(refBaseType);

    LOGGER_WRITE(std::string("baseTypeEqual: fmi1_base_type_enu_t baseType is ") + std::to_string(baseType) + " and refBaseType is of type" + std::to_string(*toComp) + std::string("."), Util::LC_INIT, Util::LL_INFO);
    if (*toComp == baseType)
    {
        LOGGER_WRITE(std::string("They are equal!"), Util::LC_INIT, Util::LL_INFO);
        return 1;
    }
    else
    {
        LOGGER_WRITE(std::string("They are not equal!"), Util::LC_INIT, Util::LL_INFO);
        return 0;
    }
}
