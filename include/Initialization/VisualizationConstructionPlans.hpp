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

/** @addtogroup Initialization
 *  @{
 *  \copyright TU Dresden. All rights reserved.
 *  \authors Volker Waurich, Martin Flehmig
 *  \date Jun 2016
 */

#ifndef INCLUDE_INITIALIZATION_VISUALIZATIONCONSTRUCTIONPLANS_HPP_
#define INCLUDE_INITIALIZATION_VISUALIZATIONCONSTRUCTIONPLANS_HPP_

#include "Util/Util.hpp"
#include "Model/VisualizationTypes.hpp"

#include <string>
#include <stdexcept>

namespace OMVIS
{

    namespace Initialization
    {

        /*! \brief This class represents a construction plan for a local visualization of a simulation.
         *
         * This class holds the necessary information to let the factory create a OMVisualizer object. The computation
         * and visualization is done on the localhost.
         *
         * A construction plan is fully specified by the name of the FMU or MAT file. The name includes the path to the
         * file and the prefix (.fmu or .mat).
         */
        class VisualizationConstructionPlan
        {
         public:
            /*-----------------------------------------
             * CONSTRUCTORS
             *---------------------------------------*/

            /*! \brief Default constructor.
             *
             * Initializes the members with default values.
             */
            VisualizationConstructionPlan();

            /*! \brief Constructor from arguments.
             *
             * This constructors throws std::invalid_argument exceptions, if the object can not be constructed. This
             * arises, if the model file name or the path is empty or the model tpe (FMU or MAT) cannot determined
             * (no prefix at model file name).
             *
             * \param modelFileIn   Name of the model file with prefix, e.g., ModelX.fmu
             * \param pathIn        Path to the model file.
             */
            VisualizationConstructionPlan(const std::string& modelFileIn, const std::string& pathIn);

            virtual ~VisualizationConstructionPlan() = default;

            VisualizationConstructionPlan(const VisualizationConstructionPlan&) = default;
            VisualizationConstructionPlan& operator=(const VisualizationConstructionPlan&) = default;

            /*-----------------------------------------
             * Members
             *---------------------------------------*/

            Model::VisType visType;
            /*! Name of the model file without path but with prefix, e.g., modelFoo.fmu . */
            std::string modelFile;
            /*! Path to the model file, e.g., /home/user/models/ . */
            std::string path;
        };

        /*! \brief This class represents a construction plan for a remote visualization of a simulation.
         *
         * This class holds the necessary information to let the factory create a OMVisualizer[FMU|MAT]Client object.
         * The computation is performed on the remote server whereas the visualization is done on the localhost.
         *
         * A construction plan is fully specified by the following attributes:
         *      - IP address of the server
         *      - Port number to use
         *      - Name of the model file (Currently, only FMUs can be used for remote visualization)
         *      - Path to the model file on server
         *      - Local working directory (e.g., ./tmp)
         */
        class RemoteVisualizationConstructionPlan : public VisualizationConstructionPlan
        {
         public:
            /*-----------------------------------------
             * Constructors
             *---------------------------------------*/

            /*! \brief Default constructor.
             *
             * Initializes the members with default values.
             */
            RemoteVisualizationConstructionPlan();

            /*! \brief Constructor from arguments.
             *
             * This constructors throws std::invalid_argument exceptions, if the object can not be constructed. This
             * arises, if
             *      - the model file name or the path is empty
             *      - the model type (FMU or MAT) cannot determined
             *      - the host address is not valid.
             *
             * \param modelFile     Name of the model file with prefix, e.g., ModelX.fmu
             * \param path          Path to the model file.
             * \param hostAddress   Host address of the remote server.
             * \param port          The port to use for the connection to the remote server.
             * \param workingDir    Local working directory for the remote visualization.
             */
            RemoteVisualizationConstructionPlan(const std::string& modelFile, const std::string& path,
                                                const std::string& hostAddress, const int port,
                                                const std::string& wDir);

            virtual ~RemoteVisualizationConstructionPlan() = default;

            RemoteVisualizationConstructionPlan(const RemoteVisualizationConstructionPlan&) = default;
            RemoteVisualizationConstructionPlan& operator=(const RemoteVisualizationConstructionPlan&) = default;

            /*-----------------------------------------
             * Members
             *---------------------------------------*/

            /*! The IP address of the server. */
            std::string hostAddress;
            /*! The port to use for the connection. */
            int port;
            /*! The local working directory. */
            std::string wDir;
        };

    }  // End namespace Initialization
}  // End namespace OMVIS

#endif /* INCLUDE_INITIALIZATION_VISUALIZATIONCONSTRUCTIONPLANS_HPP_ */
/**
 * @}
 */
