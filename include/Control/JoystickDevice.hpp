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

#include <SDL_events.h>
#include <SDL_joystick.h>

// Forward declaration
namespace Model{
    class InputData;
}

namespace Control
{
    /*! \brief Class that serves as controller for input from joystick.
     *
     */
    class JoystickDevice
    {
     public:
        /*! \brief Default constructor.
         *
         * Initialices the joystick with the corresponding ID
         */
        JoystickDevice(int joyID);

        /// Destructor.
        ~JoystickDevice()
        {
        }

        /// The copy constructor is forbidden.
        //JoystickDevice(const JoystickDevice& jd) = delete;

        /// The assignment operator is forbidden.
        JoystickDevice& operator=(const JoystickDevice& jd) = delete;

        void detectContinuousInputEvents(Model::InputData inputInfo);

		int getXDir();

		int getYDir();


     private:
		/// \todo Should be a std::unique_ptr, but at least std::shared_ptr. Memory is allocated in this class!
        SDL_Joystick* _joystick;
        SDL_Event _inputEvent;
        int _xDir;
        int _yDir;
		int _joystickId;
    };

}  // End namespace Control

#endif /* INCLUDE_JOYSTICKDEVICE_HPP_ */
/**
 * @}
 */
