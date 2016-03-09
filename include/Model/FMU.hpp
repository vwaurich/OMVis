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

#ifndef INCLUDE_FMUSIMULATE_HPP_
#define INCLUDE_FMUSIMULATE_HPP_

#include <string>
#include "WrapperFMILib.hpp"

// Forward Declaration
namespace Model
{
    class SimSettings;
}

#define BUFFER 1000

/// \todo Can we find a better place for this structs and functions?
///
namespace Model
{

    typedef struct
    {
        fmi1_real_t* _states;
        fmi1_real_t* _statesDer;
        fmi1_real_t* _eventIndicators;
        fmi1_real_t* _eventIndicatorsPrev;
        size_t _nStates;
        size_t _nEventIndicators;
        fmi1_status_t _fmiStatus;
        fmi1_event_info_t _eventInfo;
        fmi1_real_t _tcur;
        fmi1_real_t _hcur;
    } FMUData;


    /// MF: \todo Complete this class and remove the structs and free functions.
    class FMU
    {
     public:
        FMU();
        ~FMU() = default;

        FMU(const FMU&) = delete;
        FMU& operator=(const FMU&) = delete;

        void initialize(/*fmi1_import_t* fmu, */ Model::SimSettings* settings);
        void load(const char* FMUPath, const char* modelName);

     public:
        fmi1_import_t* _fmu;
        fmi_import_context_t* _context;  //MF Muss es fmi1_import_context_t* sein?
        jm_callbacks* _callbacks;
        fmi1_callback_functions_t _callBackFunctions;
        FMUData _fmuData;
    };


//    typedef struct
//    {
//        fmi1_import_t* _fmu;
//        fmi_import_context_t* _context;  //MF Muss es fmi1_import_context_t* sein?
//        jm_callbacks* _callbacks;
//        fmi1_callback_functions_t _callBackFunctions;
//    } fmul_t;



    //functions

    //fmuData initializeFMU(fmi1_import_t* fmu, Model::SimSettings* settings);

    //fmul_t load(const char* FMUPath, const char* modelName);

    fmi1_base_type_enu_t getFMI1baseTypeFor4CharString(const std::string typeString);

}  // End namespace Model

#endif /* INCLUDE_FMUSIMULATE_HPP_ */
