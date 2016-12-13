/*
 * FMU_ME_2.cpp
 *
 *  Created on: 13.12.2016
 *      Author: mf
 */

#include "Model/FMU/FMU_ME_2.hpp"
#include "Util/Logger.hpp"

namespace OMVIS
{
    namespace Model
    {

        /*-----------------------------------------
         * CONSTRUCTORS
         *---------------------------------------*/

        FMU_ME_2::FMU_ME_2()
                : FMUAbstract(),
                  _fmu(nullptr),
                  _callBackFunctions(),
                  _fmiStatus(/*fmi2_status_pending ?*/),
                  _eventInfo(),
                  _terminateSimulation(fmi2_false)
        {
        }

        FMU_ME_2::~FMU_ME_2()
        {
            fmi2_import_destroy_dllfmu(_fmu);
            fmi2_import_free(_fmu);
        }

        void FMU_ME_2::load(const std::string& modelFile, const std::string& path, fmi_import_context_t* context)
        {
            // First we need to define the callbacks and set up the context.
//            _callbacks.malloc = malloc;
//            _callbacks.calloc = calloc;
//            _callbacks.realloc = realloc;
//            _callbacks.free = free;
//            _callbacks.logger = jm_default_logger;
//            _callbacks.log_level = jm_log_level_debug;  // jm_log_level_error;
//            _callbacks.context = nullptr;

            _callBackFunctions.logger = fmi2_log_forwarding;
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
            _fmu = fmi2_import_parse_xml(context, path.c_str(), 0);
            if (!_fmu)
            {
                LOGGER_WRITE("Error parsing XML. Exiting.", Util::LC_LOADER, Util::LL_ERROR);
            }
            //chek if its a model excahnge FMU
            if (fmi2_import_get_fmu_kind(_fmu) != fmi2_fmu_kind_me)
            {
                std::cout << "Only Model-Exchange FMUs are supported right now." << std::endl;
            }

            jm_status_enu_t status = fmi2_import_create_dllfmu(_fmu, fmi2_fmu_kind_me, &_callBackFunctions);
            if (status == jm_status_error)
            {
                LOGGER_WRITE("Could not create the DLL loading mechanism(C-API test). Exiting.", Util::LC_LOADER,
                             Util::LL_ERROR);
            }
        }

        void FMU_ME_2::initialize(const std::shared_ptr<SimSettingsFMU> simSettings)
        {
            // Initialize data
            _hcur = simSettings->getHdef();
            _tcur = simSettings->getTstart();

            LOGGER_WRITE("Version returned from FMU: " + std::string(fmi2_import_get_version(_fmu)), Util::LC_LOADER,
                         Util::LL_INFO);
            LOGGER_WRITE("Platform type returned: " + std::string(fmi2_import_get_types_platform(_fmu)),
                         Util::LC_LOADER, Util::LL_INFO);

            // Calloc everything
            _nStates = fmi2_import_get_number_of_continuous_states(_fmu);
            _nEventIndicators = fmi2_import_get_number_of_event_indicators(_fmu);
            LOGGER_WRITE("n_states: " + std::to_string(_nStates) + " " + std::to_string(_nEventIndicators),
                         Util::LC_LOADER, Util::LL_INFO);

            _states = static_cast<double*>(calloc(_nStates, sizeof(double)));
            _statesDer = static_cast<double*>(calloc(_nStates, sizeof(double)));
            _eventIndicators = static_cast<double*>(calloc(_nEventIndicators, sizeof(double)));
            _eventIndicatorsPrev = static_cast<double*>(calloc(_nEventIndicators, sizeof(double)));

            // Instantiate model
            jm_status_enu_t jmstatus = fmi2_import_instantiate(_fmu, "Test ME model instance", fmi2_model_exchange, 0,
                                                               0);
            if (jm_status_error == jmstatus)
            {
                LOGGER_WRITE("fmi2_import_instantiate failed. Exiting.", Util::LC_LOADER, Util::LL_ERROR);
            }

            //initialize
            _fmiStatus = fmi2_import_setup_experiment(_fmu, simSettings->getToleranceControlled(),
                                                      simSettings->getRelativeTolerance(), simSettings->getTstart(),
                                                      fmi2_false, 0.0);
            try
            {
                _fmiStatus = fmi2_import_enter_initialization_mode(_fmu);
                _fmiStatus = fmi2_import_exit_initialization_mode(_fmu);
            }
            catch (std::exception &ex)
            {
                std::cout << __FILE__ << " : " << __LINE__ << " Exception: " << ex.what() << std::endl;
            }

            //set logger
            _fmiStatus = fmi2_import_set_debug_logging(_fmu, fmi2_false, 0, 0);
            printf("fmi2_import_set_debug_logging:  %s\n", fmi2_status_to_string(_fmiStatus));
            //fmi2_import_set_debug_logging(_fmu, fmi2_true, 0, 0);

            //fmiExitInitializationMode leaves FMU in event mode
            do_event_iteration(_fmu, &_eventInfo);
            fmi2_import_enter_continuous_time_mode(_fmu);

            _fmiStatus = fmi2_import_get_continuous_states(_fmu, _states, _nStates);
            _fmiStatus = fmi2_import_get_event_indicators(_fmu, _eventIndicatorsPrev, _nEventIndicators);

            LOGGER_WRITE("FMU::initialize(). Finished.", Util::LC_LOADER, Util::LL_INFO);
        }

