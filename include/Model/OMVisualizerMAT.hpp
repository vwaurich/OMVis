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

            /*! \brief This method updates the visualization attributes after a time step has been performed.
             *
             * The method updates the actual data for the visualization bodies by using variables from the MAT file.
             *
             * \param time The visualization time.
             * \return Error value.
             */
            int updateVisAttributes(const double time) override;

            /*! \brief For MAT file based visualization, nothing has to be done. Just get the visualizationAttributes.
             */
            void updateScene(const double time);

            /*! \brief Update the attribute of the Object using a mat-file result. */
            void updateObjectAttributeMAT(Model::ShapeObjectAttribute* attr, double time, ModelicaMatReader reader);

            /*! \brief Fetches the value of a variable at a certain time.
             *
             * \todo: The method omc_matlab4_val which is called inside this method returns 0 on success. Thus, we
             *        should test the return value.
             *
             * \param reader The ModelicaMatReader object to use for reading the MAT file.
             * \param varName The name of the variable to fetch the value for.
             * \param time The time to get the value for.
             * \return
             */
            double* omc_get_varValue(ModelicaMatReader* reader, const char* varName, double time);

            /*-----------------------------------------
             * GETTERS AND SETTERS
             *---------------------------------------*/

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
