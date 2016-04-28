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
#include "Util/Util.hpp"
#include "Control/OMVisManager.hpp"
#include "Visualize.hpp"

namespace OMVIS
{
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
        if (!Util::exists(resFileName))
            LOGGER_WRITE(std::string("Mat file ") + resFileName + std::string(" could not be found. Is it in the same directory as the model file?"), Util::LC_LOADER, Util::LL_ERROR);

        // read mat file
        omc_new_matlab4_reader(resFileName.c_str(), &_matReader);

        /*
         FILE * fileA = fopen("allVArs.txt", "w+");
         omc_matlab4_print_all_vars(fileA, &matReader);
         fclose(fileA);
         */
    }

    int OMVisualizerMAT::initData()
    {
        int isOk(0);
        isOk = OMVisualizerAbstract::initData();
        readMat(_baseData->_modelName, _baseData->_dirName);
        _omvManager->setStartTime(omc_matlab4_startTime(&_matReader));
        _omvManager->setEndTime(omc_matlab4_stopTime(&_matReader));
        return isOk;
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

    int OMVisualizerMAT::updateVisAttributes(const double time)
    {
        int isOk(0);
        // Update all shapes
        rapidxml::xml_node<>* rootNode = _baseData->_xmlDoc.first_node();
        unsigned int shapeIdx = 0;
		OMVIS::Util::rAndT rT;
        osg::ref_ptr<osg::Node> child = nullptr;
        try
        {
		for (std::vector<Model::ShapeObject>::size_type i = 0; i != _baseData->_shapes.size(); i++)
            {
			ShapeObject shape = _baseData->_shapes[i];

                // get the values for the scene graph objects
            Util::updateObjectAttributeMAT(&shape._length, time, _matReader);
			Util::updateObjectAttributeMAT(&shape._width, time, _matReader);
			Util::updateObjectAttributeMAT(&shape._height, time, _matReader);
											   
			Util::updateObjectAttributeMAT(&shape._lDir[0], time, _matReader);
			Util::updateObjectAttributeMAT(&shape._lDir[1], time, _matReader);
			Util::updateObjectAttributeMAT(&shape._lDir[2], time, _matReader);
											   
			Util::updateObjectAttributeMAT(&shape._wDir[0], time, _matReader);
			Util::updateObjectAttributeMAT(&shape._wDir[1], time, _matReader);
			Util::updateObjectAttributeMAT(&shape._wDir[2], time, _matReader);
											   
			Util::updateObjectAttributeMAT(&shape._r[0], time, _matReader);
			Util::updateObjectAttributeMAT(&shape._r[1], time, _matReader);
			Util::updateObjectAttributeMAT(&shape._r[2], time, _matReader);
											   
			Util::updateObjectAttributeMAT(&shape._rShape[0], time, _matReader);
			Util::updateObjectAttributeMAT(&shape._rShape[1], time, _matReader);
			Util::updateObjectAttributeMAT(&shape._rShape[2], time, _matReader);
											   
			Util::updateObjectAttributeMAT(&shape._T[0], time, _matReader);
			Util::updateObjectAttributeMAT(&shape._T[1], time, _matReader);
			Util::updateObjectAttributeMAT(&shape._T[2], time, _matReader);
			Util::updateObjectAttributeMAT(&shape._T[3], time, _matReader);
			Util::updateObjectAttributeMAT(&shape._T[4], time, _matReader);
			Util::updateObjectAttributeMAT(&shape._T[5], time, _matReader);
			Util::updateObjectAttributeMAT(&shape._T[6], time, _matReader);
			Util::updateObjectAttributeMAT(&shape._T[7], time, _matReader);
			Util::updateObjectAttributeMAT(&shape._T[8], time, _matReader);

			Util::updateObjectAttributeMAT(&shape._color[0], time, _matReader);
			Util::updateObjectAttributeMAT(&shape._color[1], time, _matReader);
			Util::updateObjectAttributeMAT(&shape._color[2], time, _matReader);

			Util::updateObjectAttributeMAT(&shape._specCoeff, time, _matReader);

			osg::Vec3f v = Util::normalize(osg::Vec3f(0.0,0.0,0.0));

			
			rT = Util::rotation(osg::Vec3f(shape._r[0].exp, shape._r[1].exp, shape._r[2].exp),
				osg::Vec3f(shape._rShape[0].exp, shape._rShape[1].exp, shape._rShape[2].exp),
				osg::Matrix3(shape._T[0].exp, shape._T[1].exp, shape._T[2].exp,
					shape._T[3].exp, shape._T[4].exp, shape._T[5].exp,
					shape._T[6].exp, shape._T[7].exp, shape._T[8].exp),
				osg::Vec3f(shape._lDir[0].exp, shape._lDir[1].exp, shape._lDir[2].exp),
				osg::Vec3f(shape._wDir[0].exp, shape._wDir[1].exp, shape._wDir[2].exp),
				shape._length.exp, shape._width.exp, shape._height.exp, shape._type);

			shape._mat = Util::assemblePokeMatrix(shape._mat, rT._T, rT._r);

            //update the shapes
            _nodeUpdater->_shape = shape;
			///shape.dumpVisAttributes();

                //get the scene graph nodes and stuff
                child = _viewerStuff->getScene().getRootNode()->getChild(shapeIdx);  // the transformation
                child->accept(*_nodeUpdater);
                ++shapeIdx;
            }
        }
        catch (std::exception& e)
        {
            LOGGER_WRITE(std::string("Something went wrong in OMVisualizer::updateVisAttributes at time point ") + std::to_string(time) + std::string(" ."), Util::LC_SOLVER, Util::LL_WARNING);
            isOk = 1;
        }
        return isOk;
    }

    void OMVisualizerMAT::initializeVisAttributes(const double time)
    {
        if (0.0 > time)
            LOGGER_WRITE(std::string("Cannot load visualization attributes for time point < 0.0."), Util::LC_LOADER, Util::LL_ERROR);
        updateVisAttributes(time);
    }

    void OMVisualizerMAT::updateScene(const double time)
    {
        if (0.0 > time)
            LOGGER_WRITE(std::string("Cannot load visualization attributes for time point < 0.0."), Util::LC_LOADER, Util::LL_ERROR);
        updateVisAttributes(time);
    }

    std::string OMVisualizerMAT::getType() const
    {
        return "mat";
    }

}  // End namepsace Model
}  // End namespace OMVIS
