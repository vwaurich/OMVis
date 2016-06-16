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

#include "Initialization/VisualizationConstructionPlan.hpp"
#include "Model/OMVisualizerAbstract.hpp"

#include <memory>

namespace OMVIS
{
    namespace Initialization
    {

        /*! \brief This factory class can create OMVisualization objects that are required for an FMU or result file
         *         (,i.e., MAT file) based visualization.
         *
         * In order to let the factory create a OMVisualizer object for the visualization of a FMU or MAT file, just
         * call the method \ref createVisualizationObject with a proper construction plan.
         */
        class Factory
        {
         public:
            /*-----------------------------------------
             * CONSTRUCTORS
             *---------------------------------------*/

            Factory() = default;
            ~Factory() = default;
            Factory(const Factory& f) = delete;
            Factory& operator=(const Factory& f) = delete;

            /*-----------------------------------------
             * CREATE METHODS
             *---------------------------------------*/

            /*! \brief Creates a new visualization object according to the given construction plan.
             *
             * A OMVisualizerFMU or OMVisualizerMAT object is created and a pointer to this object is returned. If the
             * construction fails, a nullptr is returned.
             *
             * \remark This method checks for the presence of the visual XML file. If the XML file is not present, an
             *         exception is thrown.
             *
             * @param[in] cP Construction plan for local visualization.
             * @return Pointer to allocated OMVisualizer object.
             */
            std::shared_ptr<Model::OMVisualizerAbstract> createVisualizationObject(const VisualizationConstructionPlan& cP);

            /*! \brief Creates a new visualization object according to the given construction plan for remote
             *         visualization.
             *
             * A OMVisualizerFMUClient or OMVisualizerMATClient object is created and a pointer to this object is
             * returned. If the construction fails, a nullptr is returned.
             *
             * \remark This method checks for the presence of the visual XML file in the local working directory
             * (\see RemoteVisualizationConstructionPlan). If the XML file is not present, an exception is thrown.
             *
             * @param[in] cP Construction plan for remote visualization.
             * @return Pointer to allocated OMVisualizer object.
             */
            std::shared_ptr<Model::OMVisualizerAbstract> createVisualizationObject(const RemoteVisualizationConstructionPlan& cP);
        };

    }  // End namespace Initialization
}  // End namespace OMVIS

#endif /* INCLUDE_FACTORY_HPP_ */
/**
 * @}
 */
