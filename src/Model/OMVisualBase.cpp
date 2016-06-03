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
#include "Util/Util.hpp"

#include <osgDB/ReadFile>

namespace OMVIS
{
    namespace Model
    {

        OMVisualBase::OMVisualBase(const std::string& model, const std::string& dir)
                : _modelName(model),
                  _dirName(dir),
                  _xmlDoc(),
                  _shapes(),
                  _xmlFileName(dir + model + "_visual.xml")
        {
        }

        int OMVisualBase::initXMLDoc()
        {
            int isOk(0);
            // check if xml file is available
            if (!Util::fileExists(_xmlFileName))
            {
                LOGGER_WRITE(std::string("There is no xml file named ") + _xmlFileName + ".", Util::LC_LOADER, Util::LL_ERROR);
                isOk = 1;
            }
            else
            {
                // read xml
                osgDB::ifstream t;
                // unused const char * titel = _xmlFileName.c_str();
                t.open(_xmlFileName.c_str(), std::ios::binary);      // open input file
                t.seekg(0, std::ios::end);    // go to the end
                int length = t.tellg();       // report location (this is the length)
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
            return isOk;
        }

        int OMVisualBase::initVisObjects()
        {
            rapidxml::xml_node<>* rootNode = _xmlDoc.first_node();
            for (rapidxml::xml_node<>* shapeNode = rootNode->first_node("shape"); shapeNode; shapeNode = shapeNode->next_sibling())
            {
                Model::ShapeObject shape;
                rapidxml::xml_node<>* expNode = shapeNode->first_node((const char*) "ident")->first_node();
                shape._id = std::string(expNode->value());

                expNode = shapeNode->first_node((const char*) "type")->first_node();
                if (expNode == 0)
                {
                    LOGGER_WRITE(std::string("The type of  ") + shape._id + " is not supported right in the visxml file.", Util::LC_LOADER, Util::LL_DEBUG);
                    break;
                }

                shape._type = std::string(expNode->value());

                expNode = shapeNode->first_node((const char*) "length")->first_node();
                shape._length = Util::getObjectAttributeForNode(expNode);
                expNode = shapeNode->first_node((const char*) "width")->first_node();
                shape._width = Util::getObjectAttributeForNode(expNode);
                expNode = shapeNode->first_node((const char*) "height")->first_node();
                shape._height = Util::getObjectAttributeForNode(expNode);

                expNode = shapeNode->first_node((const char*) "lengthDir")->first_node();
                shape._lDir[0] = Util::getObjectAttributeForNode(expNode);
                expNode = expNode->next_sibling();
                shape._lDir[1] = Util::getObjectAttributeForNode(expNode);
                expNode = expNode->next_sibling();
                shape._lDir[2] = Util::getObjectAttributeForNode(expNode);

                expNode = shapeNode->first_node((const char*) "widthDir")->first_node();
                shape._wDir[0] = Util::getObjectAttributeForNode(expNode);
                expNode = expNode->next_sibling();
                shape._wDir[1] = Util::getObjectAttributeForNode(expNode);
                expNode = expNode->next_sibling();
                shape._wDir[2] = Util::getObjectAttributeForNode(expNode);

                expNode = shapeNode->first_node((const char*) "r")->first_node();
                shape._r[0] = Util::getObjectAttributeForNode(expNode);
                expNode = expNode->next_sibling();
                shape._r[1] = Util::getObjectAttributeForNode(expNode);
                expNode = expNode->next_sibling();
                shape._r[2] = Util::getObjectAttributeForNode(expNode);

                expNode = shapeNode->first_node((const char*) "r_shape")->first_node();
                shape._rShape[0] = Util::getObjectAttributeForNode(expNode);
                expNode = expNode->next_sibling();
                shape._rShape[1] = Util::getObjectAttributeForNode(expNode);
                expNode = expNode->next_sibling();
                shape._rShape[2] = Util::getObjectAttributeForNode(expNode);

                expNode = shapeNode->first_node((const char*) "color")->first_node();
                shape._color[0] = Util::getObjectAttributeForNode(expNode);
                expNode = expNode->next_sibling();
                shape._color[1] = Util::getObjectAttributeForNode(expNode);
                expNode = expNode->next_sibling();
                shape._color[2] = Util::getObjectAttributeForNode(expNode);

                expNode = shapeNode->first_node((const char*) "T")->first_node();
                shape._T[0] = Util::getObjectAttributeForNode(expNode);
                expNode = expNode->next_sibling();
                shape._T[1] = Util::getObjectAttributeForNode(expNode);
                expNode = expNode->next_sibling();
                shape._T[2] = Util::getObjectAttributeForNode(expNode);
                expNode = expNode->next_sibling();
                shape._T[3] = Util::getObjectAttributeForNode(expNode);
                expNode = expNode->next_sibling();
                shape._T[4] = Util::getObjectAttributeForNode(expNode);
                expNode = expNode->next_sibling();
                shape._T[5] = Util::getObjectAttributeForNode(expNode);
                expNode = expNode->next_sibling();
                shape._T[6] = Util::getObjectAttributeForNode(expNode);
                expNode = expNode->next_sibling();
                shape._T[7] = Util::getObjectAttributeForNode(expNode);
                expNode = expNode->next_sibling();
                shape._T[8] = Util::getObjectAttributeForNode(expNode);

                expNode = shapeNode->first_node((const char*) "specCoeff")->first_node();
                shape._specCoeff = Util::getObjectAttributeForNode(expNode);

                _shapes.push_back(shape);
            }
            return 0;
        }

        void OMVisualBase::clearXMLDoc()
        {
            _xmlDoc.clear();
        }

        rapidxml::xml_node<>* OMVisualBase::getFirstXMLNode()
        {
            return _xmlDoc.first_node();
        }

        const std::string OMVisualBase::getModelName() const
        {
            return _modelName;
        }

        const std::string OMVisualBase::getDirName() const
        {
            return _dirName;
        }

        const std::string OMVisualBase::getXMLFileName() const
        {
            return _xmlFileName;
        }

    }  // End namespace Model
}  // End namespace OMVIS
