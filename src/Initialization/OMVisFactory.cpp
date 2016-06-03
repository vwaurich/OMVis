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
#include "Model/OMVisualizerAbstract.hpp"
#include "Util/Logger.hpp"
#include "Util/Util.hpp"
#include "Util/CommandLineArgs.hpp"
#include "Control/OMVisManager.hpp"

namespace OMVIS
{
    namespace Initialization
    {

        std::shared_ptr<Model::OMVisualizerAbstract> Factory::createVisualizationFromCLargs(const Util::CommandLineArgs& cLArgs)
        {
            return createVisualizationObject(cLArgs._modelName, cLArgs._modelPath, cLArgs._useFMU);
        }

        std::shared_ptr<Model::OMVisualizerAbstract> Factory::createVisualizationObject(const std::string modelName, const std::string path, const bool useFMU)
        {
            std::shared_ptr<Model::OMVisualizerAbstract> result(nullptr);
            std::string fullPath = path + "/" + modelName + "_visual.xml";

            // Command line is empty. Model has to be loaded via GUI. Return nullptr.
            if (modelName.empty() && path.empty())
            {
                LOGGER_WRITE("Initialize OMVisalizerAbstract because path and model name are empty.", Util::LC_LOADER, Util::LL_DEBUG);
            }
            else if (Util::fileExists(fullPath))
            {
                //FMU based visualization
                if (useFMU)
                {
                    result = std::shared_ptr<Model::OMVisualizerAbstract>(new Model::OMVisualizerFMU(modelName, path));
                    LOGGER_WRITE("Initialize OMVisalizerFMU.", Util::LC_LOADER, Util::LL_DEBUG);
                }
                //Mat-file based visualization
                else
                {
                    result = std::shared_ptr<Model::OMVisualizerAbstract>(new Model::OMVisualizerMAT(modelName, path));
                    LOGGER_WRITE("Initialize OMVisalizerMAT.", Util::LC_LOADER, Util::LL_DEBUG);
                }
            }
            else
            {
                LOGGER_WRITE("Initialize OMVisalizerAbstract because path " + (fullPath) + std::string(" can not be accessed."), Util::LC_LOADER, Util::LL_DEBUG);
            }
            return result;
        }

        std::shared_ptr<Model::OMVisualizerAbstract> Factory::createVisualizationObject(const VisualizationConstructionPlan& plan)
        {
            std::shared_ptr<Model::OMVisualizerAbstract> result(nullptr);
            std::string fullPath = plan.dirPath + "/" + plan.fileName + "_visual.xml";

            // Todo: Implement me!
            // Construction plan is valid?
            // if (!constructionPlanIsValid())

            if (plan.fileName.empty() && plan.dirPath.empty())
                LOGGER_WRITE("Initialize OMVisalizerAbstract because path and model name are empty.", Util::LC_LOADER, Util::LL_DEBUG);
            else if (Util::fileExists(fullPath))
            {
                //FMU based visualization
                if (plan.isFMU)
                {
                    if (plan.serverName.size() > 1 && plan.port != -1)
                    {
                        result = std::shared_ptr<Model::OMVisualizerAbstract>(new Model::OMVisualizerFMUClient(plan));
                        LOGGER_WRITE("Initialize OMVisalizerFMUClient.", Util::LC_LOADER, Util::LL_DEBUG);
                    }
                    else
                    {
                        result = std::shared_ptr<Model::OMVisualizerAbstract>(new Model::OMVisualizerFMU(plan.fileName, plan.dirPath));
                        LOGGER_WRITE("Initialize OMVisalizerFMU.", Util::LC_LOADER, Util::LL_DEBUG);
                    }
                }
                //Mat-file based visualization
                else
                {
//TODO                    if (plan.serverName.size() > 1 && plan.port != -1)
//                    {
//                        result = std::shared_ptr<Model::OMVisualizerAbstract>(new Model::OMVisualizerMATClient(plan));
//                        LOGGER_WRITE("Initialize OMVisalizerMATClient.", Util::LC_LOADER, Util::LL_DEBUG);
//                    }
//                    else
//                    {
                        result = std::shared_ptr<Model::OMVisualizerAbstract>(new Model::OMVisualizerMAT(plan.fileName, plan.dirPath));
                        LOGGER_WRITE("Initialize OMVisalizerMAT.", Util::LC_LOADER, Util::LL_DEBUG);
//                    }
                }
            }
            else
                LOGGER_WRITE("Initialize OMVisalizerAbstract because path " + (fullPath) + std::string(" can not be accessed."), Util::LC_LOADER, Util::LL_DEBUG);

            return result;

        }

    }  // End namespace Initialization
}  // End namespace OMVIS

