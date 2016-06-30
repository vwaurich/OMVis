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

/** @addtogroup Model
 *  @{
 *  \copyright TU Dresden. All rights reserved.
 *  \authors Volker Waurich, Martin Flehmig
 *  \date Feb 2016
 */

#ifndef INCLUDE_INPUTDATA_HPP_
#define INCLUDE_INPUTDATA_HPP_

#include "WrapperFMILib.hpp"
#include "InputValues.hpp"

#include <VariableList.hpp>

#include <map>

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

namespace OMVIS
{
    namespace Model
    {

        /*! \brief Base class for input data.
         *
         * Only FMU
         */
        class InputData
        {
         public:
            /*-----------------------------------------
             * CONSTRUCTORS
             *---------------------------------------*/

            /*! \brief Default constructor. Attributes are initialized with default values. */
            InputData();

            /*! \brief Let the compiler create the destructor. */
            ~InputData() = default;

            /*! \brief Copy constructor. */
            InputData(const InputData& ipd);

            /*! The assignment operator is forbidden. */
            InputData& operator=(const InputData& ipd) = delete;

            /*-----------------------------------------
             * INITIALIZATION METHODS
             *---------------------------------------*/

            /*! \brief Initializes all input data including the keymap for the given FMU.
             *
             * This method is used in case of FMU visualization by the class \ref OMVIsualizerFMU.
             *
             * \input fmu The FMU to initialize the data for.
             */
            void initializeInputs(fmi1_import_t* fmu);

            /*! \brief Initializes all input data including the keymap in case of remote visualization.
             *
             * This method is used in case of remote FMU visualization by the class \ref OMVIsualizerFMUClient.
             *
             * \input inputVars The input variables to initialize the data for.
             */
            void initializeInputs(const NetOff::VariableList& inputVars);

            /*! \brief This is a helper function for the two initialize functions.
             *
             * This helper holds code, that both functions share. Thus, it reduces code and improves the
             * maintainability.
             */
            void initializeHelper();

            /*! \brief Resets the input values to default values.
             * This is:
             *      - real values to 0.0
             *      - integer values to 0
             *      - boolean values to false
             *      - string values to empty string
             */
            void resetInputValues();

            /*! \brief Resets the discrete input values to zero (no reals and integers). */
            void resetDiscreteInputValues();

            /*-----------------------------------------
             * GETTERS and SETTERS
             *---------------------------------------*/

            /*! \brief Sets the input variables in the FMU. */
            void setInputsInFMU(fmi1_import_t* fmu);

            /*! \brief Gets the names of the variables and stores them in the given vector varNames.
             *
             * \remark: The variable names are added via push_back method at the end of the given vector.
             */
            void getVariableNames(fmi1_import_variable_list_t* varLst, const int numVars, std::vector<std::string>& varNames);

            /*! Returns pointer to real values. */
            fmi1_real_t* getRealValues() const;
            /*! Returns pointer to integer values. */
            fmi1_integer_t* getIntValues() const;
            /*! Returns pointer to boolean values. */
            fmi1_boolean_t* getBoolValues() const;

            /*-----------------------------------------
             * PRINTERS
             *---------------------------------------*/

            /*! \brief Prints the current input values. */
            void printValues() const;

            /*! \brief Prints the keyToInputMap*/
            void printKeyToInputMap() const;

         public:
            /// \todo Can this attribute be private?
            InputValues _data;
            /// \todo Can this attribute be private?
            keyMap _keyToInputMap;
            /// \todo Can this attribute be private?
            keyboardMap _keyboardToKeyMap;
        };

        /*-----------------------------------------
         * Free Functions
         *---------------------------------------*/

        bool setRealInputValueForInputKey(const inputKey key, const double value, InputData& data);

        /*! \brief Converts the given std::string to inputKey enum. */
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
}  // End namespace OMVIS

#endif /* INCLUDE_INPUTDATA_HPP_ */
/**
 * @}
 */
