/*
 * FMU_ME_1.hpp
 *
 *  Created on: 12.12.2016
 *      Author: mf
 */

#ifndef SOURCE_DIRECTORY__INCLUDE_MODEL_FMU_FMU_ME_1_HPP_
#define SOURCE_DIRECTORY__INCLUDE_MODEL_FMU_FMU_ME_1_HPP_

#include "FMUAbstract.hpp"

namespace OMVIS
{
    namespace Model
    {

        class FMU_ME_1 : public FMUAbstract
        {
         public:

            FMU_ME_1();

            ~FMU_ME_1();

            FMU_ME_1(const FMU_ME_1&) = delete;

            FMU_ME_1& operator=(const FMU_ME_1&) = delete;

            void load(const std::string& modelFile, const std::string& path, fmi_import_context_t* mpContext);
            void initialize(const std::shared_ptr<SimSettingsFMU> simSettings);
            //to run simulation
            //bool checkForTriggeredEvent();
            virtual bool itsEventTime() const;
            void handleEvents(const int intermediateResults);
            void prepareSimulationStep(const double time);
            void updateNextTimeStep(const double hdef);
            //void setLastStepSize(const double simTimeEnd);
            void solveSystem();
            //void doEulerStep();
            void setContinuousStates();
            void completedIntegratorStep(int* callEventUpdate);

//            const FMUData* getFMUData();
            void fmi_get_real(unsigned int* valueRef, double* res);
            unsigned int fmi_get_variable_by_name(const char* name);

         private:
            fmi1_import_t* _fmu;
            fmi1_callback_functions_t _callBackFunctions;
            fmi1_status_t _fmiStatus;
            fmi1_event_info_t _eventInfo;
        };

    }  // namespace Model
}  // namespace OMVIS

#endif /* SOURCE_DIRECTORY__INCLUDE_MODEL_FMU_FMU_ME_1_HPP_ */
