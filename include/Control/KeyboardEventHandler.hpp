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

#ifndef INCLUDE_CONTROL_KEYBOARDEVENTHANDLER_HPP_
#define INCLUDE_CONTROL_KEYBOARDEVENTHANDLER_HPP_

#include <osgGA/GUIEventHandler>

#include <memory>

namespace OMVIS
{
    namespace Model
    {
        class InputData;
    }
}

namespace OMVIS
{
    namespace Control
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
//            KeyboardEventHandler(Model::InputData* inputs);
            KeyboardEventHandler(std::shared_ptr<Model::InputData> inputs);

            /// Let the compiler provide the destructor.
            ~KeyboardEventHandler() = default;

            /// The copy constructor is forbidden.
            KeyboardEventHandler(const KeyboardEventHandler& keh) = delete;

            /// The assignment operator is forbidden.
            KeyboardEventHandler& operator=(const KeyboardEventHandler& keh) = delete;

            /*! \todo According to /.../include/osgGA/GUIEventHandler, this method is deprecated. We should move to
             *     1. virtual bool handle(osgGA::Event* event, osg::Object* object, osg::NodeVisitor* nv);
             *  or 2. virtual bool handle(const GUIEventAdapter& ea,GUIActionAdapter& aa, osg::Object*, osg::NodeVisitor*);
             */
            virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

         private:
            /// \todo Should be a std::shared_ptr (points to memory managed by other class, i.e. _guiController->getInputData())
            std::shared_ptr<Model::InputData> _inputs;
        };

    }  // End namespace Control
}  // End namespace OMVIS

#endif /* INCLUDE_CONTROL_KEYBOARDEVENTHANDLER_HPP_ */
/**
 * @}
 */
