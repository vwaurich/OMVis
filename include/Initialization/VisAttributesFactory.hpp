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
 * Factory.hpp
 *
 *  Created on: 18.02.2016
 *      Author: mf
 */

#ifndef INCLUDE_VISATTRIBUTE_FACTORY_HPP_
#define INCLUDE_VISATTRIBUTE_FACTORY_HPP_

 // Forward declarations
namespace Model {
	class VisAttributes;
	class ShapeAttributes;
}


namespace Initialization
{
    /*! \brief This factory class can create VisAttributes object depending on the type
     *
     */
    class VisAttributesFactory
    {
     public:
		 VisAttributesFactory() = default;

        ~VisAttributesFactory() = default;
		VisAttributesFactory(const VisAttributesFactory& f) = delete;
		VisAttributesFactory& operator=(const VisAttributesFactory& f) = delete;

        Model::VisAttributes* createVisAttributes(std::string typeName);
    };

}  // End namespace Initialization

#endif /* INCLUDE_VISATTRIBUTE_FACTORY_HPP_ */
