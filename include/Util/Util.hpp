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
 * Util.hpp
 *
 *  Created on: 15.02.2016
 *      Author: mf
 */

#ifndef INCLUDE_UTIL_UTIL_HPP_
#define INCLUDE_UTIL_UTIL_HPP_

namespace Util
{

    /*! \brief Converts a bool into a std::string, i.e., returning "true" or "false".
     *
     * By using this quite simple function we do not need to include any fancy third party method
     * or library, like boost cast.
     * @param b Bool to convert.
     * @return "true" or "false".
     */
    inline const char* const boolToString(bool b)
    {
        return b ? "true" : "false";
    }

}  // End namespace Util

#endif /* INCLUDE_UTIL_UTIL_HPP_ */
