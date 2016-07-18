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

#include "Model/OSGScene.hpp"
#include "Visualize.hpp"
#include "Util/Logger.hpp"
#include "Util/Util.hpp"

#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>
#include <osg/Material>

#include <string>

namespace OMVIS
{
    namespace Model
    {

    OSGScene::OSGScene()
            : _rootNode(new osg::Group)
    {
    }

    int OSGScene::setUpScene(std::vector<Model::ShapeObject> allShapes)
    {
        int isOk(0);
		for (std::vector<Model::ShapeObject>::size_type i = 0; i != allShapes.size(); i++)
        {
			Model::ShapeObject shape = allShapes[i];

			osg::ref_ptr<osg::Geode> geode;
			osg::ref_ptr<osg::StateSet> ss;
 
            std::string type = shape._type;
			LOGGER_WRITE(std::string("Shape: ") + shape._id + std::string(", type: ") + type,Util::LC_LOADER, Util::LL_DEBUG);

			//color
			osg::ref_ptr<osg::Material> material = new osg::Material();
			material->setDiffuse(osg::Material::FRONT, osg::Vec4f(0.0, 0.0, 0.0, 0.0));

			//matrix transformation
			osg::ref_ptr<osg::MatrixTransform> transf = new osg::MatrixTransform();

			//stl node
			if (Util::isCADType(type))
			{

				std::string filename = Util::extractCADFilename(type);
				LOGGER_WRITE(std::string("Its a STL and the path is ") + _path ,Util::LC_LOADER, Util::LL_INFO);
				filename = _path + filename;
				// \todo What do we do at this point?
				if (!Util::fileExists(filename))
				{
					LOGGER_WRITE(std::string("Could not find the file ") + filename +std::string(".") ,Util::LC_LOADER, Util::LL_WARNING);
					isOk = 1;
					return isOk;
                }

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

        return isOk;
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

    }  // End namespace Model
}  // End namespace OMVIS