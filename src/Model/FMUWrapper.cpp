/*
 * Copyright (C) 2016, Volker Waurich
 *
 * This file is part of OMVIS.
 *
 * OMVIS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OMVIS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OMVIS.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Util/Logger.hpp"
#include "Util/Util.hpp"
#include <FMI/fmi_import_util.h>
#include <Model/FMUWrapper.hpp>

#include <iostream>

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

        FMUWrapper::FMUWrapper()
                : _fmu(nullptr),
                  _context(nullptr),
                  _callbacks(),
                  _callBackFunctions(),
                  _fmuData()
        {
        }

        FMUWrapper::~FMUWrapper()
        {
            // Free memory associated with the FMUData and its context.
            delete _fmuData._states;
            delete _fmuData._statesDer;
            delete _fmuData._eventIndicators;
            delete _fmuData._eventIndicatorsPrev;
        }

        /*-----------------------------------------
         * INITIALIZATION METHODS
         *---------------------------------------*/

        void FMUWrapper::load(const std::string& modelFile, const std::string& path)
        {
            // First we need to define the callbacks and set up the context.
            _callbacks.malloc = malloc;
            _callbacks.calloc = calloc;
            _callbacks.realloc = realloc;
            _callbacks.free = free;
            _callbacks.logger = jm_default_logger;
            _callbacks.log_level = jm_log_level_debug;  // jm_log_level_error;
            _callbacks.context = nullptr;

            _callBackFunctions.logger = fmi1_log_forwarding;
            _callBackFunctions.allocateMemory = calloc;
            _callBackFunctions.freeMemory = free;

#ifdef FMILIB_GENERATE_BUILD_STAMP
            //printf("Library build stamp:\n%s\n", fmilib_get_build_stamp());
            std::cout << "Library build stamp: \n" << fmilib_get_build_stamp() << std::endl;
#endif
            _context = std::shared_ptr<fmi_import_context_t>(fmi_import_allocate_context(&_callbacks),
                                                             fmi_import_free_context);

            // If the FMU is already extracted, we remove the shared object file.
            std::string sharedObjectFile(fmi_import_get_dll_path(path.c_str(), modelFile.c_str(), &_callbacks));
            if (Util::fileExists(sharedObjectFile))
            {
                if (0 != remove(sharedObjectFile.c_str()))
                {
                    LOGGER_WRITE("Error deleting the shared object file " + sharedObjectFile + ".", Util::LC_LOADER,
                                 Util::LL_ERROR);
                }
                else
                {
                    LOGGER_WRITE("Shared object file " + sharedObjectFile + " deleted.", Util::LC_LOADER,
                                 Util::LL_DEBUG);
                }
            }
            else
            {
                LOGGER_WRITE("Shared object file " + sharedObjectFile + " does not exist.", Util::LC_LOADER,
                             Util::LL_DEBUG);
            }

            // Unzip the FMU and pars it.
            // Unzip the FMU only once. Overwriting the dll/so file may cause a segmentation fault.
            std::string fmuFileName = path + modelFile;
            fmi_version_enu_t version = fmi_import_get_fmi_version(_context.get(), fmuFileName.c_str(), path.c_str());
            if (fmi_version_1_enu != version)
            {
                LOGGER_WRITE("Only version 1.0 is supported so far. Exiting.", Util::LC_LOADER, Util::LL_ERROR);
                doExit();
            }

            _fmu = std::shared_ptr<fmi1_import_t>(fmi1_import_parse_xml(_context.get(), path.c_str()),
                                                  fmi1_import_free);
            if (!_fmu)
            {
                LOGGER_WRITE("Error parsing XML. Exiting.", Util::LC_LOADER, Util::LL_ERROR);
                doExit();
            }

            //loadFMU dll
            jm_status_enu_t status = fmi1_import_create_dllfmu(_fmu.get(), _callBackFunctions, 1);
            if (jm_status_error == status)
            {
                LOGGER_WRITE("Could not create the DLL loading mechanism(C-API test). Exiting.", Util::LC_LOADER,
                             Util::LL_ERROR);
                doExit();
            }
        }

        void FMUWrapper::initialize(const std::shared_ptr<SimSettingsFMU> simSettings)
        {
            // Initialize data
            _fmuData._hcur = simSettings->getHdef();
            _fmuData._tcur = simSettings->getTstart();

            LOGGER_WRITE("Version returned from FMU: " + std::string(fmi1_import_get_version(_fmu.get())),
                         Util::LC_LOADER, Util::LL_INFO);
            LOGGER_WRITE("Platform type returned: " + std::string(fmi1_import_get_model_types_platform(_fmu.get())),
                         Util::LC_LOADER, Util::LL_INFO);

            // Calloc everything
            _fmuData._nStates = fmi1_import_get_number_of_continuous_states(_fmu.get());
            _fmuData._nEventIndicators = fmi1_import_get_number_of_event_indicators(_fmu.get());
            LOGGER_WRITE(
                    "n_states: " + std::to_string(_fmuData._nStates) + " " + std::to_string(_fmuData._nEventIndicators),
                    Util::LC_LOADER, Util::LL_INFO);

            _fmuData._states = static_cast<fmi1_real_t*>(calloc(_fmuData._nStates, sizeof(double)));
            _fmuData._statesDer = static_cast<fmi1_real_t*>(calloc(_fmuData._nStates, sizeof(double)));
            _fmuData._eventIndicators = static_cast<fmi1_real_t*>(calloc(_fmuData._nEventIndicators, sizeof(double)));
            _fmuData._eventIndicatorsPrev =
                    static_cast<fmi1_real_t*>(calloc(_fmuData._nEventIndicators, sizeof(double)));

            // Instantiate model
            jm_status_enu_t jmstatus = fmi1_import_instantiate_model(_fmu.get(), "Test ME model instance");
            if (jm_status_error == jmstatus)
            {
                LOGGER_WRITE("fmi1_import_instantiate_model failed. Exiting.", Util::LC_LOADER, Util::LL_ERROR);
                doExit();
            }

            //initialize
            _fmuData._fmiStatus = fmi1_import_set_time(_fmu.get(), simSettings->getTstart());
            _fmuData._fmiStatus = fmi1_import_initialize(_fmu.get(), simSettings->getToleranceControlled(),
                                                         simSettings->getRelativeTolerance(), &_fmuData._eventInfo);
            _fmuData._fmiStatus = fmi1_import_get_continuous_states(_fmu.get(), _fmuData._states, _fmuData._nStates);
            _fmuData._fmiStatus = fmi1_import_get_event_indicators(_fmu.get(), _fmuData._eventIndicatorsPrev,
                                                                   _fmuData._nEventIndicators);
            _fmuData._fmiStatus = fmi1_import_set_debug_logging(_fmu.get(), fmi1_false);

            // Turn on logging in FMI library.
            fmi1_import_set_debug_logging(_fmu.get(), fmi1_false);

            LOGGER_WRITE("FMU::initialize(). Finished.", Util::LC_LOADER, Util::LL_INFO);
        }

        /*-----------------------------------------
         * GETTERS and SETTERS
         *---------------------------------------*/

        const FMUData* FMUWrapper::getFMUData() const
        {
            return &_fmuData;
        }

        fmi1_import_t* FMUWrapper::getFMU() const
        {
            return _fmu.get();
        }

        double FMUWrapper::getTcur() const
        {
            return _fmuData._tcur;
        }

        void FMUWrapper::setContinuousStates()
        {
            _fmuData._fmiStatus = fmi1_import_set_continuous_states(_fmu.get(), _fmuData._states, _fmuData._nStates);
        }

        /*-----------------------------------------
         * SIMULATION METHODS
         *---------------------------------------*/

        bool FMUWrapper::checkForTriggeredEvent() const
        {
            for (size_t k = 0; k < _fmuData._nEventIndicators; ++k)
            {
                if (_fmuData._eventIndicators[k] * _fmuData._eventIndicatorsPrev[k] < 0)
                {
                    LOGGER_WRITE("Event occurred at " + std::to_string(_fmuData._tcur), Util::LC_CTR, Util::LL_DEBUG);
                    return true;
                }
            }
            return false;
        }

        bool FMUWrapper::itsEventTime() const
        {
            return ((0 != _fmuData._eventInfo.upcomingTimeEvent) && _fmuData._tcur == _fmuData._eventInfo.nextEventTime);
        }

        void FMUWrapper::updateNextTimeStep(const fmi1_real_t hdef)
        {
            if (0 != _fmuData._eventInfo.upcomingTimeEvent)
            {
                if (_fmuData._tcur + hdef < _fmuData._eventInfo.nextEventTime)
                {
                    _fmuData._hcur = hdef;
                }
                else
                {
                    _fmuData._hcur = _fmuData._eventInfo.nextEventTime - _fmuData._tcur;
                }
            }
            else
            {
                _fmuData._hcur = hdef;
            }

            // Increase with step size
            _fmuData._tcur += _fmuData._hcur;
        }

        void FMUWrapper::fmi1ImportGetDerivatives()
        {
            _fmuData._fmiStatus = fmi1_import_get_derivatives(_fmu.get(), _fmuData._statesDer, _fmuData._nStates);
        }

        void FMUWrapper::handleEvents(const fmi1_boolean_t intermediateResults)
        {
            // LOGGER_WRITE("Handle event at " + std::to_string(_fmuData._tcur), Util::LC_CTR, Util::LL_DEBUG);
            _fmuData._fmiStatus = fmi1_import_eventUpdate(_fmu.get(), intermediateResults, &_fmuData._eventInfo);
            _fmuData._fmiStatus = fmi1_import_get_continuous_states(_fmu.get(), _fmuData._states, _fmuData._nStates);
            _fmuData._fmiStatus = fmi1_import_get_event_indicators(_fmu.get(), _fmuData._eventIndicators,
                                                                   _fmuData._nEventIndicators);
            _fmuData._fmiStatus = fmi1_import_get_event_indicators(_fmu.get(), _fmuData._eventIndicatorsPrev,
                                                                   _fmuData._nEventIndicators);
        }

        void FMUWrapper::prepareSimulationStep(const double time)
        {
            _fmuData._fmiStatus = fmi1_import_set_time(_fmu.get(), time);
            _fmuData._fmiStatus = fmi1_import_get_event_indicators(_fmu.get(), _fmuData._eventIndicators,
                                                                   _fmuData._nEventIndicators);
        }

        void FMUWrapper::updateTimes(const double simTimeEnd)
        {
            if (_fmuData._tcur > simTimeEnd - _fmuData._hcur / 1e16)
            {
                _fmuData._tcur -= _fmuData._hcur;
                _fmuData._hcur = simTimeEnd - _fmuData._tcur;
                _fmuData._tcur = simTimeEnd;
            }
        }

        void FMUWrapper::solveSystem()
        {
            _fmuData._fmiStatus = fmi1_import_get_derivatives(_fmu.get(), _fmuData._statesDer, _fmuData._nStates);
        }

        void FMUWrapper::doEulerStep()
        {
            for (size_t k = 0; k < _fmuData._nStates; ++k)
            {
                _fmuData._states[k] = _fmuData._states[k] + _fmuData._hcur * _fmuData._statesDer[k];
            }
        }

        void FMUWrapper::completedIntegratorStep(fmi1_boolean_t* callEventUpdate)
        {
            _fmuData._fmiStatus = fmi1_import_completed_integrator_step(_fmu.get(), callEventUpdate);
        }

        /*-----------------------------------------
         * FREE METHODS
         *---------------------------------------*/

        /// \todo Return statement is missing.
        fmi1_base_type_enu_t getFMI1baseTypeFor4CharString(const std::string& typeString)
        {
            if (0 == typeString.compare("real"))
            {
                return fmi1_base_type_real;
            }
            if (0 == typeString.compare("bool"))
            {
                return fmi1_base_type_bool;
            }
            if (0 == typeString.compare("inte"))
            {
                return fmi1_base_type_int;
            }
            if (0 == typeString.compare("stri"))
            {
                return fmi1_base_type_str;
            }

            std::cout << "getFMI1baseTypeFor4CharString failed" << std::endl;
            return fmi1_base_type_enum;
        }

    }  // namespace Model
}  // namespace OMVIS
