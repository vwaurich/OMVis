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


#include <iostream>
#include "Util/CommandLineArgs.hpp"

namespace Util
{

	CommandLineArgs getCommandLineArguments(int argc, char *argv[]) {

		std::map<std::string, Util::LogCategory> logcatmap;
		logcatmap["loader"] = Util::LC_LOADER;
		logcatmap["controller"] = Util::LC_CTR;
		logcatmap["viewer"] = Util::LC_VIEWER;
		logcatmap["solver"] = Util::LC_SOLVER;
		logcatmap["other"] = Util::LC_OTHER;

		std::map<std::string, Util::LogLevel> loglvlmap;
		loglvlmap["error"] = Util::LL_ERROR;
		loglvlmap["warning"] = Util::LL_WARNING;
		loglvlmap["info"] = Util::LL_INFO;
		loglvlmap["debug"] = Util::LL_DEBUG;

		namespace po = boost::program_options;
		po::options_description desc("Options");
		desc.add_options()("help", "Prints help message.")
			("path", boost::program_options::value<std::string>(),
				"Path where the model file is stored in.")
			("model,M", boost::program_options::value<std::string>(),
				"Name of the model file which should be visualized.")
			("useFMU,U", boost::program_options::bool_switch()->default_value(false),
				"OMVIS uses a FMU if specified for visualization.")
			("loggerSettings,l", po::value<std::vector<std::string> >(),
				"Specification of the logging information.\n"
				"Available categories: loader, controller, viewer, solver, other.\n"
				"Available levels: error, warning, info, debug.\n"
				"Hint: Different settings of logger category and level can be specified "
				"separately to allow fine grained control, e.g., -l=\"loader=error\" -l=\"viewer=info\" ");

		po::variables_map vm;
		CommandLineArgs result;
		try
		{
			po::store(po::parse_command_line(argc, argv, desc), vm);

			if (vm.count("help"))
			{
				std::cout << "Basic Command Line Parameter for OMVIS." << std::endl << desc << std::endl;
				exit(0);
				return result;
			}

			if (vm.count("path"))
				result._modelPath = vm["path"].as<std::string>();
//			else
//				throw std::runtime_error("Path to visualization model required, but missing.");

			if (vm.count("model"))
				result._modelName = vm["model"].as<std::string>();
//			else
//				throw std::runtime_error("Model file required, but missing.");

			//Set value of useFMU
			result._useFMU = vm["useFMU"].as<bool>();

			Util::LogSettings logSet;
			if (vm.count("loggerSettings"))
			{
				std::vector<std::string> logVec = vm["loggerSettings"].as<std::vector<std::string> >();
				std::vector<std::string> tmpvec;
				for (unsigned i = 0; i < logVec.size(); ++i)
				{
					tmpvec.clear();
					boost::split(tmpvec, logVec[i], boost::is_any_of("="));
					//std::cout << i << ". " << tmpvec[0] << "\t" << tmpvec[1] << std::endl;
					if (tmpvec.size() > 1 && loglvlmap.find(tmpvec[1]) != loglvlmap.end() && (tmpvec[0] == "all" || logcatmap.find(tmpvec[0]) != logcatmap.end()))
					{
						if (tmpvec[0] == "all")
						{
							//std::cout << "All Logger Categories are set to level " << tmpvec[1] << std::endl;
							logSet.setAll(loglvlmap[tmpvec[1]]);
							break;
						}
						else
						{
							//std::cout << "Logger Category " << tmpvec[0] << " is set to level " << tmpvec[1] << std::endl;
							logSet.modes[logcatmap[tmpvec[0]]] = loglvlmap[tmpvec[1]];
						}
					}
					else
						throw std::runtime_error("loggerSettings flags not supported: " + logVec[i] + "\n");
				}
				result._logSettings = logSet;
			}
		}
		catch (po::error& e)
		{
			throw std::runtime_error((std::string("Cannot parse command line arguments. ") + std::string(e.what())).c_str());
		}

		return result;
	}

	void CommandLineArgs::print()
	{
		std::cout << "\nHei, the command line arguments for OMVIS are as follows:" << std::endl;
		std::cout << "  Model: " << _modelName << std::endl;
		std::cout << "  Use FMU: " << Util::boolToString(_useFMU) << std::endl;
		_logSettings.print();
	}

} /* namespace Util */
