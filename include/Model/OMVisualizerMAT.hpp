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

namespace Model
{

    /*! \brief Class that stores results in matlab file format.
     *
     *
     */
    class OMVisualizerMAT : public OMVisualizerAbstract
    {
     public:
        OMVisualizerMAT() = delete;

        OMVisualizerMAT(const std::string model, const std::string path);

        virtual ~OMVisualizerMAT() = default;

        OMVisualizerMAT(const OMVisualizerMAT& omvm) = delete;
        OMVisualizerMAT& operator=(const OMVisualizerMAT& omvm) = delete;

        void readMat(const std::string modelString, const std::string dirString);

        virtual void initData();

        virtual void simulate(Control::OMVisManager& omvm) { }

		/*! \brief Implementation for OMVisualizerAbstract::updateVisAttributes to update all visualization attributes based on a mat file
		*
		* @param time The point of time
		*/
		void updateVisAttributes(const double time);

		/*! \brief Implementation for OMVisualizerAbstract::initializeVisAttributes to set the scene to initial position.
		*/
		void initializeVisAttributes(const double time);

		/*! \brief For MAT-file based visualization, nothing has to be done. Just get the visualizationAttributes
		*/
		void updateScene(const double time);

		/*! \brief Returns a 0 if we use mat-failes, 1 if we use fmus.
		*/
		int getDataTypeID();

     private:
        ModelicaMatReader _matReader;
    };

}  // End namepsace Model

#endif /* INCLUDE_OMVISUALIZERMAT_HPP_ */
/**
 * @}
 */
