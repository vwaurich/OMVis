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
#include "Model/OMVisualizerFMUClient.hpp"
#include "Model/OMVisualizerMAT.hpp"
#include "Model/OMVisualizerMATClient.hpp"
#include "Util/Logger.hpp"
#include "Util/Util.hpp"

namespace OMVIS
{
    namespace Initialization
    {

        /*-----------------------------------------
         * CREATE METHODS
         *---------------------------------------*/

        std::shared_ptr<Model::OMVisualizerAbstract> Factory::createVisualizationObject(const VisualizationConstructionPlan& cP)
        {
            std::shared_ptr<Model::OMVisualizerAbstract> result(nullptr);

            // Todo: Implement me!
            // Construction plan is valid?
            // if (!constructionPlanIsValid())

            if (cP.modelFile.empty() && cP.path.empty())
                LOGGER_WRITE("Initialize OMVisalizerAbstract because path and model name are empty.", Util::LC_LOADER, Util::LL_DEBUG);
            else if (Util::checkForXMLFile(cP.modelFile, cP.path))
            {
                //FMU based visualization
                if (cP.isFMU)
                {
                    result = std::shared_ptr<Model::OMVisualizerAbstract>(new Model::OMVisualizerFMU(cP.modelFile, cP.path));
                    LOGGER_WRITE("Initialize OMVisalizerFMU.", Util::LC_LOADER, Util::LL_DEBUG);
                }
                //MAT file based visualization
                else
                {
                    result = std::shared_ptr<Model::OMVisualizerAbstract>(new Model::OMVisualizerMAT(cP.modelFile, cP.path));
                    LOGGER_WRITE("Initialize OMVisalizerMAT.", Util::LC_LOADER, Util::LL_DEBUG);
                }
            }
            else
            {
                std::string msg = "Visual XML file could not be found for the chosen model in the path.";
                LOGGER_WRITE(msg, Util::LC_LOADER, Util::LL_ERROR);
                throw std::runtime_error(msg);
            }
            return result;
        }

    }  // End namespace Initialization
}  // End namespace OMVIS