        void FMU_ME_2::do_event_iteration(fmi2_import_t* fmu, fmi2_event_info_t* eventInfo)
        {
            eventInfo->newDiscreteStatesNeeded = fmi2_true;
            eventInfo->terminateSimulation = fmi2_false;
            while (eventInfo->newDiscreteStatesNeeded && !eventInfo->terminateSimulation)
            {
                fmi2_import_new_discrete_states(fmu, eventInfo);
            }
        }

        bool FMU_ME_2::itsEventTime() const
        {
            return (_eventInfo.nextEventTimeDefined && _tcur == _eventInfo.nextEventTime);
        }

        void FMU_ME_2::handleEvents(const int intermediateResults)
        {
            //std::cout<<"Handle event at "<<std::to_string(_tcur)<<std::endl;
            _fmiStatus = fmi2_import_enter_event_mode(_fmu);
            do_event_iteration(_fmu, &_eventInfo);
            _fmiStatus = fmi2_import_enter_continuous_time_mode(_fmu);
            _fmiStatus = fmi2_import_get_continuous_states(_fmu, _states, _nStates);
            _fmiStatus = fmi2_import_get_event_indicators(_fmu, _eventIndicators, _nEventIndicators);
        }

        void FMU_ME_2::prepareSimulationStep(const double time)
        {
            _fmiStatus = fmi2_import_set_time(_fmu, time);
            _fmiStatus = fmi2_import_get_event_indicators(_fmu, _eventIndicators, _nEventIndicators);
        }

        void FMU_ME_2::updateNextTimeStep(const double hdef)
        {
            double tlast = _tcur;
            _tcur += hdef;
            if (_eventInfo.nextEventTimeDefined && (_tcur >= _eventInfo.nextEventTime))
            {
                _tcur = _eventInfo.nextEventTime;
            }
            _hcur = _tcur - tlast;
        }

        void FMU_ME_2::solveSystem()
        {
            _fmiStatus = fmi2_import_get_derivatives(_fmu, _statesDer, _nStates);
        }

        void FMU_ME_2::setContinuousStates()
        {
            _fmiStatus = fmi2_import_set_continuous_states(_fmu, _states, _nStates);
        }

        void FMU_ME_2::completedIntegratorStep(int* callEventUpdate)
        {
            _fmiStatus = fmi2_import_completed_integrator_step(_fmu, fmi2_true, (fmi2_boolean_t*) callEventUpdate,
                                                               &_terminateSimulation);
        }

        //const FMUData* getFMUData();

        void FMU_ME_2::fmi_get_real(unsigned int* valueRef, double* res)
        {
            fmi2_import_get_real(_fmu, valueRef, 1, res);
        }

        unsigned int FMU_ME_2::fmi_get_variable_by_name(const char* name)
        {
            fmi2_import_variable_t* var = fmi2_import_get_variable_by_name(_fmu, name);
            return (unsigned int) fmi2_import_get_variable_vr(var);
        }

    }   // namespace Model
}  // namespace OMVIS
