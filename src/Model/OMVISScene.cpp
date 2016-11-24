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

#include <Model/OMVISScene.hpp>
#include "Model/InfoVisitor.hpp"

namespace OMVIS
{
    namespace Model
    {

        /*-----------------------------------------
         * CONSTRUCTORS
         *---------------------------------------*/

        OMVISScene::OMVISScene()
                : _scene()
        {
        }

        /*-----------------------------------------
         * GETTERS AND SETTERS
         *---------------------------------------*/

        void OMVISScene::dumpOSGTreeDebug()
        {
            // The node traverser which dumps the tree
            InfoVisitor infoVisitor;
            _scene.getRootNode()->accept(infoVisitor);
        }

        OSGScene* OMVISScene::getScene()
        {
            return &_scene;
        }

    }  // namespace Model
}  // namespace OMVIS
