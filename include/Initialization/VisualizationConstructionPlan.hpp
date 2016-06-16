/*
 * VisualizationConstructionPlan.hpp
 *
 *  Created on: 06.06.2016
 *      Author: mf
 */

/** @addtogroup Initialization
 *  @{
 *  \copyright TU Dresden. All rights reserved.
 *  \authors Volker Waurich, Martin Flehmig
 *  \date Jun 2016
 */

#ifndef INCLUDE_INITIALIZATION_VISUALIZATIONCONSTRUCTIONPLAN_HPP_
#define INCLUDE_INITIALIZATION_VISUALIZATIONCONSTRUCTIONPLAN_HPP_

#include <string>

namespace OMVIS
{
    namespace Initialization
    {
        /*! \brief This class represents a construction plan for local visualization.
         *
         * This class holds the necessary information to let the factory create a OMVisualizer
         * object. The computation and visualization is done on the localhost.
         *
         * A construction plan is fully specified by the name of the FMU or MAT file. The name
         * includes the path to the file and the prefix (.fmu or .mat).
         */
        struct VisualizationConstructionPlan
        {
//            /*! \brief This method checks, if the construction plan is valid.
//             *
//             * A construction plan is valid, if all attributes are
//             * @return True, if the plan is valid.
//             */
//            bool isValid() const;

            /*! Name of the model file without path but with prefix, e.g., modelFoo.fmu . */
            std::string modelFile;
            /*! Path to the model file, e.g., /home/user/models/ . */
            std::string path;
            bool isFMU;
        };

        struct RemoteVisualizationConstructionPlan
        {
            /*! \brief Constructs a construction plan for remote visualization from the given arguments. */
            //?RemoteVisualizationConstructionPlan(std::string& ipAddress, int portNumber, std::string& modelFile, std::string workingDirectory);

//?            /*! \brief This method checks, if the construction plan is valid.
//             *
//             * A construction plan is valid, if all attributes are
//             * @return True, if the plan is valid.
//             */
//            bool isValid() const;

            /*! The IP address of the server. */
            std::string ipAddress;
            /*! The port to use for the connection. */
            int portNumber;
            /*! Name of the model file (without path). */
            std::string modelFile;
            /*! Path to the model file on server, e.g., /scratch/project/models/ . */
            std::string path;
            /*! The local working directory. */
            std::string workingDirectory;
            bool isFMU;
        };

    }  // End namespace Initialization
}  // End namespace OMVIS

#endif /* INCLUDE_INITIALIZATION_VISUALIZATIONCONSTRUCTIONPLAN_HPP_ */
/**
 * @}
 */
