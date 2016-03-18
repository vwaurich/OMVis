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

#include "Model/OMVisualizerMAT.hpp"
#include "Util/Logger.hpp"
#include "Control/OMVisManager.hpp"

namespace Model
{

    OMVisualizerMAT::OMVisualizerMAT(const std::string model, const std::string path)
            : OMVisualizerAbstract(model, path),
              _matReader()
    {
    }

    void OMVisualizerMAT::readMat(const std::string modelString, const std::string dirString)
    {
        std::string resFileName = dirString + modelString + "_res.mat";
        //checks
        if (!exists(resFileName))
            LOGGER_WRITE(std::string("Mat file ") + resFileName + std::string(" could not be found. Is it in the same directory as the model file?"), Util::LC_INIT, Util::LL_ERROR);

        // read mat file
        omc_new_matlab4_reader(resFileName.c_str(), &_matReader);

        /*
         FILE * fileA = fopen("allVArs.txt", "w+");
         omc_matlab4_print_all_vars(fileA, &matReader);
         fclose(fileA);
         */
    }

    void OMVisualizerMAT::initData()
    {
        OMVisualizerAbstract::initData();
        readMat(_baseData->_modelName, _baseData->_dirName);
        _omvManager->_startTime = omc_matlab4_startTime(&_matReader);
        _omvManager->_endTime = omc_matlab4_stopTime(&_matReader);
    }

    double* omc_get_varValue1(ModelicaMatReader* reader, const char* varName, double time)
    {
        double val = 0.0;
        double* res = &val;
        ModelicaMatVariable_t* var = nullptr;
        var = omc_matlab4_find_var(reader, varName);
        if (var == nullptr)
            LOGGER_WRITE(std::string("Could not get variable ") + varName + std::string(" from result file."), Util::LC_CTR, Util::LL_ERROR);
        else
            omc_matlab4_val(res, reader, var, time);

        return res;
    }

    void OMVisualizerMAT::updateVisAttributes(const double time)
    {
        // Update all shapes
        rapidxml::xml_node<>* rootNode = _baseData->_xmlDoc.first_node();
        unsigned int shapeIdx = 0;
        rAndT rT;
        osg::ref_ptr<osg::Node> child = nullptr;
        for (rapidxml::xml_node<>* shapeNode = rootNode->first_node("shape"); shapeNode; shapeNode = shapeNode->next_sibling())
        {
            // get the values for the scene graph objects
            _baseData->_visAttr._type = getShapeType(shapeNode);
            _baseData->_visAttr._length = getShapeAttrMAT((const char*) "length", shapeNode, time, _matReader);
            _baseData->_visAttr._width = getShapeAttrMAT((const char*) "width", shapeNode, time, _matReader);
            _baseData->_visAttr._height = getShapeAttrMAT((const char*) "height", shapeNode, time, _matReader);
            _baseData->_visAttr._r = getShapeVectorMAT((char*) "r", shapeNode, time, _matReader);
            _baseData->_visAttr._rShape = getShapeVectorMAT((char*) "r_shape", shapeNode, time, _matReader);
            _baseData->_visAttr._lDir = getShapeVectorMAT((char*) "lengthDir", shapeNode, time, _matReader);
            _baseData->_visAttr._wDir = getShapeVectorMAT((char*) "widthDir", shapeNode, time, _matReader);
            _baseData->_visAttr._color = getShapeVectorMAT((char*) "color", shapeNode, time, _matReader);
            _baseData->_visAttr._T = getShapeMatrixMAT((char*) "T", shapeNode, time, _matReader);
            rT = staticRotation(_baseData->_visAttr._r, _baseData->_visAttr._rShape, _baseData->_visAttr._T, _baseData->_visAttr._lDir, _baseData->_visAttr._wDir, _baseData->_visAttr._length, _baseData->_visAttr._width, _baseData->_visAttr._height, _baseData->_visAttr._type);
            _baseData->_visAttr._r = rT._r;
            _baseData->_visAttr._T = rT._T;
            _baseData->_visAttr._mat = assemblePokeMatrix(_baseData->_visAttr._mat, _baseData->_visAttr._T, _baseData->_visAttr._r);

            //update the shapes
            _nodeUpdater->_visAttr = _baseData->_visAttr;
            //_baseData->_visAttr.dumpVisAttributes();

            //get the scene graph nodes and stuff
            child = _viewerStuff->_scene._rootNode->getChild(shapeIdx);  // the transformation
            child->accept(*_nodeUpdater);
            ++shapeIdx;
        }
    }

    void OMVisualizerMAT::initializeVisAttributes(const double time)
    {
        updateVisAttributes(time);
    }

    void OMVisualizerMAT::updateScene(const double time)
    {
        updateVisAttributes(time);
    }

    int OMVisualizerMAT::getDataTypeID()
    {
        return 0;
    }

}  // End namepsace Model

