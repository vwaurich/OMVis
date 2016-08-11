/*
 * VisualizationTyps.hpp
 *
 *  Created on: 04.08.2016
 *      Author: mf
 */

#ifndef INCLUDE_MODEL_VISUALIZATIONTYPES_HPP_
#define INCLUDE_MODEL_VISUALIZATIONTYPES_HPP_

namespace OMVIS
{
    namespace Model
    {

        /*! This enum class holds the possible visualization modes of OMVis. */
        enum class VisType
        {
            NONE = 0,
            FMU = 1,
            FMU_REMOTE = 2,
            MAT = 3,
            MAT_REMOTE = 4
        };

    }  // End namespace Model
}  // End namespace OMVIS

#endif /* INCLUDE_MODEL_VISUALIZATIONTYPES_HPP_ */
