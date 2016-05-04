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

#include "Control/JoystickDevice.hpp"
#include "Model/InputData.hpp"
#include "Util/Logger.hpp"

#include <SDL.h>

#include <string>

namespace OMVIS
{
    namespace Control
    {
        JoystickDevice::JoystickDevice(const int joyID)
                : _joystick(nullptr),
                  _xDir(0),
                  _yDir(0),
                  _joystickId(joyID)
        {
            //Initialize SDL
            //if (SDL_Init(SDL_INIT_JOYSTICK) < 0)
            //    LOGGER_WRITE(std::string("SDL could not be initialized."),  Util::LC_LOADER, Util::LL_ERROR);

            //Check for joysticks
            if (SDL_NumJoysticks() < 1)
                LOGGER_WRITE(std::string("No joysticks connected!"), Util::LC_LOADER, Util::LL_WARNING);
            else
            {
                LOGGER_WRITE(std::string("Found ") + std::to_string(SDL_NumJoysticks()) + std::string(" joystick(s)"), Util::LC_LOADER, Util::LL_INFO);
                //Load joystick
                _joystick = SDL_JoystickOpen(joyID);
                if (_joystick == nullptr)
                    LOGGER_WRITE(std::string("Unable to open joystick! SDL Error: ") + SDL_GetError(), Util::LC_LOADER, Util::LL_INFO);
            }
        }

        int JoystickDevice::getXDir()
        {
            return _xDir;
        }

        int JoystickDevice::getYDir()
        {
            return _yDir;
        }

        void JoystickDevice::detectContinuousInputEvents(std::shared_ptr<Model::InputData> inputInfo)
        {

            SDL_PollEvent (&_inputEvent);

            if (_inputEvent.type == SDL_JOYAXISMOTION)
            {
                //Motion on first joystick
                if (_inputEvent.jaxis.which == _joystickId)
                {
                    //X axis motion
                    if (_inputEvent.jaxis.axis == 0)
                    {
                        _xDir = _inputEvent.jaxis.value;
                        Model::setRealInputValueForInputKey(inputKey(_joystickId), _xDir, *inputInfo);
                    }
                    //Y axis motion
                    else if (_inputEvent.jaxis.axis == 1)
                    {
                        _yDir = _inputEvent.jaxis.value;
                        Model::setRealInputValueForInputKey(inputKey(_joystickId + 1), _yDir, *inputInfo);
                    }
                    else
                    {
                        _xDir = 0;
                        _yDir = 0;
                    }
                }
            }
        }

    }  // End namespace Control
}  // End namespace OMVIS
