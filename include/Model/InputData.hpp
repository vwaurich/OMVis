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
 * InputData.hpp
 *
 *  Created on: 10.02.2016
 *      Author: mf
 */

#ifndef INCLUDE_INPUTDATA_HPP_
#define INCLUDE_INPUTDATA_HPP_

#include <map>
#include "WrapperFMILib.hpp"
#include "InputValues.hpp"

enum inputKey
{
    JOY_1_X,
    JOY_1_Y,
	JOY_2_X,
	JOY_2_Y,
	KEY_W,
    KEY_A,
    KEY_S,
    KEY_D
};

typedef struct
{
    fmi1_base_type_enu_t _baseType;
    unsigned int _valueIdx;
} KeyMapValue;

//mapping from inputKeys to the input variable values
typedef std::map<inputKey, KeyMapValue> keyMap;
typedef keyMap::const_iterator keyMapIter;

typedef std::map<unsigned int, inputKey> keyboardMap;
typedef keyboardMap::const_iterator keyBoardMapIter;

namespace Model
{

    /*! \brief Base class for input data.
     *
     * Only FMU
     */
    class InputData
    {
     public:
        /*! \brief Default constructor. Attributes are initialized with default values. */
        InputData();

        /*! \brief Let the compiler create the destructor. */
        ~InputData() = default;

        /*! \brief Copy constructor. */
        InputData(const InputData& ipd);

        /*! The assignment operator is forbidden. */
        InputData& operator=(const InputData& ipd) = delete;

        /*! \brief Initializes all input data including the keymap. */
        void initializeInputs(fmi1_import_t* fmu);

        /*! \brief Sets the input variables in the fmu. */
        void setInputsInFMU(fmi1_import_t* fmu);

        /*! \brief Resets the input values to zero. */
        void resetInputValues();

        /*! \brief Prints the current input values. */
        void printValues();

		/*! \brief Gets the names of the variables*/
		void getVariableNames(fmi1_import_variable_list_t* varLst, const int numVars, std::vector<std::string>* varNames);

		/*! \brief Prints the keyToInputMap*/
		void printKeyToInputMap();

	public:
        /// \todo Can this attribute be private?
        InputValues _data;
        /// \todo Can this attribute be private?
        keyMap _keyToInputMap;
        /// \todo Can this attribute be private?
        keyboardMap _keyboardToKeyMap;
    };

    bool setRealInputValueForInputKey(const inputKey key, const double value, InputData data);

	/*! \brief Conversion from std::string to inputKey enum */
	inputKey getInputDataKeyForString(std::string keyString);

	std::string keyMapValueToString(KeyMapValue);

	/*! \brief Checks the causality of the var and outputs 1 if they are equal, 0 otherwise.
	 *
	 */
	int causalityEqual(fmi1_import_variable_t* var, void* enumIdx);

	/*! \brief Checks the type of the var and outputs 1 if they are equal.
	 *
	 */
	int baseTypeEqual(fmi1_import_variable_t* var, void* refBaseType);

}  // End namespace Model

#endif /* INCLUDE_INPUTDATA_HPP_ */
