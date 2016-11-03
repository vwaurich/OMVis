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

#include "Control/KeyboardEventHandler.hpp"
#include "Model/InputData.hpp"
#include "Util/Logger.hpp"

#include <string>

namespace OMVIS
{
    namespace Control
    {

        /*-----------------------------------------
         * CONSTRUCTORS
         *---------------------------------------*/

        KeyboardEventHandler::KeyboardEventHandler(std::shared_ptr<Model::InputData> inputs)
                : GUIEventHandler(),
                  _inputs(std::move(inputs))
        {
        }

        /*-----------------------------------------
         * FUNCTIONS
         *---------------------------------------*/

        bool KeyboardEventHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& /*aa*/)
        {
            switch (ea.getEventType())
            {
                case (osgGA::GUIEventAdapter::KEYDOWN):
                {
                    LOGGER_WRITE(std::string("KEYDOWN"), Util::LC_CTR, Util::LL_DEBUG);
                    unsigned int keyboardValue = ea.getKey();  // the ascii value corresponding to the pressed key

                    auto keyboardmapValue = _inputs->getKeyboardMap()->find(keyboardValue);

                    if (keyboardmapValue != _inputs->getKeyboardMap()->end())
                    {
                        //std::cout<<"pressed key "<<keyboardValue<<". Thats the input key: "<<keyboardmapValue->second<<std::endl;
                        //its a assigned key
                        auto iter = _inputs->getKeyMap()->find(static_cast<inputKey>(keyboardmapValue->second));
                        //this key is not assigned as input
                        if (iter == _inputs->getKeyMap()->end())
                        {
                            LOGGER_WRITE(std::string("Wrong key ") + std::to_string(keyboardValue), Util::LC_CTR,
                                         Util::LL_DEBUG);
                            //set the input variable
                        }
                        else
                        {
                            KeyMapValue iterValue = iter->second;
                            LOGGER_WRITE(
                                    std::string("Got the right key ") + std::to_string(keyboardValue)
                                            + " and base type " + std::to_string((int )iterValue._baseType),
                                    Util::LC_CTR, Util::LL_DEBUG);
                            int baseTypeIdx = static_cast<int>(iterValue._baseType);
                            switch (baseTypeIdx)
                            {
                                case (0):
                                    _inputs->getInputValues()->_valuesReal[iterValue._valueIdx] = 1.0;
                                    break;
                                case (1):
                                    _inputs->getInputValues()->_valuesInteger[iterValue._valueIdx] = 1;
                                    break;
                                case (2):
                                    _inputs->getInputValues()->_valuesBoolean[iterValue._valueIdx] = 1;
                                    break;
                                case (3):
                                    _inputs->getInputValues()->_valuesString[iterValue._valueIdx] = "";
                                    break;
                            }
                        }
                    }
                    break;
                }
                default:
                {
                    //inputs.resetDiscreteInputValues();
                    break;
                }
            }
            return false;
        }

    }  // End namespace Control
}  // End namespace OMVIS
