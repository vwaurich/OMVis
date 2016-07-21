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

#include <string>
#include <stdexcept>

namespace OMVIS
{

    /*! This enum holds the possible visualization modes of OMVis. */
    enum VisualizationType
    {
        NONE = 0,
        FMU = 1,
        FMU_REMOTE = 2,
        MAT = 3,
        MAT_REMOTE = 4
    };

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

            /*! \brief The default constructor for VisualizationConstructionPlan is forbidden.
             *
             * The rationale is: If we provide a default constructor, than we need to provide setter methods for the
             * members and a valid method, that needs to be called from the user code in order to check for validity of
             * a construction plan. This sounds error prone. Thus, we just provide the constructor with parameters,
             * which throws, if the parameters are not valid, to construct a fully specified and valid object of this
             * class.
             */
            VisualizationConstructionPlan() = delete;

            /*! \brief Constructor from arguments.
             *
             * This constructors throws std::invalid_argument exceptions, if the object can not be constructed. This
             * arises, if the model file name or the path is empty or the model tpe (FMU or MAT) cannot determined
             * (no prefix at model file name).
             *
             * \param modelFileIn   Name of the model file with prefix, e.g., ModelX.fmu
             * \param pathIn        Path to the model file.
             */
            VisualizationConstructionPlan(const std::string& modelFileIn, const std::string& pathIn)
                    : modelFile(modelFileIn),
                      path(pathIn)
            {
                if (modelFileIn.empty())
                    throw std::invalid_argument("No model file specified.");
                if (pathIn.empty())
                    throw std::invalid_argument("No path specified.");

                // Get visualization type.
                if (Util::isFMU(modelFile))
                    visType = VisualizationType::FMU;
                else if (Util::isMAT(modelFile))
                    visType = VisualizationType::MAT;
                else
                    throw std::invalid_argument("VisualizationType is NONE.");
            }

            virtual ~VisualizationConstructionPlan() = default;

            VisualizationConstructionPlan(const VisualizationConstructionPlan&) = default;
            VisualizationConstructionPlan& operator= (const VisualizationConstructionPlan&) = default;

            /*-----------------------------------------
             * Members
             *---------------------------------------*/

            VisualizationType visType;
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

            /*!
             * The default constructor is deleted for the very same reason as the default constructor for
             * \ref VisualizationConstructionPlan is deleted.
             */
            RemoteVisualizationConstructionPlan() = delete;

            /*! \brief Constructor from arguments.
             *
             * This constructors throws std::invalid_argument exceptions, if the object can not be constructed. This
             * arises, if
             *      - the model file name or the path is empty
             *      - the model tpe (FMU or MAT) cannot determined
             *      - the IP address is not valid.
             *
             * \param modelFileIn   Name of the model file with prefix, e.g., ModelX.fmu
             * \param pathIn        Path to the model file.
             * \param ipAddressIn   IP address of the remote server.
             * \param portNumberIn  The port number to use for the connection to the remote server.
             * \param workingDirIn  Local working directory for the remote visualization.
             */
            RemoteVisualizationConstructionPlan(const std::string& modelFileIn, const std::string& pathIn,
                                                const std::string& ipAddressIn, const int portNumberIn,
                                                const std::string& workingDirIn)
                    : VisualizationConstructionPlan(modelFileIn, pathIn),
                      ipAddress(ipAddressIn),
                      portNumber(portNumberIn),
                      workingDirectory(workingDirIn)
            {
                if (!(Util::isValidIPv4(ipAddressIn) || Util::isValidIPv6(ipAddressIn)))
                    throw std::invalid_argument("Invalid IP address.");
                if (workingDirIn.empty())
                    throw std::invalid_argument("No local working directory specified.");
            }

            virtual ~RemoteVisualizationConstructionPlan() = default;

            RemoteVisualizationConstructionPlan(const RemoteVisualizationConstructionPlan&) = default;
            RemoteVisualizationConstructionPlan& operator= (const RemoteVisualizationConstructionPlan&) = default;

            /*-----------------------------------------
             * Members
             *---------------------------------------*/

            /*! The IP address of the server. */
            std::string ipAddress;
            /*! The port to use for the connection. */
            int portNumber;
            /*! The local working directory. */
            std::string workingDirectory;
        };

    }  // End namespace Initialization
}  // End namespace OMVIS

#endif /* INCLUDE_INITIALIZATION_VISUALIZATIONCONSTRUCTIONPLANS_HPP_ */
/**
 * @}
 */
