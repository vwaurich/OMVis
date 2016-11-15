/*
 * Copyright (C) 2016, Volker Waurich
 *
 * This file is part of OMVIS.
 *
 * OMVIS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OMVIS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OMVIS.  If not, see <http://www.gnu.org/licenses/>.
 */

/** \addtogroup Model
 *  \{
 *  \copyright TU Dresden. All rights reserved.
 *  \authors Volker Waurich, Martin Flehmig
 *  \date Feb 2016
 */

#ifndef INCLUDE_VISUALIZERMAT_HPP_
#define INCLUDE_VISUALIZERMAT_HPP_

#include "Model/VisualizerAbstract.hpp"
#include <read_matlab4.h>

namespace OMVIS
{
    namespace Model
    {

        /*! \brief Class that reads results in MAT file format.
         *
         *
         */
        class VisualizerMAT : public VisualizerAbstract
        {
         public:
            /*-----------------------------------------
             * CONSTRUCTORS
             *---------------------------------------*/

            VisualizerMAT() = delete;

            /*! \brief Constructs a VisualizerMAT object from the given arguments.
             *
             * Essentially a MAT file and its path need to be specified.
             *
             * \param modelFile  Model file name without path.
             * \param path       Path to the model file.
             */
            VisualizerMAT(const std::string& modelFile, const std::string& path);

            virtual ~VisualizerMAT() = default;

            VisualizerMAT(const VisualizerMAT& rhs) = delete;

            VisualizerMAT& operator=(const VisualizerMAT& rhs) = delete;

            /*-----------------------------------------
             * INITIALIZATION METHODS
             *---------------------------------------*/

            void setSimulationSettings(const UserSimSettingsMAT& simSetMAT);

         private:
            /*-----------------------------------------
             * MEMBERS
             *---------------------------------------*/

            ModelicaMatReader _matReader;

            /*-----------------------------------------
             * PRIVATE METHODS
             *---------------------------------------*/

            void initData() override;

            /*! \brief Initializes the visualization attributes in order to set the scene to the initial position. */
            void initializeVisAttributes(const double time = -1.0) override;

            void readMat(const std::string& modelFile, const std::string& path);

            /*-----------------------------------------
             * SIMULATION METHODS
             *---------------------------------------*/

            void simulate(Control::TimeManager& omvm) override
            {
            }

            /*! \brief This method updates the visualization attributes after a time step has been performed.
             *
             * The method updates the actual data for the visualization bodies by using variables from the MAT file.
             *
             * \param time  The visualization time.
             */
            void updateVisAttributes(const double time) override;

            /*! \brief For MAT file based visualization, nothing has to be done. Just get the visualizationAttributes. */
            void updateScene(const double time) override;

            /*! \brief Update the attribute of the Object using a MAT result file. */
            void updateObjectAttributeMAT(Model::ShapeObjectAttribute* attr, double time, ModelicaMatReader* reader);

            /*! \brief Fetches the value of a variable at a certain time.
             *
             * \todo The method omc_matlab4_val which is called inside this method returns 0 on success. Thus, we
             *       should test the return value.
             *
             * \param reader    The ModelicaMatReader object to use for reading the MAT file.
             * \param varName   The name of the variable to fetch the value for.
             * \param time      The time to get the value for.
             * \return Value of the variable at the specified time.
             */
            double omcGetVarValue(ModelicaMatReader* reader, const char* varName, double time);
        };

    }  // namespace Model
}  // namespace OMVIS

#endif /* INCLUDE_VISUALIZERMAT_HPP_ */
/**
 * \}
 */
