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

#include <iostream>
#include <cstdlib>
#include "Util/Logger.hpp"
#include "Model/FMU.hpp"
#include "Model/SimSettings.hpp"

namespace OMVIS
{
    namespace Model
    {
        void doExit()
        {
            //printf("Press 'Enter' to exit\n");
            std::cout << "Press 'Enter' to exit" << std::endl;
            /* getchar(); */
            exit(0);
        }

        FMU::FMU()
                : _fmu(nullptr),
                  _context(nullptr),
                  _callbacks(nullptr),
                  _callBackFunctions(),
                  _fmuData(),
                  _isUnzipped(false)
        {
        }

        FMU::~FMU()
        {
            // Free memory associated with the fmu and its context.
            fmi1_import_free(_fmu);
            fmi_import_free_context(_context);
            delete _callbacks;
        }

        void FMU::clear()
        {
            // Free memory associated with the fmu and its context.
            fmi1_import_free(_fmu);
            fmi_import_free_context(_context);
            _fmu = nullptr;
            _context = nullptr;

            delete _callbacks;
            _callbacks = nullptr;
            _callBackFunctions = fmi1_callback_functions_t();
            _fmuData = FMUData();
            _isUnzipped = false;
        }

        void FMU::load(const std::string FMUPath, const std::string modelName)
        {
            // First we need to define the callbacks and set up the context
            /// \todo MF: From my point of view, this can be done in the constructor.
            _callbacks = (jm_callbacks*) malloc(sizeof(jm_callbacks));
            _callbacks->malloc = malloc;
            _callbacks->calloc = calloc;
            _callbacks->realloc = realloc;
            _callbacks->free = free;
            _callbacks->logger = jm_default_logger;
            _callbacks->log_level = jm_log_level_debug;  // jm_log_level_error;
            _callbacks->context = 0;

            _callBackFunctions.logger = fmi1_log_forwarding;
            _callBackFunctions.allocateMemory = calloc;
            _callBackFunctions.freeMemory = free;

#ifdef FMILIB_GENERATE_BUILD_STAMP
            //printf("Library build stamp:\n%s\n", fmilib_get_build_stamp());
            std::cout << "Library build stamp: \n" << fmilib_get_build_stamp() << std::endl;
#endif
            _context = fmi_import_allocate_context(_callbacks);

            //unzip the fmu and pars it
            if (!_isUnzipped)
            { /* Unzip the FMU only once. Overwriting the dll/so file may cause a segfault. */
                fmi_version_enu_t version = fmi_import_get_fmi_version(_context, modelName.c_str(), FMUPath.c_str());
                if (version != fmi_version_1_enu)
                {
                    LOGGER_WRITE(std::string("Only version 1.0 is supported so far. Exiting."), Util::LC_LOADER, Util::LL_ERROR);
                    doExit();
                }
                _isUnzipped = true;
            }

            _fmu = fmi1_import_parse_xml(_context, FMUPath.c_str());
            if (!_fmu)
            {
                LOGGER_WRITE(std::string("Error parsing XML. Exiting."), Util::LC_LOADER, Util::LL_ERROR);
                doExit();
            }

            //loadFMU dll
            jm_status_enu_t status = fmi1_import_create_dllfmu(_fmu, _callBackFunctions, 1);
            if (status == jm_status_error)
            {
                LOGGER_WRITE(std::string("Could not create the DLL loading mechanism(C-API test). Exiting."), Util::LC_LOADER, Util::LL_ERROR);
                doExit();
            }
        }

