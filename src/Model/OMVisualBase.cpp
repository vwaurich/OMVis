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

#include "Model/OMVisualBase.hpp"
#include "Visualize.hpp"
#include "Util/Logger.hpp"

#include <osgDB/ReadFile>

namespace Model
{

    OMVisualBase::OMVisualBase(std::string model, std::string dir)
            : _modelName(model),
              _dirName(dir),
              _xmlDoc(),
              _visAttr(),
              _xmlFileName(dir + model + "_visual.xml")
    {
    }

    void OMVisualBase::initXMLDoc()
    {
        // check if xml file is available
        if (!exists(_xmlFileName))
            LOGGER_WRITE(std::string("There is no xml file named ") + _xmlFileName + ".", Util::LC_LOADER, Util::LL_ERROR);
        //std::cout << "There is no xml file named: " << _xmlFileName << std::endl;

        // read xml
        osgDB::ifstream t;
        // unused const char * titel = _xmlFileName.c_str();
        t.open(_xmlFileName.c_str(), std::ios::binary);      // open input file
        t.seekg(0, std::ios::end);    // go to the end
        int length = t.tellg();           // report location (this is the length)
        t.seekg(0, std::ios::beg);    // go back to the beginning
        char* buffer = new char[length];    // allocate memory for a buffer of appropriate dimension
        t.read(buffer, length);       // read the whole file into the buffer
        t.close();
        std::string buff = std::string(buffer);  // strings are good
        std::string buff2 = buff.substr(0, buff.find("</visualization>"));  // remove the crappy ending
        buff2.append("</visualization>");
        char* buff3 = strdup(buff2.c_str());  // cast to char*
        _xmlDoc.parse<0>(buff3);
        LOGGER_WRITE(std::string("Reading the xml file ") + _xmlFileName + " was successful.", Util::LC_LOADER, Util::LL_DEBUG);
    }

}  // End namespace Model
