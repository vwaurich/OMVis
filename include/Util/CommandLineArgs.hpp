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

/** @addtogroup Util
 *  @{
 *  \copyright TU Dresden. All rights reserved.
 *  \authors Volker Waurich, Martin Flehmig
 *  \date Feb 2016
 */

#ifndef INCLUDE_UTIL_COMMANDLINEARGS_HPP_
#define INCLUDE_UTIL_COMMANDLINEARGS_HPP_

#include <string>
#include <map>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include "Util/Logger.hpp"
#include "Util/Util.hpp"

namespace Util
{

    /*! \brief This class holds the settings specified by the user via command line.
     * \Remark: This is not longer supported.
     * \todo: Volker, do we want to remove this code from source?
     */
    class CommandLineArgs
    {
     public:
        /// Default constructor. Attributes are initialized with default values;
        CommandLineArgs()
                : _modelName(""),
                  _modelPath(""),
                  _useFMU(false),
                  _logSettings()
        {
        }

        ~CommandLineArgs() = default;
        CommandLineArgs(const CommandLineArgs& cla) = default;
        CommandLineArgs& operator=(const CommandLineArgs& cla) = delete;

        /// Outputs command line arguments to screen.
		void print();

     public:
        /// Name of the file containing the model to visualize.
        std::string _modelName;
        /// Path to the model file.
        std::string _modelPath;
        bool _useFMU;
        Util::LogSettings _logSettings;
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


}  // End namespace Util

#endif /* INCLUDE_UTIL_COMMANDLINEARGS_HPP_ */
/**
 * @}
 */
