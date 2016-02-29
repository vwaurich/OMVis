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

/*
 * JoystickDevice.hpp
 *
 *  Created on: 10.02.2016
 *      Author: mf
 */

#ifndef INCLUDE_JOYSTICKDEVICE_HPP_
#define INCLUDE_JOYSTICKDEVICE_HPP_

#include <SDL_events.h>
#include <SDL_joystick.h>

// Forward declaration
namespace Model{
    class InputData;
}

namespace Controller
{
    /*! \brief Class that serves as controller for input from joystick.
     *
     */
    class JoystickDevice
    {
     public:
        /*! \brief Default constructor.
         *
         * Takes no arguments. Attributes are set to default values.
         */
        JoystickDevice();

        /// Destructor.
        ~JoystickDevice()
        {
        }

        /// The copy constructor is forbidden.
        JoystickDevice(const JoystickDevice& jd) = delete;

        /// The assignment operator is forbidden.
        JoystickDevice& operator=(const JoystickDevice& jd) = delete;

        void detectContinuousInputEvents(Model::InputData inputInfo);

     private:
        SDL_Joystick* _joystick;
        SDL_Event _inputEvent;
        int _xDir;
        int _yDir;
    };

}  // End namespace Controller

#endif /* INCLUDE_JOYSTICKDEVICE_HPP_ */
