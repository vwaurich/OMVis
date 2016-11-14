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

/** \addtogroup Model
 *  \{
 *  \copyright TU Dresden. All rights reserved.
 *  \authors Volker Waurich, Martin Flehmig
 *  \date Feb 2016
 */

#ifndef INCLUDE_INPUTVALUES_HPP_
#define INCLUDE_INPUTVALUES_HPP_

#include "WrapperFMILib.hpp"

#include <string>
#include <vector>

namespace OMVIS
{
    namespace Model
    {

        typedef struct
        {
            fmi1_real_t _min;
            fmi1_real_t _max;
            fmi1_real_t _nominal;
            fmi1_real_t _start;
        } AttributesReal;

        /*! \brief Base class for all inputValues.
         *
         * \todo Can the attributes be private?
         */
        class InputValues
        {
         public:
            /*-----------------------------------------
             * CONSTRUCTORS
             *---------------------------------------*/

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
                      _namesReal(),
                      _namesInteger(),
                      _namesBool(),
                      _namesString(),
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

            /*-----------------------------------------
             * SETTERS AND GETTERS
             *---------------------------------------*/

            void setNumReal(const size_t num)
            {
                _numReal = num;
            }
            void setNumInteger(const size_t num)
            {
                _numInteger = num;
            }
            void setNumBoolean(const size_t num)
            {
                _numBoolean = num;
            }
            void setNumString(const size_t num)
            {
                _numString = num;
            }

            size_t getNumReal() const
            {
                return _numReal;
            }
            size_t getNumBoolean() const
            {
                return _numBoolean;
            }
            size_t getNumInteger() const
            {
                return _numInteger;
            }
            size_t getNumString() const
            {
                return _numString;
            }

         public:
            /*-----------------------------------------
             * MEMBERS
             *---------------------------------------*/

            const fmi1_value_reference_t* _vrReal;
            const fmi1_value_reference_t* _vrInteger;
            const fmi1_value_reference_t* _vrBoolean;
            const fmi1_value_reference_t* _vrString;
            fmi1_real_t* _valuesReal;
            fmi1_integer_t* _valuesInteger;
            fmi1_boolean_t* _valuesBoolean;
            fmi1_string_t* _valuesString;
            std::vector<std::string> _namesReal;
            std::vector<std::string> _namesInteger;
            std::vector<std::string> _namesBool;
            std::vector<std::string> _namesString;

         private:
            size_t _numReal;
            size_t _numInteger;
            size_t _numBoolean;
            size_t _numString;

         public:
            AttributesReal* _attrReal;
        };

    }  // namespace Model
}  // namespace OMVIS

#endif /* INCLUDE_INPUTVALUES_HPP_ */
/**
 * \}
 */
