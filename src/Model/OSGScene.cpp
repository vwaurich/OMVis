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
#include "Util/Visualize.hpp"
#include "Util/Logger.hpp"
#include "Util/Util.hpp"

#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osgDB/ReadFile>

namespace OMVIS
{
    namespace Model
    {

        /*-----------------------------------------
         * CONSTRUCTORS
         *---------------------------------------*/

        OSGScene::OSGScene()
                : _rootNode(new osg::Group()),
                  _path("")
        {
        }

        /*-----------------------------------------
         * INITIALIZATION METHODS
         *---------------------------------------*/

        void OSGScene::setUpScene(const std::vector<Model::ShapeObject>& allShapes)
        {
            osg::ref_ptr<osg::Geode> geode;
            osg::ref_ptr<osg::StateSet> ss;
            std::string type;
            osg::ref_ptr<osg::Material> material(nullptr);
            osg::Vec4f zeroVec(0.0, 0.0, 0.0, 0.0);
            osg::ref_ptr<osg::MatrixTransform> transf(nullptr);

            for (auto& shape : allShapes)
            {
                type = shape._type;
                LOGGER_WRITE(std::string("Shape: ") + shape._id + std::string(", type: ") + type, Util::LC_LOADER, Util::LL_DEBUG);

                // Color
                material = new osg::Material();
                material->setDiffuse(osg::Material::FRONT, zeroVec);

                // Matrix transformation
                transf = new osg::MatrixTransform();

				//cad node
				if ((shape._type.compare("dxf") == 0) || (shape._type.compare("stl") == 0))
				{
					//std::cout<<"Its a CAD and the filename is "<<shape._fileName<<std::endl;
					osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(shape._fileName);
					osg::ref_ptr<osg::StateSet> ss = node->getOrCreateStateSet();

					ss->setAttribute(material.get());
					node->setStateSet(ss);
					transf->addChild(node.get());
				}
                // Geode with shape drawable
                else
                {
                    auto shapeDraw = new osg::ShapeDrawable();
                    shapeDraw->setColor(osg::Vec4(1.0, 1.0, 1.0, 1.0));
                    geode = new osg::Geode();
                    geode->addDrawable(shapeDraw);
                    osg::ref_ptr<osg::StateSet> ss = geode->getOrCreateStateSet();
                    ss->setAttribute(material.get());
                    geode->setStateSet(ss);
                    transf->addChild(geode);
                }
                _rootNode->addChild(transf.get());
            }
        }

        /*-----------------------------------------
         * GETTERS AND SETTERS
         *---------------------------------------*/

        osg::ref_ptr<osg::Group> OSGScene::getRootNode()
        {
            return _rootNode;
        }

        std::string OSGScene::getPath() const
        {
            return _path;
        }

        void OSGScene::setPath(const std::string& path)
        {
            _path = path;
        }

    }  // namespace Model
}  // namespace OMVIS
