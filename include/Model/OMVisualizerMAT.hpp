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

/** @addtogroup Model
 *  @{
 *  \copyright TU Dresden. All rights reserved.
 *  \authors Volker Waurich, Martin Flehmig
 *  \date Feb 2016
 */

#ifndef INCLUDE_OMVISUALIZERMAT_HPP_
#define INCLUDE_OMVISUALIZERMAT_HPP_

#include "OMVisualizerAbstract.hpp"
#include "Visualize.hpp"

#include <read_matlab4.h>

namespace OMVIS
{
    namespace Model
    {

        /*! \brief Class that reads results in MAT file format.
         *
         *
         */
        class OMVisualizerMAT : public OMVisualizerAbstract
        {
         public:
            /*-----------------------------------------
             * CONSTRUCTORS
             *---------------------------------------*/

            OMVisualizerMAT() = delete;

            /*! \brief Constructs a OMVisualizerMAT object from the given arguments.
             *
             * Essentially, a MAT file and its path need to be specified.
             *
             * @param[in] fileName Model file name without path.
             * @param[in] path Path to the model file.
             */
            OMVisualizerMAT(const std::string& fileName, const std::string& path);

            virtual ~OMVisualizerMAT() = default;

            OMVisualizerMAT(const OMVisualizerMAT& omvm) = delete;
            OMVisualizerMAT& operator=(const OMVisualizerMAT& omvm) = delete;

            /*-----------------------------------------
             * INITIALIZATION METHODS
             *---------------------------------------*/

            /*! \brief
             *
             * @return Error value.
             */
            virtual int initData();

            /*! \brief Implementation for OMVisualizerAbstract::initializeVisAttributes to set the scene to initial position.
             */
            void initializeVisAttributes(const double time = -1.0);

            void readMat(const std::string& modelFile, const std::string& path);

            /*-----------------------------------------
             * SIMULATION METHODS
             *---------------------------------------*/

            virtual void simulate(Control::OMVisManager& omvm)
            {
            }

            /*! \brief Implementation for OMVisualizerAbstract::updateVisAttributes to update all visualization attributes based on a MAT file.
             *
             * @param time The point of time
             * \return Error value.
             */
            int updateVisAttributes(const double time);

            /*! \brief For MAT file based visualization, nothing has to be done. Just get the visualizationAttributes.
             */
            void updateScene(const double time);

            /*! \brief Returns "mat".
             */
            std::string getType() const;

         private:
            /*-----------------------------------------
             * MEMBERS
             *---------------------------------------*/

            ModelicaMatReader _matReader;
        };

    }  // End namespace Model
}  // End namespace OMVIS

#endif /* INCLUDE_OMVISUALIZERMAT_HPP_ */
/**
 * @}
 */
