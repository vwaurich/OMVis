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

#include "Model/InputData.hpp"
#include "Util/Logger.hpp"
#include "Util/Util.hpp"

#include <iostream>
#include <string>

namespace OMVIS
{
    namespace Model
    {

        const inputKey keys_real[4] = { JOY_1_X, JOY_1_Y, JOY_2_X, JOY_2_Y };
        const inputKey keys_bool[4] = { KEY_W, KEY_A, KEY_S, KEY_D };

        /*-----------------------------------------
         * CONSTRUCTORS
         *---------------------------------------*/

        InputData::InputData()
                : _inputVals(),
                  _keyToInputMap(),
                  _keyboardToKeyMap()
        {
        }

        /*-----------------------------------------
         * INITIALIZATION METHODS
         *---------------------------------------*/

        void InputData::initializeInputs(fmi1_import_t* fmu)
        {
            // Get pointer to all variables.
            auto allVariables = fmi1_import_get_variable_list(fmu);
            // Define comparison function pointer.
            int (*causalityCheck)(fmi1_import_variable_t* vl, void* enumIdx);
            causalityCheck = &causalityEqual;
            int (*baseTypeCheck)(fmi1_import_variable_t* vl, void* refBaseType);
            baseTypeCheck = &baseTypeEqual;

            // Get all variables per type.
            fmi1_causality_enu_t causalityType = fmi1_causality_enu_t::fmi1_causality_enu_input;
            auto allInputs = fmi1_import_filter_variables(allVariables, causalityCheck,
                                                          static_cast<void*>(&causalityType));
            fmi1_base_type_enu_t baseType = fmi1_base_type_enu_t::fmi1_base_type_real;
            auto realInputs = fmi1_import_filter_variables(allInputs, baseTypeCheck, static_cast<void*>(&baseType));
            baseType = fmi1_base_type_enu_t::fmi1_base_type_int;
            auto integerInputs = fmi1_import_filter_variables(allInputs, baseTypeCheck, static_cast<void*>(&baseType));
            baseType = fmi1_base_type_enu_t::fmi1_base_type_bool;
            auto booleanInputs = fmi1_import_filter_variables(allInputs, baseTypeCheck, static_cast<void*>(&baseType));
            baseType = fmi1_base_type_enu_t::fmi1_base_type_str;
            auto stringInputs = fmi1_import_filter_variables(allInputs, baseTypeCheck, static_cast<void*>(&baseType));

            // All value references per type.
            _inputVals._vrReal = fmi1_import_get_value_referece_list(realInputs);
            _inputVals._vrInteger = fmi1_import_get_value_referece_list(integerInputs);
            _inputVals._vrBoolean = fmi1_import_get_value_referece_list(booleanInputs);
            _inputVals._vrString = fmi1_import_get_value_referece_list(stringInputs);

            // The number of inputs per type.
            _inputVals.setNumReal(fmi1_import_get_variable_list_size(realInputs));
            _inputVals.setNumInteger(fmi1_import_get_variable_list_size(integerInputs));
            _inputVals.setNumBoolean(fmi1_import_get_variable_list_size(booleanInputs));
            _inputVals.setNumString(fmi1_import_get_variable_list_size(stringInputs));

            // The variable names.
            getVariableNames(realInputs, _inputVals.getNumReal(), _inputVals._namesReal);
            getVariableNames(integerInputs, _inputVals.getNumInteger(), _inputVals._namesInteger);
            getVariableNames(booleanInputs, _inputVals.getNumBoolean(), _inputVals._namesBool);
            getVariableNames(stringInputs, _inputVals.getNumString(), _inputVals._namesString);

            // Initialize attributes for all real inputs
            _inputVals._attrReal = new Model::AttributesReal[_inputVals.getNumReal()];

            // Get attributes for all real inputs from modeldescription
            fmi1_import_real_variable_t* var = nullptr;
            for (auto r = 0u; r < _inputVals.getNumReal(); ++r)
            {
                var = fmi1_import_get_variable_as_real(fmi1_import_get_variable(realInputs, r));
                _inputVals._attrReal[r]._max = fmi1_import_get_real_variable_max(var);
                _inputVals._attrReal[r]._min = fmi1_import_get_real_variable_min(var);
                _inputVals._attrReal[r]._start = fmi1_import_get_real_variable_start(var);
                _inputVals._attrReal[r]._nominal = fmi1_import_get_real_variable_nominal(var);
            }

            initializeHelper();
        }

