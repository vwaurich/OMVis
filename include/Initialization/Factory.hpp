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

#ifndef INCLUDE_FACTORY_HPP_
#define INCLUDE_FACTORY_HPP_

#include <string>

// Forward declarations
namespace Model
{
    class OMVisualizerAbstract;
}
namespace Util
{
    class CommandLineArgs;
}
namespace Control
{
    class OMVisManager;
}

namespace Initialization
{
    /*! \brief This factory class can create OMVisualization objects that are required for an FMU or result file based visualization.
     *
     */
    class Factory
    {
     public:
        Factory() = default;

        /*! \brief Create Factory object from  parsed command line arguments.
         *
         * @param cLArgs Command line arguments.
         */
        //Factory(Util::CommandLineArgs& cLArgs);
        ~Factory() = default;
        Factory(const Factory& f) = delete;
        Factory& operator=(const Factory& f) = delete;

        /*! \brief Creates concrete abstract visualization object.
         *
         * This method is used, if the model is not configured via command line argument, but will be selected via
         * File-open dialog in the GUI.
         *
         * @return Visualization object of type OMVisualizationAbstract.
         */
        //Model::OMVisualizerAbstract* createVisualization();
        /*! \brief Creates concrete Visualization object according to the command line arguments.
         *
         * @param cLArgs The parsed command line arguments.
         * @return Visualization object of type OMVisualizationMAT or OMVisualizationFMU.
         */
        Model::OMVisualizerAbstract* createVisualizationFromCLargs(const Util::CommandLineArgs& cLArgs);

        /*! \brief Creates new visualization object accordingt to the given command line parameters.
         *
         * According to the command line parameters, a OMVisualizerFMU or OMVisualizerMAT object ist created an a pointer to this
         * object is returned. If the command line is empty, i.e., only OMVIS is called, than we create a object of base class
         * OMVisualizerAbstract and return a appropriate pointer.
         *
         * @param modelName
         * @param pathName
         * @param useFMU
         * @return
         */
        Model::OMVisualizerAbstract* createVisualizationObject(const std::string modelName = "", const std::string pathName = "", const bool useFMU = false);
    };

}  // End namespace Initialization

#endif /* INCLUDE_FACTORY_HPP_ */
/**
 * @}
 */
