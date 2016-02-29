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
 * InputValues.hpp
 *
 *  Created on: 10.02.2016
 *      Author: mf
 */

#ifndef INCLUDE_INPUTVALUES_HPP_
#define INCLUDE_INPUTVALUES_HPP_

#include "WrapperFMILib.hpp"
#include <vector>

namespace Model
{

    typedef struct
    {
        fmi1_real_t _min;
        fmi1_real_t _max;
        fmi1_real_t _nominal;
        fmi1_real_t _start;
    } attributes_real;

    /*! \brief Base class for all inputValues.
     *
     * \todo Can the attributes be private?
     */
    class InputValues
    {
     public:
        /*! \brief Default constructor. Attributes are initialized with default values. */
        InputValues()
                : _vrReal(nullptr),
                  _vrInteger(nullptr),
                  _vrBoolean(nullptr),
                  _vrString(nullptr),
                  _valuesReal(nullptr),
                  _valuesInteger(nullptr),
                  _valuesBoolean(nullptr),
                  _valuesString(nullptr),
				  namesReal(),
			   	  namesInteger(),
				  namesBool(),
				  namesString(),
                  _numReal(0),
                  _numInteger(0),
                  _numBoolean(0),
                  _numString(0),
                  _attrReal(nullptr)
        {
        }

        /*! Let the compiler provide the destructor. */
        ~InputValues() = default;

        /// \todo Check: Do we copy the pointer or should we copy the values the pointers point to?!
        InputValues(const InputValues& ipv) = default;
        InputValues& operator=(const InputValues& ipv) = delete;

     public:
        const fmi1_value_reference_t* _vrReal;
        const fmi1_value_reference_t* _vrInteger;
        const fmi1_value_reference_t* _vrBoolean;
        const fmi1_value_reference_t* _vrString;
        fmi1_real_t* _valuesReal;
        fmi1_integer_t* _valuesInteger;
        fmi1_boolean_t* _valuesBoolean;
        fmi1_string_t* _valuesString;
		std::vector<std::string> namesReal;
		std::vector<std::string> namesInteger;
		std::vector<std::string> namesBool;
		std::vector<std::string> namesString;
        unsigned int _numReal;
        unsigned int _numInteger;
        unsigned int _numBoolean;
        unsigned int _numString;

        attributes_real* _attrReal;
    };

}  // End namespace Model

#endif /* INCLUDE_INPUTVALUES_HPP_ */
