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

/** @addtogroup Control
 *  @{
 *  \copyright TU Dresden. All rights reserved.
 *  \authors Volker Waurich, Martin Flehmig
 *  \date Feb 2016
 */

#ifndef INCLUDE_JOYSTICKDEVICE_HPP_
#define INCLUDE_JOYSTICKDEVICE_HPP_

#include "Model/InputData.hpp"

#include <SDL_events.h>
#include <SDL_joystick.h>

#include <memory>


namespace OMVIS
{
    namespace Control
    {

        /*! \brief Class that serves as controller for input from joystick.
         *
         */
        class JoystickDevice
        {
         public:
            /*-----------------------------------------
             * CONSTRUCTORS
             *---------------------------------------*/

            /*! \brief Constructor.
             *
             * Initializes the joystick with the corresponding ID.
             */
            JoystickDevice(const int joyID);

            /// Destructor.
            ~JoystickDevice();

            /// The copy constructor is forbidden.
            JoystickDevice(const JoystickDevice& jd) = delete;

            /// The assignment operator is forbidden.
            JoystickDevice& operator=(const JoystickDevice& jd) = delete;

            /*-----------------------------------------
             * SIMULATION METHODS
             *---------------------------------------*/

            void detectContinuousInputEvents(std::shared_ptr<Model::InputData>& inputInfo);

            /*-----------------------------------------
             * GETTERS AND SETTERS
             *---------------------------------------*/

            int getXDir() const;
            int getYDir() const;

         private:
            /// Memory is allocated and freed within SDL library. Just call the appropriate methods.
            SDL_Joystick* _joystick;
            SDL_Event _inputEvent;
            int _xDir;
            int _yDir;
            int _joystickId;
        };

    }  // End namespace Control
}  // End namespace OMVIS

#endif /* INCLUDE_JOYSTICKDEVICE_HPP_ */
/**
 * @}
 */