        void InputData::initializeInputs(const NetOff::VariableList& inputVars)
        {
            // The number of inputs per type.
            _inputVals.setNumReal(inputVars.sizeReals());
            _inputVals.setNumInteger(inputVars.sizeInts());
            _inputVals.setNumBoolean(inputVars.sizeBools());
            _inputVals.setNumString(0);

            // Set variable names in InputValues container.
            _inputVals._namesReal = inputVars.getReals();
            _inputVals._namesInteger = inputVars.getInts();
            _inputVals._namesBool = inputVars.getBools();
            _inputVals._namesString = std::vector<std::string>(0);

            initializeHelper();
        }

        void InputData::initializeHelper()
        {
            LOGGER_WRITE("Number of Reals: " + std::to_string(_inputVals._namesReal.size()), Util::LC_LOADER,
                         Util::LL_INFO);
            LOGGER_WRITE("Number of Integers: " + std::to_string(_inputVals._namesInteger.size()), Util::LC_LOADER,
                         Util::LL_INFO);
            LOGGER_WRITE("Number of Booleans: " + std::to_string(_inputVals._namesBool.size()), Util::LC_LOADER,
                         Util::LL_INFO);
            LOGGER_WRITE("Number of Strings: " + std::to_string(_inputVals._namesString.size()), Util::LC_LOADER,
                         Util::LL_INFO);

            LOGGER_WRITE(
                    "There are " + std::to_string(_inputVals.getNumBoolean()) + " boolean inputs, "
                            + std::to_string(_inputVals.getNumReal()) + " real inputs, "
                            + std::to_string(_inputVals.getNumInteger()) + " integer inputs and "
                            + std::to_string(_inputVals.getNumString()) + " string inputs.",
                    Util::LC_LOADER, Util::LL_INFO);

            // The values for the inputs per type.
            _inputVals._valuesReal =
                    reinterpret_cast<fmi1_real_t*>(calloc(_inputVals.getNumReal(), sizeof(fmi1_real_t)));
            _inputVals._valuesInteger = reinterpret_cast<fmi1_integer_t*>(calloc(_inputVals.getNumInteger(),
                                                                                 sizeof(fmi1_integer_t)));
            _inputVals._valuesBoolean = reinterpret_cast<fmi1_boolean_t*>(calloc(_inputVals.getNumBoolean(),
                                                                                 sizeof(fmi1_boolean_t)));
            _inputVals._valuesString = reinterpret_cast<fmi1_string_t*>(calloc(_inputVals.getNumString(),
                                                                               sizeof(fmi1_string_t)));
            // malloc attributes
            _inputVals._attrReal = reinterpret_cast<AttributesReal*>(calloc(_inputVals.getNumReal(),
                                                                            sizeof(AttributesReal)));

            // init keymap and attributes
            // ------------------
            _keyboardToKeyMap[119] = KEY_W;
            _keyboardToKeyMap[97] = KEY_A;
            _keyboardToKeyMap[115] = KEY_S;
            _keyboardToKeyMap[100] = KEY_D;

            int k = 0;
            // Make map from keys to input values.
            for (auto r = 0u; r < _inputVals.getNumReal(); ++r)
            {
                _keyToInputMap[keys_real[r]] =
                {   fmi1_base_type_real, r};
                LOGGER_WRITE("Assign real input " + std::to_string(r) + " to key "
                        + std::to_string(keys_real[r]), Util::LC_LOADER, Util::LL_INFO);
                LOGGER_WRITE("min " + std::to_string(_inputVals._attrReal[r]._min) + " max "
                        + std::to_string(_inputVals._attrReal[r]._max), Util::LC_LOADER, Util::LL_INFO);
                ++k;
            }
            for (auto r = 0u; r < _inputVals.getNumInteger(); ++r)
            {
                _keyToInputMap[keys_real[k]] =
                {   fmi1_base_type_int, r};
                ++k;
            }
            for (auto r = 0u; r < _inputVals.getNumBoolean(); ++r)
            {
                _keyToInputMap[keys_bool[r]] =
                {   fmi1_base_type_bool, r};
                LOGGER_WRITE("Assign boolean input " + Util::boolToString(r) + " to key "
                        + std::to_string(keys_bool[r]), Util::LC_LOADER, Util::LL_INFO);
                ++k;
            }
            for (auto r = 0u; r < _inputVals.getNumString(); ++r)
            {
                _keyToInputMap[keys_real[k]] =
                {   fmi1_base_type_str, r};
                ++k;
            }

            for (auto iter = _keyToInputMap.begin(); iter != _keyToInputMap.end(); ++iter)
            {
                LOGGER_WRITE(
                        "Key: " + std::to_string(iter->first) + " --> Values: " + std::to_string(iter->second._baseType)
                                + " " + std::to_string(iter->second._valueIdx),
                        Util::LC_LOADER, Util::LL_INFO);
            }
        }

