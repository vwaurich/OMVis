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

/*
 * Factory.hpp
 *
 *  Created on: 18.02.2016
 *      Author: mf
 */

#ifndef INCLUDE_INITIALIZATION_FACTORY_HPP_
#define INCLUDE_INITIALIZATION_FACTORY_HPP_

#include <string>

// Forward declarations
namespace Model{
    class OMVisualizerAbstract;
}
namespace Util{
    class CommandLineArgs;
}
namespace View{
    class OMVManager;
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

        Model::OMVisualizerAbstract* createVisualizationFromCLargs(const Util::CommandLineArgs& cLArgs);

		Model::OMVisualizerAbstract* createVisualizationObjects(std::string modelName, std::string pathName, bool useFMU);

//     private:
//        bool _useFMU;
//        std::string _modelName;
//        std::string _modelPath;
    };

}  // End namespace Initialization

#endif /* INCLUDE_INITIALIZATION_FACTORY_HPP_ */
