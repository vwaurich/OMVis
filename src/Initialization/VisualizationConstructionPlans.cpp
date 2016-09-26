/*
 * Copyright (C) 2016, Volker Waurich, Martin Flehmig
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

#include "Initialization/VisualizationConstructionPlans.hpp"

namespace OMVIS
{
    namespace Initialization
    {

        VisualizationConstructionPlan::VisualizationConstructionPlan()
                : visType(Model::VisType::NONE),
                  modelFile(""),
                  path("")
        {
        }

        VisualizationConstructionPlan::VisualizationConstructionPlan(const std::string& modelFileIn, const std::string& pathIn)
            : visType(Model::VisType::NONE),
              modelFile(modelFileIn),
              path(pathIn)
        {
            if (modelFileIn.empty())
                throw std::invalid_argument("No model file specified.");
            if (pathIn.empty())
                throw std::invalid_argument("No path specified.");

            // Get visualization type.
            if (Util::isFMU(modelFile))
                visType = Model::VisType::FMU;
            else if (Util::isMAT(modelFile))
                visType = Model::VisType::MAT;
            else
                throw std::invalid_argument("VisualizationType is NONE.");
        }

        RemoteVisualizationConstructionPlan::RemoteVisualizationConstructionPlan()
            : VisualizationConstructionPlan(),
              hostAddress(""),
              port(-1),
              wDir("")
        {
        }

        RemoteVisualizationConstructionPlan::RemoteVisualizationConstructionPlan(const std::string& modelFile,
                                                                                 const std::string& path,
                                                                                 const std::string& hostAddress,
                                                                                 const int port, const std::string& wDir)
            : VisualizationConstructionPlan(modelFile, path),
              hostAddress(hostAddress),
              port(port),
              wDir(wDir)
        {
            if (hostAddress != "localhost" && (!(Util::isValidIPv4(hostAddress) || Util::isValidIPv6(hostAddress))))
                throw std::invalid_argument("Invalid IP address.");
            if (wDir.empty())
                throw std::invalid_argument("No local working directory specified.");

            // Get visualization type.
            if (Util::isFMU (modelFile))
                visType = Model::VisType::FMU_REMOTE;
            else if (Util::isMAT(modelFile))
                visType = Model::VisType::MAT_REMOTE;
            else
                throw std::invalid_argument("VisualizationType is NONE.");
        }

    }  // End namespace Initialization
}  // End namespace OMVIS
