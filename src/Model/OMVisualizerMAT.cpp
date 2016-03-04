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
 * OMVisualizerMAT.cpp
 *
 *  Created on: 19.02.2016
 *      Author: mf
 */

#include "Model/OMVisualizerMAT.hpp"

namespace Model
{

    OMVisualizerMAT::OMVisualizerMAT(std::string model, std::string path)
            : OMVisualizerAbstract(model, path), 
		      _matReader()
    {
    }

    void OMVisualizerMAT::readMat(std::string modelString, std::string dirString)
    {
        std::string resFileName = dirString + modelString + "_res.mat";
        //checks
        if (!exists(resFileName))
        {
            std::cout << "There is no mat file named: " << resFileName << std::endl;
        }
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
		omvManager->_startTime = omc_matlab4_startTime(&_matReader);
		omvManager->_endTime = omc_matlab4_stopTime(&_matReader);
    }


	double* omc_get_varValue1(ModelicaMatReader* reader, const char* varName, double time)
	{
		double val = 0.0;
		double* res = &val;
		ModelicaMatVariable_t* var = NULL;
		var = omc_matlab4_find_var(reader, varName);
		if (var == NULL)
			std::cout << "Error: Did not get variable from result file! " << varName << std::endl;
		else
			omc_matlab4_val(res, reader, var, time);

		return res;
	}


	void OMVisualizerMAT::updateVisAttributes(double time)
	{
		Initialization::VisAttributesFactory* factory = new Initialization::VisAttributesFactory;
		
		// Update all shapes
		rapidxml::xml_node<>* rootNode = _baseData->_xmlDoc.first_node();
		unsigned int shapeIdx = 0;

		for (rapidxml::xml_node<>* shapeNode = rootNode->first_node("shape"); shapeNode; shapeNode = shapeNode->next_sibling())
		{
			//create a VisAttributes object according to the node
			VisAttributes* visAttr = _baseData->allVisAttr[shapeIdx];

			visAttr->updateVisAttributesMAT(shapeNode, time, _matReader);

			//update the visualization objects with a nodevisitor
			nodeUpdater->_visAttr = visAttr;
			//nodeUpdater->_visAttr->dumpVisAttributes();
			
			//get the scene graph nodes and stuff
			osg::ref_ptr<osg::Node> child = _viewerStuff->_scene._rootNode->getChild(shapeIdx);  // the transformation

			child->accept(*nodeUpdater);
			shapeIdx++;
		} //End for-loop
	}//End function

	void OMVisualizerMAT::createVisAttributes()
	{
		// Update all shapes
		rapidxml::xml_node<>* rootNode = _baseData->_xmlDoc.first_node();
		unsigned int shapeIdx = 0;

		for (rapidxml::xml_node<>* shapeNode = rootNode->first_node("shape"); shapeNode; shapeNode = shapeNode->next_sibling())
		{
			//create a VisAttributes object according to the node
			VisAttributes* visAttr = _baseData->visAttrFactory->createVisAttributes(shapeNode->first_node("type")->value());
			_baseData->allVisAttr[shapeIdx] = visAttr;

			visAttr->updateVisAttributesMAT(shapeNode, omvManager->_startTime, _matReader);

			//update the shapes with a node visitor
			nodeUpdater->_visAttr = visAttr;
			//nodeUpdater->_visAttr->dumpVisAttributes();

			//get the scene graph nodes and stuff
			osg::ref_ptr<osg::Node> child = _viewerStuff->_scene._rootNode->getChild(shapeIdx);  // the transformation

			child->accept(*nodeUpdater);
			shapeIdx++;
		} //End for-loop
	}//End function

	void OMVisualizerMAT::updateScene(double time)
	{
		updateVisAttributes(time);
	}

	int OMVisualizerMAT::getDataTypeID()
	{
		return 0;
	}


}  // End namepsace Model