        void FMU::initialize(/*fmi1_import_t* fmu,*/SimSettings* settings)
        {
            //initialize data
            _fmuData._hcur = settings->getHdef();
            _fmuData._tcur = settings->getTstart();

            LOGGER_WRITE(std::string("Version returned from FMU: ") + std::string(fmi1_import_get_version(_fmu)), Util::LC_LOADER, Util::LL_INFO);
            LOGGER_WRITE(std::string("Platform type returned: ") + std::string(fmi1_import_get_model_types_platform(_fmu)), Util::LC_LOADER, Util::LL_INFO);
            //std::cout << "Version returned from FMU: " << fmi1_import_get_version(fmu) << std::endl;
            //std::cout << "Platform type returned: " << fmi1_import_get_model_types_platform(fmu) << std::endl;

            // calloc everything
            _fmuData._nStates = fmi1_import_get_number_of_continuous_states(_fmu);
            _fmuData._nEventIndicators = fmi1_import_get_number_of_event_indicators(_fmu);
            LOGGER_WRITE(std::string("n_states: ") + std::to_string(_fmuData._nStates) + " " + std::to_string(_fmuData._nEventIndicators), Util::LC_LOADER, Util::LL_INFO);

            _fmuData._states = (fmi1_real_t*) calloc(_fmuData._nStates, sizeof(double));
            _fmuData._statesDer = (fmi1_real_t*) calloc(_fmuData._nStates, sizeof(double));
            _fmuData._eventIndicators = (fmi1_real_t*) calloc(_fmuData._nEventIndicators, sizeof(double));
            _fmuData._eventIndicatorsPrev = (fmi1_real_t*) calloc(_fmuData._nEventIndicators, sizeof(double));

            //instantiate model
            jm_status_enu_t jmstatus = fmi1_import_instantiate_model(_fmu, "Test ME model instance");
            if (jmstatus == jm_status_error)
            {
                LOGGER_WRITE(std::string("fmi1_import_instantiate_model failed. Exiting."), Util::LC_LOADER, Util::LL_ERROR);
                doExit();
            }

            //initialize
            _fmuData._fmiStatus = fmi1_import_set_time(_fmu, settings->getTstart());
            try
            {
                _fmuData._fmiStatus = fmi1_import_initialize(_fmu, settings->getToleranceControlled(), settings->getRelativeTolerance(), &_fmuData._eventInfo);
            }
            catch (std::exception &ex)
            {
                std::cout << __FILE__ << " : " << __LINE__ << " Exception: " << ex.what() << std::endl;
            }
            _fmuData._fmiStatus = fmi1_import_get_continuous_states(_fmu, _fmuData._states, _fmuData._nStates);
            _fmuData._fmiStatus = fmi1_import_get_event_indicators(_fmu, _fmuData._eventIndicatorsPrev, _fmuData._nEventIndicators);
            _fmuData._fmiStatus = fmi1_import_set_debug_logging(_fmu, fmi1_false);

            // Turn on logging in FMI library.
            fmi1_import_set_debug_logging(_fmu, fmi1_false);

            LOGGER_WRITE(std::string("FMU::initialize(). Finished."), Util::LC_LOADER, Util::LL_INFO);
        }

        /// \todo Return statement is missing.
        fmi1_base_type_enu_t getFMI1baseTypeFor4CharString(const std::string typeString)
        {
            if (!typeString.compare("real"))
                return fmi1_base_type_real;
            else if (!typeString.compare("bool"))
                return fmi1_base_type_bool;
            else if (!typeString.compare("inte"))
                return fmi1_base_type_int;
            else if (!typeString.compare("stri"))
                return fmi1_base_type_str;
            else
                std::cout << "getFMI1baseTypeFor4CharString failed" << std::endl;
        }

        bool FMU::isUnzipped() const
        {
            return _isUnzipped;
        }

        const FMUData* FMU::getFMUData() const
        {
            return &_fmuData;
        }

