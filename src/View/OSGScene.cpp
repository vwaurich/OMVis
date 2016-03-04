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
 * OSGScene.cpp
 *
 *  Created on: 03.02.2016
 *      Author: mf
 */

#include <string>
#include <iostream>
#include "View/OSGScene.hpp"
#include "Visualize.hpp"

namespace View
{

    OSGScene::OSGScene()
            : _rootNode(new osg::Group)
    {
    }

    void OSGScene::setUpScene(rapidxml::xml_node<>* xmlRoot)
    {
        for (rapidxml::xml_node<>* shapeNode = xmlRoot->first_node("shape"); shapeNode; shapeNode = shapeNode->next_sibling())
        {
            osg::ref_ptr<osg::Geode> geode;
            osg::ref_ptr<osg::StateSet> ss;

            std::string type = getNodeType(shapeNode);
            std::cout << "we have the shape " << shapeNode->value() << " with type " << type << std::endl;

            //color
            osg::ref_ptr<osg::Material> material = new osg::Material;
            material->setDiffuse(osg::Material::FRONT, osg::Vec4f(0.0, 0.0, 0.0, 0.0));

            //matrix transformation
            osg::ref_ptr<osg::MatrixTransform> transf = new osg::MatrixTransform;

            //stl node
            if (isCADType(type))
            {
                std::string filename = extractCADFilename(type);
				std::cout << "its a stl" << std::endl;
				std::cout << path << std::endl;
                filename = path + filename;
                std::cout << "its a stl file1. " << filename << std::endl;

                if (!exists(filename))
                    std::cout << filename << " not found" << std::endl;

                osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(filename);
                osg::ref_ptr<osg::StateSet> ss = node->getOrCreateStateSet();

                ss->setAttribute(material.get());
                node->setStateSet(ss);
                transf->addChild(node.get());
            }

            //geode with shape drawable
            else
            {
                osg::ref_ptr<osg::ShapeDrawable> shapeDraw = new osg::ShapeDrawable;
                shapeDraw->setColor(osg::Vec4(1.0, 1.0, 1.0, 1.0));
                geode = new osg::Geode;
                geode->addDrawable(shapeDraw.get());
                osg::ref_ptr<osg::StateSet> ss = geode->getOrCreateStateSet();
                ss->setAttribute(material.get());
                geode->setStateSet(ss);
                transf->addChild(geode.get());
            }
            _rootNode->addChild(transf.get());
        }
    }

}  // End namespace View
