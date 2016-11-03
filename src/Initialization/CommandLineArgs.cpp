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

#include "Initialization/CommandLineArgs.hpp"

#include <iostream>

using std::cout;
using std::endl;

namespace OMVIS
{
    namespace Initialization
    {

        /*-----------------------------------------
         * CONSTRUCTORS
         *---------------------------------------*/

        CommandLineArgs::CommandLineArgs()
                : hostAddress(),
                  port(-1),
                  modelFile(),
                  modelPath(),
                  wDir(),
                  logSet()
        {
        }

        /*-----------------------------------------
         * GETTERS AND SETTERS
         *---------------------------------------*/

        VisualizationConstructionPlan CommandLineArgs::getVisualizationConstructionPlan() const
        {
            return VisualizationConstructionPlan(modelFile, modelPath);
        }

        RemoteVisualizationConstructionPlan CommandLineArgs::getRemoteVisualizationConstructionPlan() const
        {
            return RemoteVisualizationConstructionPlan(modelFile, modelPath, hostAddress, port, wDir);
        }

        bool CommandLineArgs::localVisualization() const
        {
            if (modelFile.empty() || modelPath.empty())
            {
                return false;
            }

            // Remote Visualization!
            if (1 < hostAddress.size() && -1 != port)
            {
                return false;
            }

            return (Util::fileExists(modelPath + modelFile));
        }

        bool CommandLineArgs::remoteVisualization() const
        {
            return !(modelFile.empty() || modelPath.empty() || hostAddress.empty() || 1 > port || wDir.empty());
        }

        bool CommandLineArgs::empty() const
        {
            return (modelFile.empty() || modelPath.empty());
        }

        /*-----------------------------------------
         * PRINT MEHTODS
         *---------------------------------------*/

        void CommandLineArgs::print()
        {
            if (!empty())
            {
                cout << "\nHei, the command line arguments for OMVis are as follows:" << endl;
                cout << "  Host: " << hostAddress << endl;
                cout << "  Port: " << port << endl;
                cout << "  Model File: " << modelFile << endl;
                cout << "  Model Path: " << modelPath << endl;
                cout << "  Working Directory: " << wDir << endl;
                logSet.print();
            }
        }

