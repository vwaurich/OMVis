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

/** @addtogroup Util
 *  @{
 *  \copyright TU Dresden. All rights reserved.
 *  \authors Volker Waurich, Martin Flehmig
 *  \date Feb 2016
 */

#ifndef INCLUDE_UTIL_HPP_
#define INCLUDE_UTIL_HPP_

#include <rapidxml.hpp>
#include <boost/filesystem.hpp>
#include <osgDB/ReadFile>

#include <string>

namespace OMVIS
{
    namespace Util
    {

        /*! \brief Checks if the type is a cad file
         */
        inline bool isCADType(const std::string& typeName)
        {
            return (typeName.size() >= 12 && std::string(typeName.begin(), typeName.begin() + 11) == "modelica://");
        }

        /*! \brief Get file name of the cad file
         */
        inline std::string extractCADFilename(const std::string& s)
        {
            std::string fileKey = "modelica://";
            std::string s2 = s.substr(fileKey.length(), s.length());
            int pos = s2.find("/");
            return s2.substr(pos + 1, s.length());
        }

        /*! \brief Gets the type of the shape.
         */
        inline std::string getShapeType(rapidxml::xml_node<>* node)
        {
            return node->first_node("type")->value();
        }

        inline bool exists(const std::string& name)
        {
            osgDB::ifstream f(name.c_str());
            if (f.good())
            {
                f.close();
                return true;
            }
            else
            {
                f.close();
                return false;
            }
        }

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

        /*! \brief Returns absolute path to the given path.
         *
         * \remark: It seems to be no other platform independent way than boost::filesystem.
         *          With the use of some ifdefs to encapsulate the platform dependent functions,
         *          we could resolve the dependency from boost::filesystem.
         */
        inline std::string makeAbsolutePath(const std::string& path)
        {
            boost::filesystem::path bPath = boost::filesystem::path(path.c_str());
            bPath = boost::filesystem::absolute(bPath);
            return bPath.string();
        }

    }  // End namespace Util
}  // End namespace OMVIS

#endif /* INCLUDE_UTIL_HPP_ */
/**
 * @}
 */