        void InputData::resetInputValues()
        {
            // Reset real input values to 0.0.
            for (auto r = 0u; r < _inputVals.getNumReal(); ++r)
            {
                _inputVals._valuesReal[r] = 0.0;
            }
            // Reset integer input values to 0.
            for (auto i = 0u; i < _inputVals.getNumInteger(); ++i)
            {
                _inputVals._valuesInteger[i] = 0;
            }

            // Reset boolean and string values.
            resetDiscreteInputValues();
        }

        void InputData::resetDiscreteInputValues()
        {
            // Reset boolean input values to false.
            for (auto b = 0u; b < _inputVals.getNumBoolean(); ++b)
            {
                _inputVals._valuesBoolean[b] = 0;
            }
            // Reset string input values to empty string.
            for (auto s = 0u; s < _inputVals.getNumString(); ++s)
            {
                _inputVals._valuesString[s] = "";

            }
        }

        /*-----------------------------------------
         * GETTERS and SETTERS
         *---------------------------------------*/

        /// \todo: What do we do with the variable status?
        void InputData::setInputsInFMU(fmi1_import_t* fmu)
        {
            fmi1_status_t status = fmi1_import_set_real(fmu, _inputVals._vrReal, _inputVals.getNumReal(),
                                                        _inputVals._valuesReal);
            status = fmi1_import_set_integer(fmu, _inputVals._vrInteger, _inputVals.getNumInteger(),
                                             _inputVals._valuesInteger);
            status = fmi1_import_set_boolean(fmu, _inputVals._vrBoolean, _inputVals.getNumBoolean(),
                                             _inputVals._valuesBoolean);
            status = fmi1_import_set_string(fmu, _inputVals._vrBoolean, _inputVals.getNumString(),
                                            _inputVals._valuesString);
        }

        void InputData::getVariableNames(fmi1_import_variable_list_t* varLst, const int numVars,
                                         std::vector<std::string>& varNames)
        {
            std::string name("");
            fmi1_import_variable_t* var = nullptr;
            const char* na = nullptr;
            for (auto idx = 0; idx < numVars; ++idx)
            {
                var = fmi1_import_get_variable(varLst, idx);
                na = fmi1_import_get_variable_name(var);
                name.assign(na);
                varNames.push_back(name);
            }
        }

        fmi1_real_t* InputData::getRealValues() const
        {
            return _inputVals._valuesReal;
        }

        fmi1_integer_t* InputData::getIntValues() const
        {
            return _inputVals._valuesInteger;
        }

        fmi1_boolean_t* InputData::getBoolValues() const
        {
            return _inputVals._valuesBoolean;
        }

        bool InputData::setRealInputValueForInputKey(const inputKey key, const double value)
        {
            auto iter = _keyToInputMap.find(key);

            if (iter != _keyToInputMap.end())
            {
                KeyMapValue iterValue = iter->second;
                int baseTypeIdx = iterValue._baseType;
                if (baseTypeIdx == fmi1_base_type_real)
                {
                    int realIdx = iterValue._valueIdx;
                    //value
                    double min = _inputVals._attrReal[realIdx]._min;
                    double max = _inputVals._attrReal[realIdx]._max;
                    double val = value / 32767.0;
                    _inputVals._valuesReal[iterValue._valueIdx] = val;
                    return true;
                }

                LOGGER_WRITE("The value is not for a real input.", Util::LC_LOADER, Util::LL_WARNING);
                return false;

            }
            return false;
        }

        const keyboardMap* InputData::getKeyboardMap()
        {
            return &_keyboardToKeyMap;
        }

        const keyMap* InputData::getKeyMap()
        {
            return &_keyToInputMap;
        }

        const InputValues* InputData::getInputValues()
        {
            return &_inputVals;
        }

        /*-----------------------------------------
         * PRINTERS
         *---------------------------------------*/

