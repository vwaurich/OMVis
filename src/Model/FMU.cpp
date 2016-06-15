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

#include "Util/Logger.hpp"
#include "Util/Util.hpp"
#include "Model/FMU.hpp"
#include "Model/SimSettings.hpp"

#include <FMI/fmi_import_util.h>

#include <iostream>
#include <cstdlib>
#include <stdio.h>

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

        /*-----------------------------------------
         * CONSTRUCTORS
         *---------------------------------------*/

        FMU::FMU()
                : _fmu(nullptr),
                  _context(nullptr),
                  _callbacks(),
                  _callBackFunctions(),
                  _fmuData()
        {
        }

        FMU::~FMU()
        {
            // Free memory associated with the FMUData and its context.
            if (_fmuData._states)
                delete (_fmuData._states);
            if (_fmuData._statesDer)
                delete (_fmuData._statesDer);
            if (_fmuData._eventIndicators)
                delete (_fmuData._eventIndicators);
            if (_fmuData._eventIndicatorsPrev)
                delete (_fmuData._eventIndicatorsPrev);
        }

        /*-----------------------------------------
         * INITIALIZATION METHODS
         *---------------------------------------*/

        void FMU::initialize(std::shared_ptr<SimSettings> settings)
        {
            //initialize data
            _fmuData._hcur = settings->getHdef();
            _fmuData._tcur = settings->getTstart();

            LOGGER_WRITE(std::string("Version returned from FMU: ") + std::string(fmi1_import_get_version(_fmu.get())), Util::LC_LOADER, Util::LL_INFO);
            LOGGER_WRITE(std::string("Platform type returned: ") + std::string(fmi1_import_get_model_types_platform(_fmu.get())), Util::LC_LOADER, Util::LL_INFO);

            // calloc everything
            _fmuData._nStates = fmi1_import_get_number_of_continuous_states(_fmu.get());
            _fmuData._nEventIndicators = fmi1_import_get_number_of_event_indicators(_fmu.get());
            LOGGER_WRITE(std::string("n_states: ") + std::to_string(_fmuData._nStates) + " " + std::to_string(_fmuData._nEventIndicators), Util::LC_LOADER, Util::LL_INFO);

            _fmuData._states = (fmi1_real_t*) calloc(_fmuData._nStates, sizeof(double));
            _fmuData._statesDer = (fmi1_real_t*) calloc(_fmuData._nStates, sizeof(double));
            _fmuData._eventIndicators = (fmi1_real_t*) calloc(_fmuData._nEventIndicators, sizeof(double));
            _fmuData._eventIndicatorsPrev = (fmi1_real_t*) calloc(_fmuData._nEventIndicators, sizeof(double));

            //instantiate model
            jm_status_enu_t jmstatus = fmi1_import_instantiate_model(_fmu.get(), "Test ME model instance");
            if (jmstatus == jm_status_error)
            {
                LOGGER_WRITE(std::string("fmi1_import_instantiate_model failed. Exiting."), Util::LC_LOADER, Util::LL_ERROR);
                doExit();
            }

            //initialize
            _fmuData._fmiStatus = fmi1_import_set_time(_fmu.get(), settings->getTstart());
            try
            {
                _fmuData._fmiStatus = fmi1_import_initialize(_fmu.get(), settings->getToleranceControlled(), settings->getRelativeTolerance(), &_fmuData._eventInfo);
            }
            catch (std::exception &ex)
            {
                std::cout << __FILE__ << " : " << __LINE__ << " Exception: " << ex.what() << std::endl;
            }
            _fmuData._fmiStatus = fmi1_import_get_continuous_states(_fmu.get(), _fmuData._states, _fmuData._nStates);
            _fmuData._fmiStatus = fmi1_import_get_event_indicators(_fmu.get(), _fmuData._eventIndicatorsPrev, _fmuData._nEventIndicators);
            _fmuData._fmiStatus = fmi1_import_set_debug_logging(_fmu.get(), fmi1_false);

            // Turn on logging in FMI library.
            fmi1_import_set_debug_logging(_fmu.get(), fmi1_false);

            LOGGER_WRITE(std::string("FMU::initialize(). Finished."), Util::LC_LOADER, Util::LL_INFO);
        }

        void FMU::load(const std::string& modelFile, const std::string& path)
        {
            // First we need to define the callbacks and set up the context.
            _callbacks.malloc = malloc;
            _callbacks.calloc = calloc;
            _callbacks.realloc = realloc;
            _callbacks.free = free;
            _callbacks.logger = jm_default_logger;
            _callbacks.log_level = jm_log_level_debug;  // jm_log_level_error;
            _callbacks.context = 0;

            _callBackFunctions.logger = fmi1_log_forwarding;
            _callBackFunctions.allocateMemory = calloc;
            _callBackFunctions.freeMemory = free;

#ifdef FMILIB_GENERATE_BUILD_STAMP
            //printf("Library build stamp:\n%s\n", fmilib_get_build_stamp());
            std::cout << "Library build stamp: \n" << fmilib_get_build_stamp() << std::endl;
#endif
            _context = std::shared_ptr<fmi_import_context_t>(fmi_import_allocate_context(&_callbacks), fmi_import_free_context);

            // If the FMU is already extracted, we remove the shared object file.
            std::string sharedObjectFile(fmi_import_get_dll_path(path.c_str(), modelFile.c_str(), &_callbacks));
            if (Util::fileExists(sharedObjectFile))
            {
                if (remove(sharedObjectFile.c_str()) != 0)
                    LOGGER_WRITE(std::string("Error deleting the shared object file " + sharedObjectFile + std::string(".")), Util::LC_LOADER, Util::LL_ERROR);
                else
                    LOGGER_WRITE(std::string("Shared object file " + sharedObjectFile + std::string(" deleted.")), Util::LC_LOADER, Util::LL_DEBUG);
            }
            else
                LOGGER_WRITE(std::string("Shared object file " + sharedObjectFile + std::string(" does not exist.")), Util::LC_LOADER, Util::LL_DEBUG);

            // Unzip the FMU and pars it.
            // Unzip the FMU only once. Overwriting the dll/so file may cause a segmentation fault.
            // std::string fmuFileName = dirPath + fileName + ".fmu";
            // fileName now has .fmu prefix. So do not add it again.
            std::string fmuFileName = path + modelFile;
            fmi_version_enu_t version = fmi_import_get_fmi_version(_context.get(), fmuFileName.c_str(), path.c_str());
            if (version != fmi_version_1_enu)
            {
                LOGGER_WRITE(std::string("Only version 1.0 is supported so far. Exiting."), Util::LC_LOADER, Util::LL_ERROR);
                doExit();
            }

            _fmu = std::shared_ptr<fmi1_import_t>(fmi1_import_parse_xml(_context.get(), path.c_str()), fmi1_import_free);
            if (!_fmu)
            {
                LOGGER_WRITE(std::string("Error parsing XML. Exiting."), Util::LC_LOADER, Util::LL_ERROR);
                doExit();
            }

            //loadFMU dll
            jm_status_enu_t status = fmi1_import_create_dllfmu(_fmu.get(), _callBackFunctions, 1);
            if (status == jm_status_error)
            {
                LOGGER_WRITE(std::string("Could not create the DLL loading mechanism(C-API test). Exiting."), Util::LC_LOADER, Util::LL_ERROR);
                doExit();
            }
        }

        /*-----------------------------------------
         * GETTERS and SETTERS
         *---------------------------------------*/

        const FMUData* FMU::getFMUData() const
        {
            return &_fmuData;
        }

        fmi1_import_t* FMU::getFMU() const
        {
            return _fmu.get();
        }

        double FMU::getTcur()
        {
            return _fmuData._tcur;
        }

        /*-----------------------------------------
         * SIMULATION METHODS
         *---------------------------------------*/

        bool FMU::checkForTriggeredEvent() const
        {
            for (size_t k = 0; k < _fmuData._nEventIndicators; ++k)
            {
                if (_fmuData._eventIndicators[k] * _fmuData._eventIndicatorsPrev[k] < 0)
                {
                    LOGGER_WRITE(std::string("Event occurred at ") + std::to_string(_fmuData._tcur), Util::LC_CTR, Util::LL_DEBUG);
                    //break;
                    return true;
                }
            }
            return false;
        }

        bool FMU::itsEventTime() const
        {
            return (_fmuData._eventInfo.upcomingTimeEvent && _fmuData._tcur == _fmuData._eventInfo.nextEventTime);
        }

        void FMU::updateNextTimeStep(const fmi1_real_t hdef)
        {
            if (_fmuData._eventInfo.upcomingTimeEvent)
            {
                if (_fmuData._tcur + hdef < _fmuData._eventInfo.nextEventTime)
                    _fmuData._hcur = hdef;
                else
                    _fmuData._hcur = _fmuData._eventInfo.nextEventTime - _fmuData._tcur;
            }
            else
                _fmuData._hcur = hdef;

            /* increase with step size*/
            _fmuData._tcur += _fmuData._hcur;
        }

        void FMU::fmi1ImportGetDerivatives()
        {
            _fmuData._fmiStatus = fmi1_import_get_derivatives(_fmu.get(), _fmuData._statesDer, _fmuData._nStates);
        }

        void FMU::handleEvents(const fmi1_boolean_t intermediateResults)
        {
            std::cout << "HANDLE EVENT at " << _fmuData._tcur << std::endl;
            _fmuData._fmiStatus = fmi1_import_eventUpdate(_fmu.get(), intermediateResults, &_fmuData._eventInfo);
            _fmuData._fmiStatus = fmi1_import_get_continuous_states(_fmu.get(), _fmuData._states, _fmuData._nStates);
            _fmuData._fmiStatus = fmi1_import_get_event_indicators(_fmu.get(), _fmuData._eventIndicators, _fmuData._nEventIndicators);
            _fmuData._fmiStatus = fmi1_import_get_event_indicators(_fmu.get(), _fmuData._eventIndicatorsPrev, _fmuData._nEventIndicators);
        }

        void FMU::prepareSimulationStep(const double time)
        {
            _fmuData._fmiStatus = fmi1_import_set_time(_fmu.get(), time);
            _fmuData._fmiStatus = fmi1_import_get_event_indicators(_fmu.get(), _fmuData._eventIndicators, _fmuData._nEventIndicators);
        }

        /// @todo TODO Remove hard coded value? Let the user specify the value.
        void FMU::updateTimes(const double simTimeEnd)
        {
            if (_fmuData._tcur > simTimeEnd - _fmuData._hcur / 1e16)
            {
                _fmuData._tcur -= _fmuData._hcur;
                _fmuData._hcur = simTimeEnd - _fmuData._tcur;
                _fmuData._tcur = simTimeEnd;
            }
        }

        void FMU::solveSystem()
        {
            _fmuData._fmiStatus = fmi1_import_get_derivatives(_fmu.get(), _fmuData._statesDer, _fmuData._nStates);
        }

        void FMU::doEulerStep()
        {
            for (size_t k = 0; k < _fmuData._nStates; ++k)
                _fmuData._states[k] = _fmuData._states[k] + _fmuData._hcur * _fmuData._statesDer[k];
        }

        void FMU::setContinuousStates()
        {
            _fmuData._fmiStatus = fmi1_import_set_continuous_states(_fmu.get(), _fmuData._states, _fmuData._nStates);
        }

        void FMU::completedIntegratorStep(fmi1_boolean_t* callEventUpdate)
        {
            _fmuData._fmiStatus = fmi1_import_completed_integrator_step(_fmu.get(), callEventUpdate);
        }

        /*-----------------------------------------
         * FREE METHODS
         *---------------------------------------*/
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

    }
// End namespace Model
}// End namespace OMVIS
