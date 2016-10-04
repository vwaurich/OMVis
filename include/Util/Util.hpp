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

#include "Util/Logger.hpp"

#include <rapidxml.hpp>
#include <boost/filesystem.hpp>
#include <osgDB/ReadFile>

#include <string>
#include <sys/stat.h>

#ifndef _WIN32
#include <arpa/inet.h>
#endif

namespace OMVIS
{
    namespace Util
    {

        /*! \brief Checks if the given type is a cad file
         */
        inline bool isCADType(const std::string& type)
        {
            return (type.size() >= 12 && std::string(type.begin(), type.begin() + 11) == "modelica://");
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

//        inline bool exists(const std::string& name)
//        {
//            osgDB::ifstream f(name.c_str());
//            if (f.good())
//            {
//                f.close();
//                return true;
//            }
//            else
//            {
//                f.close();
//                return false;
//            }
//        }

        /*! \brief Checks if the file is accessible. */
        inline bool fileExists(const std::string& file)
        {
            struct stat buffer;
            return (stat(file.c_str(), &buffer) == 0);
        }

        /*! \brief Converts a bool into a std::string, i.e., returning "true" or "false".
         *
         * By using this quite simple function we do not need to include any fancy third party method
         * or library, like boost cast to get the job done.
         *
         * \param b Bool to convert.
         * \return "true" or "false".
         */
        inline const char* boolToString(bool b)
        {
            return b ? "true" : "false";
        }

        /*! \brief Returns absolute path for the given path.
         *
         * \remark: It seems to be no other platform independent way than boost::filesystem.
         *          With the use of some ifdefs to encapsulate the platform dependent functions,
         *          we could resolve the dependency from boost::filesystem. Or we wait for C++17 :-)
         */
        inline std::string makeAbsolutePath(const std::string& path)
        {
            boost::filesystem::path bPath = boost::filesystem::path(path.c_str());
            bPath = boost::filesystem::absolute(bPath);
            return bPath.string();
        }

        /*! \brief Functions that tests the given string to be a valid IPv4 address.
         *
         * @param ipAddress The address to be tested.
         * @return Boolean.
         */
        inline bool isValidIPv4(const std::string& ipAddress)
        {
#ifndef _WIN32
            struct sockaddr_in sa;
            int result = inet_pton(AF_INET, ipAddress.c_str(), &(sa.sin_addr));

            if (result != 0)
            {
                LOGGER_WRITE(ipAddress + std::string(" is a valid IPv4 address."), Util::LC_GUI, Util::LL_WARNING);
                return true;
            }
            else
            {
                LOGGER_WRITE(ipAddress + std::string(" is not a valid IPv4 address."), Util::LC_GUI, Util::LL_WARNING);
                return false;
            }
#elif _WIN32
            LOGGER_WRITE("isValidIpAddressV4 has not been implemented for Windows", Util::LC_GUI, Util::LL_WARNING);
            return false;
#endif

            // Short way without logger usage.
            //return result != 0;
        }

        /*! \brief Functions that tests the given string to be a valid IPv6 address.
         *
         * @param ipAddress The address to be tested.
         * @return Boolean.
         */
        inline bool isValidIPv6(const std::string& ipAddress)
        {
#ifndef _WIN32

            struct sockaddr_in6 sa;
            int result = inet_pton(AF_INET6, ipAddress.c_str(), &(sa.sin6_addr));

            if (result != 0)
            {
                LOGGER_WRITE(ipAddress + std::string(" is a valid IPv6 address."), Util::LC_GUI, Util::LL_WARNING);
                return true;
            }
            else
            {
                LOGGER_WRITE(ipAddress + std::string(" is not a valid IPv6 address."), Util::LC_GUI, Util::LL_WARNING);
                return false;
            }
            LOGGER_WRITE(ipAddress + std::string(" is not a valid IPv6 address."), Util::LC_GUI, Util::LL_WARNING);
            // Short way without logger usage.
            //return result != 0;
#elif _WIN32
            LOGGER_WRITE("isValidIpAddressV6 has not been implemented for Windows", Util::LC_GUI, Util::LL_WARNING);
            return false;
#endif
        }

        /// \todo Implement me!
        inline bool isValidServerName(const std::string& serverName)
        {
            LOGGER_WRITE(serverName + std::string(" is assumed to be a valid Server Name. TODO: Implement a proper check!"), Util::LC_GUI, Util::LL_WARNING);
            return true;
        }

        /*! Extracts the path from a given file and returns it.
         *
         * Example: /home/user/models/modelX.fmu --> /home/user/models/
         *
         * @param[in] fileIn A full specified file, i.e., including path.
         */
        inline std::string getPath(const std::string& fileIn)
        {
            std::size_t pos = fileIn.find_last_of("/\\");
            std::string dirPath = fileIn.substr(0, pos + 1);
            return dirPath;
        }

        /*! Extracts the name from a given file and returns it.
         *
         * Example: /home/user/models/modelX.fmu --> modelX.fmu
         *
         * @param[in] fileIn A full specified file, i.e., including path.
         */
        inline std::string getFileName(const std::string& fileIn)
        {
            std::size_t pos = fileIn.find_last_of("/\\");
            std::string modelFileName = fileIn.substr(pos + 1, fileIn.length());
            return modelFileName;
        }

        inline bool isFMU(const std::string& fileIn)
        {
            std::size_t fmu = fileIn.find(".fmu");
            return (fmu != std::string::npos);
        }

        inline bool isMAT(const std::string& fileIn)
        {
            std::size_t mat = fileIn.find(".mat");
            return (mat != std::string::npos);
        }

        /*! \brief Creates the name of the xml visual file from model name and path.
         *
         * @param[in] modelFile Name of the file containing the model, e.g., modelFoo.fmu
         * @param[in] path Path where the model file is stored, e.g., /home/usr/models/
         * @return XML file name, e.g., /home/usr/models/modelFoo_visual.xml
         */
        inline std::string getXMLFileName(const std::string& modelFile, const std::string& path)
        {
            int signsOff(0);
            // modelFoo.fmu --> -4 sings
            if (isFMU(modelFile))
                signsOff = 4;
            // modelFoo_res.mat --> -8 signs
            else if (isMAT(modelFile))
                signsOff = 8;
            // Mhh, what kind of file? Or new pattern?
            else
            {
                // todo: Handle this case.
            }
            // Cut off prefix [fmu|mat]
            std::string fileName = modelFile.substr(0, modelFile.length() - signsOff);
            // Construct XML file name
            std::string xmlFileName = path + fileName + "_visual.xml";
            return xmlFileName;
        }

        /*! Checks if the visual XML file for the given model and path is present.
         *
         * @param[in] modelFile Name of the file containing the model, e.g., modelFoo.fmu
         * @param[in] path Path where the model file is stored, e.g., /home/usr/models/
         * @return True, if the visual XML file exists in the path.
         */
        inline bool checkForXMLFile(const std::string& modelFile, const std::string& path)
        {
            // Cut off prefix [fmu|mat]
            std::string xmlFileName = getXMLFileName(modelFile, path);
            return Util::fileExists(xmlFileName);
        }

    }  // End namespace Util
}  // End namespace OMVIS

#endif /* INCLUDE_UTIL_HPP_ */
/**
 * @}
 */