        void InputData::printValues() const
        {
            for (auto r = 0u; r < _inputVals.getNumReal(); ++r)
            {
                LOGGER_WRITE(
                        "Real input " + std::to_string(r) + " (" + _inputVals._namesReal[r] + ") is "
                                + std::to_string(_inputVals._valuesReal[r]),
                        Util::LC_CTR, Util::LL_DEBUG);
            }
            for (auto i = 0u; i < _inputVals.getNumInteger(); ++i)
            {
                LOGGER_WRITE(
                        "Integer input " + std::to_string(i) + " (" + _inputVals._namesInteger[i] + ") is "
                                + std::to_string(_inputVals._valuesInteger[i]),
                        Util::LC_CTR, Util::LL_DEBUG);
            }
            for (auto b = 0u; b < _inputVals.getNumBoolean(); ++b)
            {
                LOGGER_WRITE(
                        "Boolean input " + Util::boolToString(b) + " (" + _inputVals._namesBool[b] + ") is "
                                + std::to_string(_inputVals._valuesBoolean[b]),
                        Util::LC_CTR, Util::LL_DEBUG);
            }
            for (auto s = 0u; s < _inputVals.getNumString(); ++s)
            {
                LOGGER_WRITE(
                        "String input " + std::to_string(s) + " (" + _inputVals._namesString[s] + ") is "
                                + _inputVals._valuesString[s],
                        Util::LC_CTR, Util::LL_DEBUG);
            }
        }

        void InputData::printKeyToInputMap() const
        {
            LOGGER_WRITE("KEY TO INPUT MAP", Util::LC_CTR, Util::LL_DEBUG);
            for (auto it = _keyToInputMap.begin(); it != _keyToInputMap.end(); ++it)
            {
                LOGGER_WRITE(std::to_string(it->first) + " " + keyMapValueToString(it->second), Util::LC_CTR,
                             Util::LL_DEBUG);
            }
        }

        /*-----------------------------------------
         * Free Functions
         *---------------------------------------*/

        /// \todo: No return?!
        inputKey getInputDataKeyForString(std::string keyString)
        {
            if (0 == keyString.compare("JOY_1_X"))
            {
                return JOY_1_X;
            }
            if (0 == keyString.compare("JOY_1_Y"))
            {
                return JOY_1_Y;
            }
            if (0 == keyString.compare("JOY_2_X"))
            {
                return JOY_2_X;
            }
            if (0 == keyString.compare("JOY_2_Y"))
            {
                return JOY_2_Y;
            }
            if (0 == keyString.compare("KEY_W"))
            {
                return KEY_W;
            }
            if (0 == keyString.compare("KEY_A"))
            {
                return KEY_A;
            }
            if (0 == keyString.compare("KEY_S"))
            {
                return KEY_S;
            }
            if (0 == keyString.compare("KEY_D"))
            {
                return KEY_D;
            }

            std::cout << "getInputKeyForString  failed" << std::endl;
        }

        std::string keyMapValueToString(KeyMapValue val)
        {
            return std::string(fmi1_base_type_to_string(val._baseType)).append(std::to_string(val._valueIdx));
        }

        int causalityEqual(fmi1_import_variable_t* var, void* enumIdx)
        {
            // Get causality attribute, i.e., fmi1_causality_enu_input, fmi1_causality_enu_output, etc.
            fmi1_causality_enu_t causality = fmi1_import_get_causality(var);
            // Use static cast to "convert" void* to fmi1_causality_enu_t*. With that, we can compare the attributes.
            fmi1_causality_enu_t* toComp = static_cast<fmi1_causality_enu_t*>(enumIdx);

            //LOGGER_WRITE(std::string("causalityEqual: fmi1_causality_enu_t causality is ") + std::to_string(causality) + " and enumIdx has attribute " + std::to_string(*toComp) + std::string("."), Util::LC_INIT, Util::LL_INFO);
            if (*toComp == causality)
            {
                //LOGGER_WRITE(std::string("They are equal!"), Util::LC_INIT, Util::LL_INFO);
                //std::cout << "the input var: " << fmi1_import_get_variable_name(var) << std::endl;
                return 1;
            }

            //LOGGER_WRITE(std::string("They are not equal!"), Util::LC_INIT, Util::LL_INFO);
            return 0;

        }

        int baseTypeEqual(fmi1_import_variable_t* var, void* refBaseType)
        {
            fmi1_base_type_enu_t baseType = fmi1_import_get_variable_base_type(var);
            fmi1_base_type_enu_t* toComp = static_cast<fmi1_base_type_enu_t*>(refBaseType);

            //LOGGER_WRITE(std::string("baseTypeEqual: fmi1_base_type_enu_t baseType is ") + std::to_string(baseType) + " and refBaseType is of type" + std::to_string(*toComp) + std::string("."), Util::LC_INIT, Util::LL_INFO);
            if (*toComp == baseType)
            {
                //LOGGER_WRITE(std::string("They are equal!"), Util::LC_INIT, Util::LL_INFO);
                return 1;
            }

            //LOGGER_WRITE(std::string("They are not equal!"), Util::LC_INIT, Util::LL_INFO);
            return 0;

        }

    }  // namespace Model
}  // namespace OMVIS
