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
 *  \date Feb 2016
 */

#ifndef INCLUDE_COMMANDLINEARGS_HPP_
#define INCLUDE_COMMANDLINEARGS_HPP_

#include "Util/Logger.hpp"
#include "Util/Util.hpp"
#include "Initialization/VisualizationConstructionPlans.hpp"

#include <boost/program_options.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <string>
#include <map>

namespace OMVIS
{
    namespace Initialization
    {

        /*! \brief This class holds the settings specified by the user via command line.
         * \Remark: This is not longer supported.
         * \todo: Volker, do we want to remove this code from source?
         */
        class CommandLineArgs
        {
         public:
            /*-----------------------------------------
             * CONSTRUCTORS
             *---------------------------------------*/

            /// Default constructor. Attributes are initialized with default values;
            CommandLineArgs();

            ~CommandLineArgs() = default;

            CommandLineArgs(const CommandLineArgs& rhs) = default;

            CommandLineArgs& operator=(const CommandLineArgs& rhs) = delete;

            /*-----------------------------------------
             * GETTERS AND SETTERS
             *---------------------------------------*/

            /*!
             * @return
             */
            VisualizationConstructionPlan getVisualizationConstructionPlan() const;

            RemoteVisualizationConstructionPlan getRemoteVisualizationConstructionPlan() const;

            /*! \brief Checks, if the command line arguments specify a local visualization.
             *
             * Returns true, if a valid local visualization is specified via the command line arguments, i.e., a model
             * file is specified and a path to the model file is given. Furthermore, it is checked that the model file
             * exists. If a host address and a port are specified, it is also not a local visualization.
             */
            bool localVisualization() const;

            /*! \brief Checks, if the command line arguments specify a remote visualization.
             * Returns true, if a valid remote visualization is specified via the command line arguments, i.e., all
             * necessary arguments for remote visualization are given.
             */
            bool remoteVisualization() const;

            /*! \brief Checks, if the command line is empty.
             *
             * We assume the command line is empty, if no model file or no path is given. If both, model file and path,
             * are specified, this method returns false.
             */
            bool empty() const;

            /*-----------------------------------------
             * PRINT METHODS
             *---------------------------------------*/

            /// Outputs command line arguments to screen.
            void print();

         public:
            /*-----------------------------------------
             * MEMBERS
             *---------------------------------------*/

            std::string hostAddress;
            int port;
            std::string modelFile;
            std::string modelPath;
            std::string wDir;
            Util::LogSettings logSet;
        };

        /*! \brief This method parses the command line arguments for visualization settings.
         *
         * Command line arguments to set up a visualization:
         *      --help                          Prints help message.
         *      --model=/PATH/TO/MODELNAME      Path (absolute or relative) to the model which should be visualized.
         *      --useFMU                        OMVIS uses a FMU if specified for visualization.
         *      --loggersettings="loader=warning"
         *
         * @param argc
         * @param argv
         * @return Returns the parsed arguments.
         */
        CommandLineArgs getCommandLineArguments(int argc, char *argv[]);

        void checkCommandLineArguments(const CommandLineArgs& clArgs);

    }  // End namespace Initialization
}  // End namespace OMVIS

#endif /* INCLUDE_COMMANDLINEARGS_HPP_ */
/**
 * @}
 */
