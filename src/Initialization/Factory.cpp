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

#include "Model/VisualizerFMU.hpp"
#include "Model/VisualizerFMUClient.hpp"
#include "Model/VisualizerMAT.hpp"
#include "Initialization/Factory.hpp"
#include "Util/Logger.hpp"

namespace OMVIS
{
    namespace Initialization
    {

        /*-----------------------------------------
         * CREATE METHODS
         *---------------------------------------*/

        std::shared_ptr<Model::VisualizerAbstract> Factory::createVisualizerObject(
                const VisualizationConstructionPlan* cP)
        {
            std::shared_ptr<Model::VisualizerAbstract> result(nullptr);

            // Todo: Implement me!
            // Construction plan is valid?
            // if (!constructionPlanIsValid())

            // Todo: This check can be moved to constructionPlanIsValid method
            if (cP->modelFile.empty() && cP->path.empty())
            {
                std::string msg = "Initialize VisalizerAbstract because path and model name are empty.";
                LOGGER_WRITE(msg, Util::LC_LOADER, Util::LL_ERROR);
                throw std::runtime_error(msg);
            }

            // Is the visual XML file present?
//            if (!Util::checkForXMLFile(cP.modelFile, cP.path))
//            {
//                std::string msg = "Visual XML file could not be found for the chosen model in the path.";
//                LOGGER_WRITE(msg, Util::LC_LOADER, Util::LL_ERROR);
//                throw std::runtime_error(msg);
//            }

            // FMU based visualization
            if (cP->visType == Model::VisType::FMU)
            {
                result = std::shared_ptr<Model::VisualizerAbstract>(new Model::VisualizerFMU(cP->modelFile, cP->path));
                LOGGER_WRITE("Initialize OMVisualizerFMU.", Util::LC_LOADER, Util::LL_DEBUG);
            }
            // MAT file based visualization
            else if (cP->visType == Model::VisType::MAT)
            {
                result = std::shared_ptr<Model::VisualizerAbstract>(new Model::VisualizerMAT(cP->modelFile, cP->path));
                LOGGER_WRITE("Initialize VisualizerMAT.", Util::LC_LOADER, Util::LL_DEBUG);
            }
            // FMU based remote visualization
            else if (cP->visType == Model::VisType::FMU_REMOTE)
            {
                result = std::shared_ptr<Model::VisualizerAbstract>(
                        new Model::VisualizerFMUClient(dynamic_cast<const RemoteVisualizationConstructionPlan*>(cP)));
                LOGGER_WRITE("Initialize VisualizerFMUClient.", Util::LC_LOADER, Util::LL_DEBUG);
            }
            // MAT file based remote visualization
            /// \todo Todo Implement OMVisualizerMATClient!
            else if (cP->visType == Model::VisType::MAT_REMOTE)
            {
                // result = std::shared_ptr<Model::OMVisualizerAbstract> (new Model::OMVisualizerMATClient(cP.modelFile, cP.workingDirectory));
                LOGGER_WRITE("Initialize OMVisualizerMAT. Argh, wait. This is not yet implemented!!!", Util::LC_LOADER,
                             Util::LL_ERROR);
            }
            else
            {
                std::string msg =
                        "The construction plan does not have a valid visualization type. A OMVisualizerAbstract(nullptr) is returned.";
                LOGGER_WRITE(msg, Util::LC_LOADER, Util::LL_ERROR);
                throw std::runtime_error(msg);
            }

            return result;
        }

    }  // namespace Initialization
}  // namespace OMVIS
