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
 * Factory.cpp
 *
 *  Created on: 18.02.2016
 *      Author: mf
 */

#include "Initialization/Factory.hpp"
#include "Model/OMVisualizerFMU.hpp"
#include "Model/OMVisualizerEmpty.hpp"
#include "Model/OMVisualizerMAT.hpp"
#include "Model/OMVisualizerAbstract.hpp"
#include "Util/Logger.hpp"
#include "Util/CommandLineArgs.hpp"
#include "View/OMVManager.hpp"

namespace Initialization
{

    Model::OMVisualizerAbstract* Factory::createVisualizationFromCLargs(const Util::CommandLineArgs& cLArgs)
    {
        return createVisualizationObject(cLArgs._modelName, cLArgs._modelPath, cLArgs._useFMU);
    }

    Model::OMVisualizerAbstract* Factory::createVisualizationObject(const std::string modelName, const std::string pathName, const bool useFMU)
    {
        Model::OMVisualizerAbstract* result;

        // Command line is empty. Model has to be loaded via GUI. Return pointer to base class object.
        if (modelName.empty() && pathName.empty())
        {
            result = (Model::OMVisualizerAbstract*) (new Model::OMVisualizerEmpty("",""));
            LOGGER_WRITE("Initialize OMVisalizerAbstract.", Util::LC_LOADER, Util::LL_DEBUG);
        }
        else
        {
            //FMU based visualisation
            if (useFMU)
            {
                result = (Model::OMVisualizerAbstract*) (new Model::OMVisualizerFMU(modelName, pathName));
                LOGGER_WRITE("Initialize OMVisalizerFMU.", Util::LC_LOADER, Util::LL_DEBUG);
            }
            //Mat-file based visualisation
            else
            {
                result = (Model::OMVisualizerAbstract*) (new Model::OMVisualizerMAT(modelName, pathName));
                LOGGER_WRITE("Initialize OMVisalizerMAT.", Util::LC_LOADER, Util::LL_DEBUG);
            }
        }
        return result;
    }

}  // End namespace Initialization

