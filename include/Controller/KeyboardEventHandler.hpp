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
 * KeyboardEventHandler.hpp
 *
 *  Created on: 03.02.2016
 *      Author: mf
 */

#ifndef INCLUDE_KEYBOARDEVENTHANDLER_HPP_
#define INCLUDE_KEYBOARDEVENTHANDLER_HPP_

#include <osgGA/GUIEventHandler>
#include "Model/InputData.hpp"

namespace Controller
{
    /*! \brief This class handles keyboard events.
     *
     * Inherits from osg GUIEventHandler.
     */
    class KeyboardEventHandler : public osgGA::GUIEventHandler
    {
     public:
        /// Default constructor is forbidden.
        KeyboardEventHandler() = delete;

        /*! \brief Constructs KeyboardEventHandler from InputData argument.
         *
         * @param inputs
         */
        KeyboardEventHandler(Model::InputData* inputs);

        /// Let the compiler provide the destructor.
        ~KeyboardEventHandler() = default;

        /// The copy constructor is forbidden.
        KeyboardEventHandler(const KeyboardEventHandler& keh) = delete;

        /// The assignment operator is forbidden.
        KeyboardEventHandler& operator=(const KeyboardEventHandler& keh) = delete;

        virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

     private:
        Model::InputData* _inputs;
    };

}  // End namespace controller

#endif /* INCLUDE_KEYBOARDEVENTHANDLER_HPP_ */
