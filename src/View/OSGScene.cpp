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

#include "View/OSGScene.hpp"
#include "Visualize.hpp"
#include "Util/Logger.hpp"

#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>
#include <osg/Material>

#include <string>

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

            std::string type = getShapeType(shapeNode);
            LOGGER_WRITE(std::string("Shape: ") + shapeNode->value() + std::string(", type: ") + type,Util::LC_LOADER, Util::LL_DEBUG);

            //color
            osg::ref_ptr<osg::Material> material = new osg::Material();
            material->setDiffuse(osg::Material::FRONT, osg::Vec4f(0.0, 0.0, 0.0, 0.0));

            //matrix transformation
            osg::ref_ptr<osg::MatrixTransform> transf = new osg::MatrixTransform();

            //stl node
            if (isCADType(type))
            {
                std::string filename = extractCADFilename(type);
                LOGGER_WRITE(std::string("Its a STL and the path is ") + _path ,Util::LC_LOADER, Util::LL_INFO);
				filename = _path + filename;

				// \todo What do we do at this point?
                if (!exists(filename))
                    LOGGER_WRITE(std::string("Could not find the file ") + filename +std::string(".") ,Util::LC_LOADER, Util::LL_WARNING);


                osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(filename);
                osg::ref_ptr<osg::StateSet> ss = node->getOrCreateStateSet();

                ss->setAttribute(material.get());
                node->setStateSet(ss);
                transf->addChild(node.get());
            }
            //geode with shape drawable
            else
            {
                osg::ref_ptr<osg::ShapeDrawable> shapeDraw = new osg::ShapeDrawable();
                shapeDraw->setColor(osg::Vec4(1.0, 1.0, 1.0, 1.0));
                geode = new osg::Geode();
                geode->addDrawable(shapeDraw.get());
                osg::ref_ptr<osg::StateSet> ss = geode->getOrCreateStateSet();
                ss->setAttribute(material.get());
                geode->setStateSet(ss);
                transf->addChild(geode.get());
            }

            _rootNode->addChild(transf.get());
        }
    }

    osg::ref_ptr<osg::Group> OSGScene::getRootNode()
    {
        return _rootNode;
    }

    std::string OSGScene::getPath() const
    {
        return _path;
    }

    void OSGScene::setPath(const std::string path)
    {
        _path = path;
    }

}  // End namespace View
