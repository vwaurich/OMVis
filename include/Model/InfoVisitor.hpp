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

#ifndef INCLUDE_INFOVISITOR_HPP_
#define INCLUDE_INFOVISITOR_HPP_

#include <osg/NodeVisitor>
#include <osg/Geode>

#include <string>

namespace OMVIS
{
    namespace Model
    {

        /*! \brief InfoVisitor to print the osg tree.
         *
         *
         */
        class InfoVisitor : public osg::NodeVisitor
        {
         public:
            /*-----------------------------------------
             * CONSTRUCTORS
             *---------------------------------------*/

            /// Default constructor. Attributes are set to default values and call osg::setTraversalMode method.
            InfoVisitor();

            /// Let the compiler provide the destructor.
            ~InfoVisitor() = default;

            /// The copy constructor is forbidden.
            InfoVisitor(const InfoVisitor& iv) = delete;

            /// The assignment operator is forbidden.
            InfoVisitor& operator=(const InfoVisitor& iv) = delete;

            /*-----------------------------------------
             * SIMULATION METHODS
             *---------------------------------------*/

            /// Returns some spaces
            std::string spaces();

            /// Node information
            virtual void apply(osg::Node& node);

            /// Geode information
            virtual void apply(osg::Geode& geode);

         private:
            /*-----------------------------------------
             * MEMBERS
             *---------------------------------------*/

            unsigned int _level;
        };

    }  // namespace Model
}  // namespace OMVIS

#endif /* INCLUDE_INFOVISITOR_HPP_ */
/**
 * @}
 */