    /* alter Code */
    /*
     fmuData initializeFMU(fmi1_import_t* fmu, SimSettings* settings)
     {
     //initialize data
     fmuData fmuData;
     fmuData._hcur = settings->get_hdef();
     fmuData._tcur = settings->get_tstart();

     jm_status_enu_t jmstatus;

     //printf("Version returned from FMU:   %s\n", fmi1_import_get_version(fmu));
     //printf("Platform type returned:      %s\n", fmi1_import_get_model_types_platform(fmu));
     std::cout << "Version returned from FMU: " << fmi1_import_get_version(fmu) << std::endl;
     std::cout << "Platform type returned: " << fmi1_import_get_model_types_platform(fmu) << std::endl;

     // calloc everything
     fmuData._nStates = fmi1_import_get_number_of_continuous_states(fmu);
     fmuData._nEventIndicators = fmi1_import_get_number_of_event_indicators(fmu);
     //printf("n_states %d\n", fmuData.n_states);
     std::cout << "n_states: " << fmuData._nStates << std::endl;

     fmuData._states = (fmi1_real_t*) calloc(fmuData._nStates, sizeof(double));
     fmuData._statesDer = (fmi1_real_t*) calloc(fmuData._nStates, sizeof(double));
     fmuData._eventIndicators = (fmi1_real_t*) calloc(fmuData._nEventIndicators, sizeof(double));
     fmuData._eventIndicatorsPrev = (fmi1_real_t*) calloc(fmuData._nEventIndicators, sizeof(double));

     //instantiate
     jmstatus = fmi1_import_instantiate_model(fmu, "Test ME model instance");
     if (jmstatus == jm_status_error)
     {
     //printf("fmi1_import_instantiate_model failed\n");
     std::cout << "fmi1_import_instantiate_model failed. Exiting" << std::endl;
     doExit();
     }
     //initialize
     fmuData._fmiStatus = fmi1_import_set_time(fmu, settings->get_tstart());
     fmuData._fmiStatus = fmi1_import_initialize(fmu, settings->get_toleranceControlled(), settings->get_relativeTolerance(), &fmuData._eventInfo);

     fmuData._fmiStatus = fmi1_import_get_continuous_states(fmu, fmuData._states, fmuData._nStates);
     fmuData._fmiStatus = fmi1_import_get_event_indicators(fmu, fmuData._eventIndicatorsPrev, fmuData._nEventIndicators);
     fmuData._fmiStatus = fmi1_import_set_debug_logging(fmu, fmi1_false);

     //printf("fmi1_import_set_debug_logging:  %s\n", fmi1_status_to_string(fmistatus));
     fmi1_import_set_debug_logging(fmu, fmi1_true);

     return fmuData;
     }

     fmul_t load(const char *FMUPath, const char *modelName)
     {
     fmi1_callback_functions_t callBackFunctions;
     jm_callbacks* callbacks;
     fmi_import_context_t* context;
     fmi_version_enu_t version;
     jm_status_enu_t status;
     static int isunzipped;

     fmi1_import_t* fmu;

     // First we need to define the callbacks and set up the context
     callbacks = (jm_callbacks*) malloc(sizeof(jm_callbacks));
     callbacks->malloc = malloc;
     callbacks->calloc = calloc;
     callbacks->realloc = realloc;
     callbacks->free = free;
     callbacks->logger = jm_default_logger;
     callbacks->log_level = jm_log_level_debug;
     callbacks->context = 0;

     callBackFunctions.logger = fmi1_log_forwarding;
     callBackFunctions.allocateMemory = calloc;
     callBackFunctions.freeMemory = free;

     #ifdef FMILIB_GENERATE_BUILD_STAMP
     //printf("Library build stamp:\n%s\n", fmilib_get_build_stamp());
     std::cout << "Library build stamp: \n" << fmilib_get_build_stamp() << std::endl;
     #endif
     context = fmi_import_allocate_context(callbacks);

     //unzip the fmu and pars it
     if (isunzipped == 0)
     { // Unzip the FMU only once. Overwriting the dll/so file may cause a segfault.
     version = fmi_import_get_fmi_version(context, modelName, FMUPath);
     if (version != fmi_version_1_enu)
     {
     //printf("Only version 1.0 is supported so far\n");
     std::cout << "Only version 1.0 is supported so far. Exiting." << std::endl;
     doExit();
     }
     isunzipped = 1;
     }

     fmu = fmi1_import_parse_xml(context, FMUPath);
     if (!fmu)
     {
     //printf("Error parsing XML, exiting\n");
     std::cout << "Error parsing XML. Exiting." << std::endl;
     doExit();
     }

     //loadFMU dll
     status = fmi1_import_create_dllfmu(fmu, callBackFunctions, 1);
     if (status == jm_status_error)
     {
     //printf("Could not create the DLL loading mechanism(C-API test).\n");
     std::cout << "Could not create the DLL loading mechanism(C-API test). Exiting" << std::endl;
     doExit();
     }

     {
     fmul_t fmus;
     fmus._callBackFunctions = callBackFunctions;
     fmus._callbacks = callbacks;
     fmus._context = context;
     fmus._fmu = fmu;
     return fmus;
     }
     }*/

    }  // End namespace Model
}  // End namespace OMVIS
