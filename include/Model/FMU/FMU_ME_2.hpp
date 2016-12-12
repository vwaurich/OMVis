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

        class FMUWrapper_ME_2 : public FMUAbstract
        {
         public:

            FMUWrapper_ME_2();
            ~FMUWrapper_ME_2();

            void load(const std::string& modelFile, const std::string& path, fmi_import_context_t* mpContext);
            void initialize(const std::shared_ptr<SimSettingsFMU> simSettings);
            void setContinuousStates();
            bool checkForTriggeredEvent();
            bool itsEventTime();
            void updateNextTimeStep(const double hdef);
            void handleEvents(const int intermediateResults);
            void prepareSimulationStep(const double time);
            void setLastStepSize(const double simTimeEnd);
            void solveSystem();
            void doEulerStep();
            void completedIntegratorStep(int* callEventUpdate);
            void do_event_iteration(fmi2_import_t *fmu, fmi2_event_info_t *eventInfo);

            const FMUData* getFMUData();
            void fmi_get_real(unsigned int* valueRef, double* res);
            unsigned int fmi_get_variable_by_name(const char* name);

         private:
            fmi2_import_t* mpFMU;
            fmi2_callback_functions_t mCallBackFunctions;
            FMUData mFMUdata;
        };

    }  // namespace Model
}  // namespace OMVIS

#endif /* SOURCE_DIRECTORY__INCLUDE_MODEL_FMU_FMU_ME_2_HPP_ */
