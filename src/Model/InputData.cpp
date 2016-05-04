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

        InputData::InputData()
                : _data(),
                  _keyToInputMap(),
                  _keyboardToKeyMap()
        {
        }

        InputData::InputData(const InputData& ipd)
                : _data(ipd._data),
                  _keyToInputMap(ipd._keyToInputMap),
                  _keyboardToKeyMap(ipd._keyboardToKeyMap)
        {
        }

        void InputData::initializeInputs(fmi1_import_t* fmu)
        {
            // Get pointer to all variables.
            fmi1_import_variable_list_t* allVariables = fmi1_import_get_variable_list(fmu);
            // Define comparison function pointer.
            int (*causalityCheck)(fmi1_import_variable_t* vl, void* enumIdx);
            causalityCheck = &causalityEqual;
            int (*baseTypeCheck)(fmi1_import_variable_t* vl, void* refBaseType);
            baseTypeCheck = &baseTypeEqual;

            // Get all variables per type.
            fmi1_causality_enu_t causalityType = fmi1_causality_enu_t::fmi1_causality_enu_input;
            fmi1_import_variable_list_t* allInputs = fmi1_import_filter_variables(allVariables, causalityCheck, static_cast<void*>(&causalityType));
            fmi1_base_type_enu_t baseType = fmi1_base_type_enu_t::fmi1_base_type_real;
            fmi1_import_variable_list_t* realInputs = fmi1_import_filter_variables(allInputs, baseTypeCheck, static_cast<void*>(&baseType));
            baseType = fmi1_base_type_enu_t::fmi1_base_type_int;
            fmi1_import_variable_list_t* integerInputs = fmi1_import_filter_variables(allInputs, baseTypeCheck, static_cast<void*>(&baseType));
            baseType = fmi1_base_type_enu_t::fmi1_base_type_bool;
            fmi1_import_variable_list_t* booleanInputs = fmi1_import_filter_variables(allInputs, baseTypeCheck, static_cast<void*>(&baseType));
            baseType = fmi1_base_type_enu_t::fmi1_base_type_str;
            fmi1_import_variable_list_t* stringInputs = fmi1_import_filter_variables(allInputs, baseTypeCheck, static_cast<void*>(&baseType));

            // All value references per type.
            _data._vrReal = fmi1_import_get_value_referece_list(realInputs);
            _data._vrInteger = fmi1_import_get_value_referece_list(integerInputs);
            _data._vrBoolean = fmi1_import_get_value_referece_list(booleanInputs);
            _data._vrString = fmi1_import_get_value_referece_list(stringInputs);
            //the number of inputs per type
            _data.setNumReal(fmi1_import_get_variable_list_size(realInputs));
            _data.setNumInteger(fmi1_import_get_variable_list_size(integerInputs));
            _data.setNumBoolean(fmi1_import_get_variable_list_size(booleanInputs));
            _data.setNumString(fmi1_import_get_variable_list_size(stringInputs));

            //the variable names
            getVariableNames(realInputs, _data.getNumReal(), &_data._namesReal);
            getVariableNames(integerInputs, _data.getNumInteger(), &_data._namesInteger);
            getVariableNames(booleanInputs, _data.getNumBoolean(), &_data._namesBool);
            getVariableNames(stringInputs, _data.getNumString(), &_data._namesString);

            LOGGER_WRITE(std::string(), Util::LC_LOADER, Util::LL_INFO);

            LOGGER_WRITE(std::string("Number of Reals: ") + std::to_string(_data._namesReal.size()), Util::LC_LOADER, Util::LL_INFO);
            LOGGER_WRITE(std::string("Number of Integers: ") + std::to_string(_data._namesInteger.size()), Util::LC_LOADER, Util::LL_INFO);
            LOGGER_WRITE(std::string("Number of Booleans: ") + std::to_string(_data._namesBool.size()), Util::LC_LOADER, Util::LL_INFO);
            LOGGER_WRITE(std::string("Number of Strings: ") + std::to_string(_data._namesString.size()), Util::LC_LOADER, Util::LL_INFO);

            LOGGER_WRITE(std::string("There are ") + std::to_string(_data.getNumBoolean()) + std::string(" boolean inputs, ") + std::to_string(_data.getNumReal()) + std::string(" real inputs, ") + std::to_string(_data.getNumInteger()) + std::string(" integer inputs and ") + std::to_string(_data.getNumString()) + std::string(" string inputs."), Util::LC_LOADER, Util::LL_INFO);

            // the values for the inputs per type
            _data._valuesReal = (fmi1_real_t*) calloc(_data.getNumReal(), sizeof(fmi1_real_t));
            _data._valuesInteger = (fmi1_integer_t*) calloc(_data.getNumInteger(), sizeof(fmi1_integer_t));
            _data._valuesBoolean = (fmi1_boolean_t*) calloc(_data.getNumBoolean(), sizeof(fmi1_boolean_t));
            _data._valuesString = (fmi1_string_t*) calloc(_data.getNumString(), sizeof(fmi1_string_t));
            // malloc attributes
            _data._attrReal = (AttributesReal*) calloc(_data.getNumReal(), sizeof(AttributesReal));

            // init keymap and attributes
            // ------------------
            _keyboardToKeyMap[119] = KEY_W;
            _keyboardToKeyMap[97] = KEY_A;
            _keyboardToKeyMap[115] = KEY_S;
            _keyboardToKeyMap[100] = KEY_D;

            int k = 0;
            //make map from keys to input values
            for (unsigned int r = 0; r < _data.getNumReal(); ++r)
            {
                KeyMapValue mapValue = { fmi1_base_type_real, r };
                _keyToInputMap[keys_real[r]] = mapValue;
                LOGGER_WRITE(std::string("Assign real input ") + std::to_string(r) + std::string(" to key ") + std::to_string(keys_real[r]), Util::LC_LOADER, Util::LL_INFO);
                //std::cout << "assign realinput " << r << " to key " << keys_real[r] << std::endl;
                fmi1_import_real_variable_t* var = fmi1_import_get_variable_as_real(fmi1_import_get_variable(realInputs, r));
                _data._attrReal[r]._max = fmi1_import_get_real_variable_max(var);
                _data._attrReal[r]._min = fmi1_import_get_real_variable_min(var);
                _data._attrReal[r]._start = fmi1_import_get_real_variable_start(var);
                _data._attrReal[r]._nominal = fmi1_import_get_real_variable_nominal(var);
                LOGGER_WRITE(std::string("min ") + std::to_string(_data._attrReal[r]._min) + std::string(" max ") + std::to_string(_data._attrReal[r]._max), Util::LC_LOADER, Util::LL_INFO);
                ++k;
            }
            for (unsigned int i = 0; i < _data.getNumInteger(); ++i)
            {
                KeyMapValue mapValue = { fmi1_base_type_int, i };
                _keyToInputMap[keys_real[k]] = mapValue;
                ++k;
            }
            for (unsigned int b = 0; b < _data.getNumBoolean(); ++b)
            {
                KeyMapValue mapValue = { fmi1_base_type_bool, b };
                _keyToInputMap[keys_bool[b]] = mapValue;
                LOGGER_WRITE(std::string("Assign boolean input ") + Util::boolToString(b) + std::string(" to key ") + std::to_string(keys_bool[b]), Util::LC_LOADER, Util::LL_INFO);
                ++k;
            }
            for (unsigned int s = 0; s < _data.getNumString(); ++s)
            {
                KeyMapValue mapValue = { fmi1_base_type_str, s };
                _keyToInputMap[keys_real[k]] = mapValue;
                ++k;
            }

            for (keyMapIter iter = _keyToInputMap.begin(); iter != _keyToInputMap.end(); ++iter)
                LOGGER_WRITE(std::string("Key: ") + std::to_string(iter->first) + std::string(" --> Values: ") + std::to_string(iter->second._baseType) + std::string(" ") + std::to_string(iter->second._valueIdx), Util::LC_LOADER, Util::LL_INFO);
        }

        void InputData::resetInputValues()
        {
            //reset real input values to 0
            for (size_t r = 0; r < _data.getNumReal(); ++r)
                _data._valuesReal[r] = 0.0;
            //reset integer input values to 0
            for (size_t i = 0; i < _data.getNumInteger(); ++i)
                _data._valuesInteger[i] = 0;
            //reset boolean input values to 0
            for (size_t b = 0; b < _data.getNumBoolean(); ++b)
                _data._valuesBoolean[b] = false;
            //reset string input values to 0
            for (size_t s = 0; s < _data.getNumString(); ++s)
                _data._valuesString[s] = "";
        }

        void InputData::setInputsInFMU(fmi1_import_t* fmu)
        {
            fmi1_status_t status = fmi1_import_set_real(fmu, _data._vrReal, _data.getNumReal(), _data._valuesReal);
            status = fmi1_import_set_integer(fmu, _data._vrInteger, _data.getNumInteger(), _data._valuesInteger);
            status = fmi1_import_set_boolean(fmu, _data._vrBoolean, _data.getNumBoolean(), _data._valuesBoolean);
            status = fmi1_import_set_string(fmu, _data._vrBoolean, _data.getNumString(), _data._valuesString);
        }

        void InputData::printValues()
        {
            for (unsigned int r = 0; r < _data.getNumReal(); ++r)
                std::cout << "realinput " << r << " (" << _data._namesReal.at(r) << ") " << " is " << _data._valuesReal[r] << std::endl;
            for (unsigned int i = 0; i < _data.getNumInteger(); ++i)
                std::cout << "integer input " << i << " (" << _data._namesInteger.at(i) << ") " << " is " << _data._valuesInteger[i] << std::endl;
            for (unsigned int b = 0; b < _data.getNumBoolean(); ++b)
                std::cout << "bool input " << b << " (" << _data._namesBool.at(b) << ") " << " is " << _data._valuesBoolean[b] << std::endl;
            for (unsigned int s = 0; s < _data.getNumString(); ++s)
                std::cout << "string input " << s << " (" << _data._namesString.at(s) << ") " << " is " << _data._valuesString[s] << std::endl;
        }

        void InputData::resetDiscreteInputValues()
        {
            //reset boolean input values to 0
            for (unsigned int b = 0; b < _data.getNumBoolean(); ++b)
                _data._valuesBoolean[b] = false;
            //reset string input values to 0
            for (unsigned int s = 0; s < _data.getNumString(); ++s)
                _data._valuesString[s] = "";
        }

        /// \todo: The attributes of the object 'data' are changed in line 206, but we pass not by reference. Thus,
        /// this update has no effect on the original object.
        /// \todo Todo pass INputData by reference or pointer with respect to performance!
        bool setRealInputValueForInputKey(const inputKey key, const double value, InputData data)
        {
            keyMapIter iter = data._keyToInputMap.find(key);

            if (iter != data._keyToInputMap.end())
            {
                KeyMapValue iterValue = iter->second;
                int baseTypeIdx = iterValue._baseType;
                if (baseTypeIdx == fmi1_base_type_real)
                {
                    int realIdx = iterValue._valueIdx;
                    //value
                    double min = data._data._attrReal[realIdx]._min;
                    double max = data._data._attrReal[realIdx]._max;
                    double val = value / 32767.0;
                    data._data._valuesReal[iterValue._valueIdx] = val;
                    return true;
                }
                else
                {
                    LOGGER_WRITE(std::string("The value is not for a real input."), Util::LC_LOADER, Util::LL_INFO);
                    return false;
                }
            }
            return false;
        }

        void InputData::getVariableNames(fmi1_import_variable_list_t* varLst, const int numVars, std::vector<std::string>* varNames)
        {
            std::string name("");
            fmi1_import_variable_t* var = nullptr;
            const char* na = nullptr;
            for (int idx = 0; idx < numVars; ++idx)
            {
                var = fmi1_import_get_variable(varLst, idx);
                na = fmi1_import_get_variable_name(var);
                name.assign(na);
                varNames->push_back(name);
            }
        }

        void InputData::printKeyToInputMap()
        {
            std::cout << "KEY TO INPUT MAP" << std::endl;
            for (std::map<inputKey, KeyMapValue>::const_iterator it = _keyToInputMap.begin(); it != _keyToInputMap.end(); ++it)
                std::cout << it->first << " " << keyMapValueToString(it->second) << "\n";
        }

        /*-----------------------------------------
         * Free Functions
         *---------------------------------------*/
        /// \todo: No return?!
        inputKey getInputDataKeyForString(std::string keyString)
        {
            if (!keyString.compare("JOY_1_X"))
                return JOY_1_X;
            else if (!keyString.compare("JOY_1_Y"))
                return JOY_1_Y;
            else if (!keyString.compare("JOY_2_X"))
                return JOY_2_X;
            else if (!keyString.compare("JOY_2_Y"))
                return JOY_2_Y;
            else if (!keyString.compare("KEY_W"))
                return KEY_W;
            else if (!keyString.compare("KEY_A"))
                return KEY_A;
            else if (!keyString.compare("KEY_S"))
                return KEY_S;
            else if (!keyString.compare("KEY_D"))
                return KEY_D;
            else
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
            else
            {
                //LOGGER_WRITE(std::string("They are not equal!"), Util::LC_INIT, Util::LL_INFO);
                return 0;
            }
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
            else
            {
                //LOGGER_WRITE(std::string("They are not equal!"), Util::LC_INIT, Util::LL_INFO);
                return 0;
            }
        }

    }  // End namespace Model
}  // End namespace OMVIS
