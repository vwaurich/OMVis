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

/** \addtogroup Model
 *  \{
 *  \copyright TU Dresden. All rights reserved.
 *  \authors Volker Waurich, Martin Flehmig
 *  \date Feb 2016
 */

#ifndef INCLUDE_FMUSIMULATE_HPP_
#define INCLUDE_FMUSIMULATE_HPP_

#include "Model/SimSettingsFMU.hpp"
#include "WrapperFMILib.hpp"

#include <string>
#include <memory>


/// \todo Can we find a better place for this structs and functions?
///
namespace OMVIS
{
    namespace Model
    {

        /*! \brief This struct encapsulates a FMU's data, e.g., pointer to states and state derivatives.
         *
         * \remark: We do not use smart pointers at this point because of performance.
         */
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
        /*! \brief This class represents a FMU that can be loaded into OMVIS for visualization.
         *
         * This class allocates the necessary memory for the FMU and its data.
         *
         * \remark Currently we do not provide interfaces to access any function described in the FMI 1.0 standard.
         */
        class FMUWrapper
        {
         public:
            /*-----------------------------------------
             * CONSTRUCTORS
             *---------------------------------------*/

            /*! \brief Default constructor.
             *
             * This constructor initializes the members with save default values.
             */
            FMUWrapper();

            /*! \brief The destructor frees memory allocated in FMUData. */
            ~FMUWrapper();

            FMUWrapper(const FMUWrapper&) = delete;
            FMUWrapper& operator=(const FMUWrapper&) = delete;

            /*-----------------------------------------
             * INITIALIZATION METHODS
             *---------------------------------------*/

            /*! \brief Loads the FMU given by name and path into memory. */
            void load(const std::string& modelFile, const std::string& path);

            /*! \brief Initializes the FMU with the given simulation settings. */
            void initialize(const std::shared_ptr<Model::SimSettingsFMU> simSettings);

            /*-----------------------------------------
             * GETTERS and SETTERS
             *---------------------------------------*/

            /*! \brief Returns constant pointer to FMUData in order to allow (read) access to it. */
            const FMUData* getFMUData() const;

            /*! \brief Returns constant pointer to FMU in order to allow (read) access to it. */
            fmi1_import_t* getFMU() const;

            /*! \brief Returns the current simulation time. */
            double getTcur() const;

            /*! \brief Wraps fmi1_import_set_continuous_states. */
            void setContinuousStates();

            /*-----------------------------------------
             * SIMULATION METHODS
             *---------------------------------------*/

            /*! \brief Checks if an event indicator has triggered.
             *
             * \return True, if an event indicator has triggered. Otherwise, return false.
             */
            bool checkForTriggeredEvent() const;

            /*! \brief Checks if an event is registered for the current time.
             *
             * \return True, if an event is registered for the current time. Otherwise, return false.
             */
            bool itsEventTime() const;

            void updateNextTimeStep(const fmi1_real_t hdef);

            /*! \brief Wraps fmi1_import_get_derivatives and updates _fmiStatus. */
            void fmi1ImportGetDerivatives();

            /*! \brief Handles events. */
            void handleEvents(const fmi1_boolean_t intermediateResults);

            /*! \brief Prepares FMU for new simulation step at given time.
             *
             * Wraps fmi1_import_set_time and fmi1_import_get_event_indicators. _fmiStatus is updated.
             */
            void prepareSimulationStep(const double time);

            /*! \brief Updates times _hcur and _tcur.
             *
             * \todo Remove hard coded value? Let the user specify the value.
             */
            void updateTimes(const double simTimeEnd);

            /*! \brief Solves the ODE system.
             *
             * Wraps fmi1_import_get_derivatives which provides the derivatives. Updates _fmiStatus.
             */
            void solveSystem();

            /*! \brief Performs a step of the Forward Euler algorithm to determine the state values. */
            void doEulerStep();

            /*! \brief Wraps fmi1_import_completed_integrator_step. */
            void completedIntegratorStep(fmi1_boolean_t* callEventUpdate);

         private:
            /*-----------------------------------------
             * MEMBERS
             *---------------------------------------*/

            std::shared_ptr<fmi1_import_t> _fmu;
            std::shared_ptr<fmi_import_context_t> _context;
            jm_callbacks _callbacks;
            fmi1_callback_functions_t _callBackFunctions;

            /*! The encapsulated FMU data. */
            FMUData _fmuData;
        };

        /*-----------------------------------------
         * FREE METHODS
         *---------------------------------------*/

        /// \todo Todo Do we need this method?
        fmi1_base_type_enu_t getFMI1baseTypeFor4CharString(const std::string& typeString);

    }  // namespace Model
}  // namespace OMVIS

#endif /* INCLUDE_FMUSIMULATE_HPP_ */
/**
 * \}
 */
