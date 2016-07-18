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

/** @addtogroup View
 *  @{
 *  \copyright TU Dresden. All rights reserved.
 *  \authors Volker Waurich, Martin Flehmig
 *  \date Feb 2016
 */

#ifndef INCLUDE_OMVISSCENE_HPP_
#define INCLUDE_OMVISSCENE_HPP_

#include "Model/OSGScene.hpp"

namespace OMVIS
{
    namespace Model
    {

        /*! \brief Base class that contains xml infos and viewer.
         *
         * \todo: privatize members
         */
        class OMVisScene
        {
         public:
            OMVisScene();
            ~OMVisScene() = default;

            OMVisScene(const OMVisScene& omvv) = delete;
            OMVisScene& operator=(const OMVisScene& omvv) = delete;

            /*! \brief This method dumps a string representation of the scene graph to stdOut.
             *
             */
            void dumpOSGTreeDebug();

            OSGScene& getScene();

         private:
            OSGScene _scene;
        };

    }  // End namespace Model
}  // End namespace OMVIS

#endif /* INCLUDE_OMVISSCENE_HPP_ */
/**
 * @}
 */
