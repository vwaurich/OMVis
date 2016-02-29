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
#include "Model/OMVisualizerMAT.hpp"
#include "Model/OMVisualizerAbstract.hpp"
#include "Util/Logger.hpp"
#include "Util/CommandLineArgs.hpp"
#include "View/OMVManager.hpp"

namespace Initialization
{
//    Factory::Factory(Util::CommandLineArgs* cLArgs)
//            : _useFMU(cLArgs->_useFMU),
//              _modelName(cLArgs->_modelName),
//              _modelPath(cLArgs->_modelPath)
//    {
//    }

    Model::OMVisualizerAbstract* Factory::createVisualizationFromCLargs(const Util::CommandLineArgs& cLArgs)
    {
        return createVisualizationObjects(cLArgs._modelName, cLArgs._modelPath, cLArgs._useFMU);
    }

	Model::OMVisualizerAbstract* Factory::createVisualizationObjects(std::string modelName, std::string pathName, bool useFMU)
	{
		Model::OMVisualizerAbstract* result;

		//FMU based visualisation
		if (useFMU)
		{
			result = (Model::OMVisualizerAbstract*)(new Model::OMVisualizerFMU(modelName, pathName));
			LOGGER_WRITE("Initialize OMVisalizerFMU.", Util::LC_LOADER, Util::LL_DEBUG);
		}
		//Mat-file based visualisation
		else
		{
			result = (Model::OMVisualizerAbstract*)(new Model::OMVisualizerMAT(modelName, pathName));
			LOGGER_WRITE("Initialize OMVisalizerMAT.", Util::LC_LOADER, Util::LL_DEBUG);
		}
		return result;
	}

}  // End namespace Initialization


