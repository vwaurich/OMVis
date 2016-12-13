/*
 * FMU_ME_1.cpp
 *
 *  Created on: 13.12.2016
 *      Author: mf
 */

#include "Model/FMU/FMU_ME_1.hpp"
#include "Util/Logger.hpp"

namespace OMVIS
{
    namespace Model
    {

        /*-----------------------------------------
         * CONSTRUCTORS
         *---------------------------------------*/

        FMU_ME_1::FMU_ME_1()
                : FMUAbstract(),
                  _fmu(nullptr),
                  _callBackFunctions(),
                  _fmiStatus(),
                  _eventInfo()
        {
        }

        FMU_ME_1::~FMU_ME_1()
        {
            fmi1_import_destroy_dllfmu(_fmu);
            fmi1_import_free(_fmu);
        }

        void FMU_ME_1::load(const std::string& modelFile, const std::string& path, fmi_import_context_t* context)
        {
            // First we need to define the callbacks and set up the context.
//            _callbacks.malloc = malloc;
//            _callbacks.calloc = calloc;
//            _callbacks.realloc = realloc;
//            _callbacks.free = free;
//            _callbacks.logger = jm_default_logger;
//            _callbacks.log_level = jm_log_level_debug;  // jm_log_level_error;
//            _callbacks.context = nullptr;

            _callBackFunctions.logger = fmi1_log_forwarding;
            _callBackFunctions.allocateMemory = calloc;
            _callBackFunctions.freeMemory = free;

#ifdef FMILIB_GENERATE_BUILD_STAMP
            //printf("Library build stamp:\n%s\n", fmilib_get_build_stamp());
            std::cout << "Library build stamp: \n" << fmilib_get_build_stamp() << std::endl;
#endif

            // MF: Try to use OMEdit way, see above.
//            _context = std::shared_ptr<fmi_import_context_t>(fmi_import_allocate_context(&_callbacks),
//                                                             fmi_import_free_context);

// If the FMU is already extracted, we remove the shared object file.
//            std::string sharedObjectFile(fmi_import_get_dll_path(path.c_str(), modelFile.c_str(), &_callbacks));
//            if (Util::fileExists(sharedObjectFile))
//            {
//                if (0 != remove(sharedObjectFile.c_str()))
//                {
//                    LOGGER_WRITE("Error deleting the shared object file " + sharedObjectFile + ".", Util::LC_LOADER,
//                                 Util::LL_ERROR);
//                }
//                else
//                {
//                    LOGGER_WRITE("Shared object file " + sharedObjectFile + " deleted.", Util::LC_LOADER,
//                                 Util::LL_DEBUG);
//                }
//            }
//            else
//            {
//                LOGGER_WRITE("Shared object file " + sharedObjectFile + " does not exist.", Util::LC_LOADER,
//                             Util::LL_DEBUG);
//            }
//            // Unzip the FMU and pars it.
//            // Unzip the FMU only once. Overwriting the dll/so file may cause a segmentation fault.
//            std::string fmuFileName = path + modelFile;
//            fmi_version_enu_t version = fmi_import_get_fmi_version(_context.get(), fmuFileName.c_str(), path.c_str());
//            if (fmi_version_1_enu != version)
//            {
//                LOGGER_WRITE("Only version 1.0 is supported so far. Exiting.", Util::LC_LOADER, Util::LL_ERROR);
//                doExit();
//            }
//
//            _fmu = std::shared_ptr<fmi1_import_t>(fmi1_import_parse_xml(_context.get(), path.c_str()),
//                                                  fmi1_import_free);

// See OMEdit
            _fmu = fmi1_import_parse_xml(context, path.c_str());
            if (!_fmu)
            {
                LOGGER_WRITE("Error parsing XML. Exiting.", Util::LC_LOADER, Util::LL_ERROR);
            }
            //chek if its a model excahnge FMU
            if (fmi1_import_get_fmu_kind(_fmu) != fmi1_fmu_kind_enu_me)
            {
                std::cout << "Only Model-Exchange FMUs are supported right now." << std::endl;
            }

            jm_status_enu_t status = fmi1_import_create_dllfmu(_fmu, _callBackFunctions, 1);
            if (status == jm_status_error)
            {
                LOGGER_WRITE("Could not create the DLL loading mechanism(C-API test). Exiting.", Util::LC_LOADER,
                             Util::LL_ERROR);
            }
        }

