/*
 * FMU_ME_2.hpp
 *
 *  Created on: 12.12.2016
 *      Author: mf
 */

#ifndef SOURCE_DIRECTORY__INCLUDE_MODEL_FMU_FMU_ME_2_HPP_
#define SOURCE_DIRECTORY__INCLUDE_MODEL_FMU_FMU_ME_2_HPP_

#include "FMUAbstract.hpp"

namespace OMVIS
{
    namespace Model
    {

        class FMU_ME_2 : public FMUAbstract
        {
         public:

            FMU_ME_2();

            ~FMU_ME_2();

            FMU_ME_2(const FMU_ME_2&) = delete;

            FMU_ME_2& operator=(const FMU_ME_2&) = delete;

            void load(const std::string& modelFile, const std::string& path, fmi_import_context_t* mpContext);
            void initialize(const std::shared_ptr<SimSettingsFMU> simSettings);
            void setContinuousStates();
            //bool checkForTriggeredEvent();
            bool itsEventTime() const;
            void updateNextTimeStep(const double hdef);
            void handleEvents(const int intermediateResults);
            void prepareSimulationStep(const double time);
            //void setLastStepSize(const double simTimeEnd);
            void solveSystem();
            //void doEulerStep();
            void completedIntegratorStep(int* callEventUpdate);

            /// \todo Geht vermutlich auch ohne Argumente, weil man nur auf membern arbeitet!
            void do_event_iteration(fmi2_import_t *fmu, fmi2_event_info_t *eventInfo);

//            const FMUData* getFMUData();
            void fmi_get_real(unsigned int* valueRef, double* res);
            unsigned int fmi_get_variable_by_name(const char* name);

         private:
            fmi2_import_t* _fmu;
            fmi2_callback_functions_t _callBackFunctions;
            fmi2_status_t _fmiStatus;
            fmi2_event_info_t _eventInfo;
            fmi2_boolean_t _terminateSimulation;
        };

    }  // namespace Model
}  // namespace OMVIS

#endif /* SOURCE_DIRECTORY__INCLUDE_MODEL_FMU_FMU_ME_2_HPP_ */
