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

/*
 * InfoVisitor.hpp
 *
 *  Created on: 03.02.2016
 *      Author: mf
 */

#ifndef INCLUDE_INFOVISITOR_HPP_
#define INCLUDE_INFOVISITOR_HPP_

#include <string>
#include <osg/NodeVisitor>
#include <osg/Geode>

/*! \brief InfoVisitor to print the osg tree.
 *
 *
 */
class InfoVisitor : public osg::NodeVisitor
{
 public:
    /// Default constructor. Attributes are set to default values and call osg::setTraversalMode method.
    InfoVisitor();

    /// Let the compiler provide the destructor.
    ~InfoVisitor() = default;

    /// The copy constructor is forbidden.
    InfoVisitor(const InfoVisitor& iv) = delete;

    /// The assignment operator is forbidden.
    InfoVisitor& operator=(const InfoVisitor& iv) = delete;

    /// Returns some spaces
    std::string spaces();

    /// Node information
    virtual void apply(osg::Node& node);

    /// Geode information
    virtual void apply(osg::Geode& node);

 private:
    unsigned int _level;
};

#endif /* INCLUDE_INFOVISITOR_HPP_ */