        CommandLineArgs getCommandLineArguments(int argc, char *argv[])
        {
            CommandLineArgs result;

            Util::LogSettings logSet;
#ifndef NDEBUG
            logSet.setAll(Util::LogLevel::LL_DEBUG);
#else
            logSet.setAll(Util::LogLevel::LL_WARNING);
#endif
            result.logSet = logSet;

            if (1 < argc)
            {
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
                desc.add_options()("help,h", "Prints help message.")("path",
                                                                     boost::program_options::value<std::string>(),
                                                                     "Path where the model file is stored in.")(
                        "model", boost::program_options::value<std::string>(),
                        "Name of the model file which should be visualized.")(
                        "remoteSimulation", boost::program_options::value<std::string>(),
                        "Start a remote visualization.")(
                        "host", boost::program_options::value<std::string>(),
                        "Name or IP address of the host server for remote visualization.")(
                        "port", boost::program_options::value<int>(), "Port to use for remote visualization.")(
                        "wdir", boost::program_options::value<std::string>(),
                        "Local working directory for remote visualization.")(
                        "loggerSettings,l", po::value<std::vector<std::string> >(),
                        "Specification of the logging information.\n"
                        "Available categories: loader, controller, viewer, solver, other.\n"
                        "Available levels: error, warning, info, debug.\n"
                        "Hint: Different settings of logger category and level can be specified "
                        "separately to allow fine grained control, e.g., -l=\"loader=error\" -l=\"viewer=info\".\n"
                        "All categories can be set to the very same level via -l=\"all=LEVEL\".");

                po::variables_map vm;

                try
                {
                    po::store(po::parse_command_line(argc, argv, desc), vm);

                    if (0u != vm.count("help"))
                    {
                        cout << "Usage:" << endl;
                        cout << " Local Visualization: OMVis --model=BouncingBall.fmu --path=/PATH/TO/BOUNCINGBALL/ "
                             << endl;
                        cout << "Remote Visualization: OMVis --host=HOSTNAME --port=PORT --model=BouncingBall.fmu "
                             "--path=/PATH/TO/BOUNCINGBALL/ --wdir=/PATH/TO/WORKINGDIR/"
                             << endl;
                        cout << "\n" << endl;
                        cout << "OMVis - An open source tool for model and simulation visualization" << endl;
                        cout << "\n" << endl;
                        // Command line options
                        cout << desc << endl;
                        exit(0);
                        return result;
                    }

                    // User can overwrite logger settings via command line
                    Util::LogSettings logSet;
                    if (0u != vm.count("loggerSettings"))
                    {
                        std::vector<std::string> logVec = vm["loggerSettings"].as<std::vector<std::string>>();
                        std::vector<std::string> tmpvec;
                        //for (unsigned i = 0; i < logVec.size(); ++i)
                        int i = 0;
                        for (auto& logElem : logVec)
                        {
                            tmpvec.clear();
                            //boost::split(tmpvec, logVec[i], boost::is_any_of("="));
                            boost::split(tmpvec, logElem, boost::is_any_of("="));
                            cout << i << ". " << tmpvec[0] << "\t" << tmpvec[1] << endl;
                            if (tmpvec.size() > 1 && loglvlmap.find(tmpvec[1]) != loglvlmap.end()
                                    && (tmpvec[0] == "all" || logcatmap.find(tmpvec[0]) != logcatmap.end()))
                            {
                                if ("all" == tmpvec[0])
                                {
                                    cout << "All Logger Categories are set to level " << tmpvec[1] << endl;
                                    logSet.setAll(loglvlmap[tmpvec[1]]);
                                    break;
                                }
                                else
                                {
                                    cout << "Logger Category " << tmpvec[0] << " is set to level " << tmpvec[1] << endl;
                                    logSet.modes[logcatmap[tmpvec[0]]] = loglvlmap[tmpvec[1]];
                                }
                            }
                            else
                            {
                                throw std::runtime_error("loggerSettings flags not supported: " + logElem + "\n");
                            }

                            ++i;
                        }
                        result.logSet = logSet;
                    }

                    if (0u != vm.count("host"))
                    {
                        result.hostAddress = vm["host"].as<std::string>();
                    }

                    if (0u != vm.count("port"))
                    {
                        result.port = vm["port"].as<int>();
                    }

                    if (0u != vm.count("model"))
                    {
                        result.modelFile = vm["model"].as<std::string>();
                    }

                    if (0u != vm.count("path"))
                    {
                        result.modelPath = vm["path"].as<std::string>();
                    }

                    if (0u != vm.count("wdir"))
                    {
                        result.wDir = vm["wdir"].as<std::string>();
                    }

                }
                catch (po::error& e)
                {
                    throw std::runtime_error(
                            (std::string("Cannot parse command line arguments. ") + std::string(e.what())).c_str());
                }

                try
                {
                    checkCommandLineArguments(result);
                }
                catch (po::error& e)
                {
                    throw std::runtime_error(
                            (std::string("Error with command lines:") + std::string(e.what())).c_str());
                }
            }

            return result;
        }

        void checkCommandLineArguments(const CommandLineArgs& clArgs)
        {
            // Both, local and remote visualization, need a path and a model file.
            if (clArgs.modelFile.empty())
            {
                throw std::runtime_error("No model file given. Use --model=MODELFILE.");
            }
            else if (clArgs.modelPath.empty())
            {
                throw std::runtime_error("No path to model file given. Use --path=/PATH/TO/MODELFILE/.");
            }
            else
            {
                if (!Util::fileExists(clArgs.modelPath + clArgs.modelFile))
                {
                    throw std::runtime_error("The model file " + clArgs.modelFile + " does not exists.");
                }
            }

            // We assume the user wants remote visualization if one of the following parameter is specified:
            // host, port, wdir.
            bool remoteVis = !clArgs.hostAddress.empty() || -1 < clArgs.port || !clArgs.wDir.empty();
            if (remoteVis)
            {
                if (clArgs.hostAddress.empty())
                {
                    throw std::runtime_error("No host for remote visualization given. Use --host=HOSTNAME.");
                }

                if (1 > clArgs.port)
                {
                    throw std::runtime_error("No port for remote visualization given. Use --port=PORT.");
                }

                if (clArgs.wDir.empty())
                {
                    throw std::runtime_error(
                            "No (local) working directory for remote visualization given. Use --wdir=/PATH/TO/WORKDING/DIR/.");
                }
            }
        }

    }  // namespace Initialization
}  // namespace OMVIS
