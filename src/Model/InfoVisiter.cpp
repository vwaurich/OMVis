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

#include "Model/InfoVisitor.hpp"

#include <iostream>

namespace OMVIS
{
    namespace Model
    {

        InfoVisitor::InfoVisitor()
                : _level(0)
        {
            setTraversalMode(NodeVisitor::TRAVERSE_ALL_CHILDREN);
        }

        std::string InfoVisitor::spaces()
        {
            return std::string(_level * 2, ' ');
        }

        void InfoVisitor::apply(osg::Node& node)
        {
            std::cout << spaces() << node.libraryName() << "::" << node.className() << std::endl;
            ++_level;
            traverse(node);
            --_level;
        }

        void InfoVisitor::apply(osg::Geode& geode)
        {
            std::cout << spaces() << geode.libraryName() << "::" << geode.className() << std::endl;
            ++_level;
            osg::Drawable* drawable = nullptr;
            for (size_t i = 0; i < geode.getNumDrawables(); ++i)
            {
                drawable = geode.getDrawable(i);
                std::cout << spaces() << drawable->libraryName() << "::" << drawable->className() << std::endl;
            }
            traverse(geode);
            --_level;
        }

    }  // namespace Model
}  // namespace OMVIS
