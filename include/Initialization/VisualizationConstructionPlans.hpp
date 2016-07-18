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

#include <string>

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

            virtual ~VisualizationConstructionPlan() = default;

            /*! \brief This method checks, if the construction plan is valid.
             *
             * A construction plan is valid, if all attributes are set, path and file name are not empty.
             * \return True, if the plan is valid.
             */
            virtual bool isValid() const
            {
                return (visType != VisualizationType::NONE);
            }
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

            virtual ~RemoteVisualizationConstructionPlan() = default;
//            /*! \brief This method checks, if the construction plan is valid.
//             *
//             * A construction plan is valid, if all attributes are
//             * @return True, if the plan is valid.
//             */
//            bool isValid() const override
//            {
//                return VisualizationConstructionPlan::isValid();
//            }

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
