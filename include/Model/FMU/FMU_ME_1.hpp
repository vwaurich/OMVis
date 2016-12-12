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

        class FMUWrapper_ME_1 : public FMUAbstract
        {
         public:

            FMUWrapper_ME_1();
            ~FMUWrapper_ME_1();

            void load(const std::string& modelFile, const std::string& path, fmi_import_context_t* mpContext);
            void initialize(const std::shared_ptr<SimSettingsFMU> simSettings);
            //to run simulation
            bool checkForTriggeredEvent();
            bool itsEventTime();
            void handleEvents(const int intermediateResults);
            void prepareSimulationStep(const double time);
            void updateNextTimeStep(const double hdef);
            void setLastStepSize(const double simTimeEnd);
            void solveSystem();
            void doEulerStep();
            void setContinuousStates();
            void completedIntegratorStep(int* callEventUpdate);

            const FMUData* getFMUData();
            void fmi_get_real(unsigned int* valueRef, double* res);
            unsigned int fmi_get_variable_by_name(const char* name);

         private:
            fmi1_import_t* mpFMU;
            fmi1_callback_functions_t mCallBackFunctions;
            FMUData mFMUdata;
        };

    }  // namespace Model
}  // namespace OMVIS

#endif /* SOURCE_DIRECTORY__INCLUDE_MODEL_FMU_FMU_ME_1_HPP_ */
