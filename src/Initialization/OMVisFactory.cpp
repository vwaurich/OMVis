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

#include "Initialization/OMVisFactory.hpp"
#include "Model/OMVisualizerFMU.hpp"
#include "Model/OMVisualizerMAT.hpp"
#include "Model/OMVisualizerAbstract.hpp"
#include "Util/Logger.hpp"
#include "Util/Util.hpp"
#include "Util/CommandLineArgs.hpp"
#include "Control/OMVisManager.hpp"

namespace OMVIS
{
    namespace Initialization
    {

        Model::OMVisualizerAbstract* Factory::createVisualizationFromCLargs(const Util::CommandLineArgs& cLArgs)
        {
            return createVisualizationObject(cLArgs._modelName, cLArgs._modelPath, cLArgs._useFMU);
        }

        Model::OMVisualizerAbstract* Factory::createVisualizationObject(const std::string modelName, const std::string path, const bool useFMU)
        {
            Model::OMVisualizerAbstract* result(nullptr);

            // Command line is empty. Model has to be loaded via GUI. Return nullptr.
            if (modelName.empty() && path.empty())
            {
                LOGGER_WRITE("Initialize OMVisalizerAbstract because path and model name are empty.", Util::LC_LOADER, Util::LL_DEBUG);
            }
            //else if (Util::exists(path + "/" + modelName + "_visual.xml"))
            else if (Util::exists(path))
            {
                //FMU based visualization
                if (useFMU)
                {
                    result = (Model::OMVisualizerAbstract*) (new Model::OMVisualizerFMU(modelName, path));
                    LOGGER_WRITE("Initialize OMVisalizerFMU.", Util::LC_LOADER, Util::LL_DEBUG);
                }
                //Mat-file based visualization
                else
                {
                    result = (Model::OMVisualizerAbstract*) (new Model::OMVisualizerMAT(modelName, path));
                    LOGGER_WRITE("Initialize OMVisalizerMAT.", Util::LC_LOADER, Util::LL_DEBUG);
                }
            }
            else
            {
                LOGGER_WRITE("Initialize OMVisalizerAbstract because path " + (path + "/" + modelName) + std::string(" can not be accessed."), Util::LC_LOADER, Util::LL_DEBUG);
            }
            return result;
        }

    }  // End namespace Initialization
}  // End namespace OMVIS