        void FMU_ME_1::initialize(const std::shared_ptr<SimSettingsFMU> simSettings)
        {
            // Initialize data
            _hcur = simSettings->getHdef();
            _tcur = simSettings->getTstart();

            LOGGER_WRITE("Version returned from FMU: " + std::string(fmi1_import_get_version(_fmu)), Util::LC_LOADER,
                         Util::LL_INFO);
            LOGGER_WRITE("Platform type returned: " + std::string(fmi1_import_get_model_types_platform(_fmu)),
                         Util::LC_LOADER, Util::LL_INFO);

            // Calloc everything
            _nStates = fmi1_import_get_number_of_continuous_states(_fmu);
            _nEventIndicators = fmi1_import_get_number_of_event_indicators(_fmu);
            LOGGER_WRITE("n_states: " + std::to_string(_nStates) + " " + std::to_string(_nEventIndicators),
                         Util::LC_LOADER, Util::LL_INFO);

            _states = static_cast<fmi1_real_t*>(calloc(_nStates, sizeof(double)));
            _statesDer = static_cast<fmi1_real_t*>(calloc(_nStates, sizeof(double)));
            _eventIndicators = static_cast<fmi1_real_t*>(calloc(_nEventIndicators, sizeof(double)));
            _eventIndicatorsPrev = static_cast<fmi1_real_t*>(calloc(_nEventIndicators, sizeof(double)));

            // Instantiate model
            jm_status_enu_t jmstatus = fmi1_import_instantiate_model(_fmu, "Test ME model instance");
            if (jm_status_error == jmstatus)
            {
                LOGGER_WRITE("fmi1_import_instantiate_model failed. Exiting.", Util::LC_LOADER, Util::LL_ERROR);
            }

            //initialize
            _fmiStatus = fmi1_import_set_time(_fmu, simSettings->getTstart());
            try
            {
                _fmiStatus = fmi1_import_initialize(_fmu, simSettings->getToleranceControlled(),
                                                    simSettings->getRelativeTolerance(), &_eventInfo);
            }
            catch (std::exception &ex)
            {
                std::cout << __FILE__ << " : " << __LINE__ << " Exception: " << ex.what() << std::endl;
            }
            _fmiStatus = fmi1_import_get_continuous_states(_fmu, _states, _nStates);
            _fmiStatus = fmi1_import_get_event_indicators(_fmu, _eventIndicatorsPrev, _nEventIndicators);
            _fmiStatus = fmi1_import_set_debug_logging(_fmu, fmi1_false);

            // Turn on logging in FMI library.
            fmi1_import_set_debug_logging(_fmu, fmi1_false);
            LOGGER_WRITE("FMU::initialize(). Finished.", Util::LC_LOADER, Util::LL_INFO);
        }

        bool FMU_ME_1::itsEventTime() const
        {
            return (_eventInfo.upcomingTimeEvent && _tcur == _eventInfo.nextEventTime);
        }

        void FMU_ME_1::handleEvents(const int intermediateResults)
        {
            //std::cout<<"Handle event at "<<std::to_string(_tcur)<<std::endl;
            _fmiStatus = fmi1_import_eventUpdate(_fmu, intermediateResults, &_eventInfo);
            _fmiStatus = fmi1_import_get_continuous_states(_fmu, _states, _nStates);
            _fmiStatus = fmi1_import_get_event_indicators(_fmu, _eventIndicators, _nEventIndicators);
            _fmiStatus = fmi1_import_get_event_indicators(_fmu, _eventIndicatorsPrev, _nEventIndicators);
        }

        void FMU_ME_1::prepareSimulationStep(const double time)
        {
            _fmiStatus = fmi1_import_set_time(_fmu, time);
            _fmiStatus = fmi1_import_get_event_indicators(_fmu, _eventIndicators, _nEventIndicators);
        }

        void FMU_ME_1::updateNextTimeStep(const double hdef)
        {
            if (0 != _eventInfo.upcomingTimeEvent)
            {
                if (_tcur + hdef < _eventInfo.nextEventTime)
                {
                    _hcur = hdef;
                }
                else
                {
                    _hcur = _eventInfo.nextEventTime - _tcur;
                }
            }
            else
            {
                _hcur = hdef;
            }

            // Increase with step size
            _tcur += _hcur;
        }

        void FMU_ME_1::solveSystem()
        {
            _fmiStatus = fmi1_import_get_derivatives(_fmu, _statesDer, _nStates);
        }

        void FMU_ME_1::setContinuousStates()
        {
            _fmiStatus = fmi1_import_set_continuous_states(_fmu, _states, _nStates);
        }

        void FMU_ME_1::completedIntegratorStep(int* callEventUpdate)
        {
            _fmiStatus = fmi1_import_completed_integrator_step(_fmu, (char*) callEventUpdate);
        }

        //const FMUData* getFMUData();

        void FMU_ME_1::fmi_get_real(unsigned int* valueRef, double* res)
        {
            fmi1_import_get_real(_fmu, valueRef, 1, res);
        }

        unsigned int FMU_ME_1::fmi_get_variable_by_name(const char* name)
        {
            fmi1_import_variable_t* var = fmi1_import_get_variable_by_name(_fmu, name);
            return (unsigned int) fmi1_import_get_variable_vr(var);
        }

    }   // namespace Model
}  // namespace OMVIS
