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

#include "Model/VisualizerAbstract.hpp"
#include "Initialization/VisualizationConstructionPlans.hpp"

#include <memory>

namespace OMVIS
{
    namespace Initialization
    {

        /*! \brief The factory creates concrete OMVisualizer objects for model visualization from a given construction plan.
         *
         * In order to let the factory create a OMVisualizer object for the visualization of a FMU or MAT file or for
         * remote FMU visualization, just call the method \ref createVisualizationObject with an appropriate
         * construction plan. See \ref VisualizationConstructionPlan and \ref RemoteVisualizationConstructionPlan for
         * more information about construction plans.
         */
        class Factory
        {
         public:
            /*-----------------------------------------
             * CONSTRUCTORS
             *---------------------------------------*/

            /*! \brief Default constructor to create a Factory object.
             *
             * Since this class has no members, we can delegate the implementation to the compiler.
             */
            Factory() = default;

            ~Factory() = default;
            Factory(const Factory& f) = delete;
            Factory& operator=(const Factory& f) = delete;

            /*-----------------------------------------
             * CREATE METHODS
             *---------------------------------------*/

            /*! \brief Creates a new visualization object according to the given construction plan.
             *
             * According to the given construction plan, a OMVisualizerFMU, OMVisualizerMAT or OMVisualizerFMUClient
             * object is created and a pointer to this object is returned. If the construction fails, a nullptr is
             * returned. The caller method is responsible to handle this.
             * The construction plan can be of type \ref VisualizationConstructionPlan or
             * \ref RemoteVisualizationConstructionPlan for remote visualization.
             *
             * \remark This method checks for the presence of the visual XML file. If the XML file is not present, an
             *         exception of type std::runtime_error is thrown.
             *
             * \param   cP Construction plan for visualization.
             * \return  Pointer to allocated OMVisualizer object or nullptr if OMVisualizer object cannot be created.
             */
            std::shared_ptr<Model::VisualizerAbstract> createVisualizerObject(const VisualizationConstructionPlan* cP);
        };

    }  // End namespace Initialization
}  // End namespace OMVIS

#endif /* INCLUDE_FACTORY_HPP_ */
/**
 * @}
 */
