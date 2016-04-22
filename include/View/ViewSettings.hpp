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

#ifndef INCLUDE_VIEW_VIEWSETTINGS_HPP_
#define INCLUDE_VIEW_VIEWSETTINGS_HPP_

#include <vector>

namespace OMVIS
{

    namespace View
    {

        /**
         * This enum holds the available background colors for the scene window.
         */
        enum BackgroundColor
        {
            blue,  //!< blue
            lila,  //!< lila
            white,  //!< white
            green,  //!< green
            black  //!< black
        };

        /**
         * The predefined perspectives of the scene view are normal to the x-y, x-z or y-z plane.
         */
        enum Perspective
        {
            home,  //!< original perspective while loading the scene
            xy,  //!< normal to x-y plane
            xz,  //!< normal to x-z plane
            yz,  //!< normal to y-z plane
        };

        // MF: May  be we need this later?
        /*! \brief This struct holds the view settings which can be specified for OMVis.
         */
        struct ViewSettings
        {
            /**
             * This attribute represents the chosen background color for the scene window.
             */
            BackgroundColor _bgc;

            /**
             * \todo Implement me in a proper way. The aim is to provide "continuous" zoom handling.
             */
            float _zoom;

            /**
             * This attribute represents the perspective.
             */
            Perspective _perspective;
        };

    }  // End namespace View
}  // End namespace OMVIS

#endif /* INCLUDE_VIEW_VIEWSETTINGS_HPP_ */
