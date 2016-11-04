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

#include "Model/OMVisScene.hpp"
#include "Model/InfoVisitor.hpp"

namespace OMVIS
{
    namespace Model
    {

        /*-----------------------------------------
         * CONSTRUCTORS
         *---------------------------------------*/

        OMVisScene::OMVisScene()
                : _scene()
        {
        }

        /*-----------------------------------------
         * GETTERS AND SETTERS
         *---------------------------------------*/

        void OMVisScene::dumpOSGTreeDebug()
        {
            // The node traverser which dumps the tree
            InfoVisitor infoVisitor;
            _scene.getRootNode()->accept(infoVisitor);
        }

        OSGScene* OMVisScene::getScene()
        {
            return &_scene;
        }

    }  // namespace Model
}  // namespace OMVIS
