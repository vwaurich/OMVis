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

/** \addtogroup View
 *  \{
 *  \copyright TU Dresden. All rights reserved.
 *  \authors Volker Waurich, Martin Flehmig
 *  \date Feb 2016
 */

#ifndef INCLUDE_OMVISSCENE_HPP_
#define INCLUDE_OMVISSCENE_HPP_

#include "Model/OSGScene.hpp"
#include <memory>

namespace OMVIS
{
    namespace Model
    {

        /*! \brief Base class that contains XML infos and viewer.
         *
         * \todo privatize members
         */
        class OMVISScene
        {
         public:
            /*-----------------------------------------
             * CONSTRUCTORS
             *---------------------------------------*/

            OMVISScene();

            ~OMVISScene() = default;

            OMVISScene(const OMVISScene& omvv) = delete;

            OMVISScene& operator=(const OMVISScene& omvv) = delete;

            /*-----------------------------------------
             * GETTERS AND SETTERS
             *---------------------------------------*/

            /*! \brief This method dumps a string representation of the scene graph to stdOut.
             *
             */
            void dumpOSGTreeDebug();

            /*! \brief Returns pointer to OSGScene. */
            OSGScene* getScene();

         private:
            /*-----------------------------------------
             * MEMBERS
             *---------------------------------------*/

            OSGScene _scene;
        };

    }  // namespace Model
}  // namespace OMVIS

#endif /* INCLUDE_OMVISSCENE_HPP_ */
/**
 * \}
 */
