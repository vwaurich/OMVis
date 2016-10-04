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

#include "Model/UpdateVisitor.hpp"
#include "Visualize.hpp"
#include "Util/Logger.hpp"
#include "Util/Util.hpp"

#include <osgDB/ReadFile>
#include <osg/Material>
#include <osg/LineWidth>
#include <osg/Point>

namespace OMVIS
{
    namespace Model
    {

        /*-----------------------------------------
         * CONSTRUCTORS
         *---------------------------------------*/

        UpdateVisitor::UpdateVisitor()
                : _shape()
        {
            setTraversalMode(NodeVisitor::TRAVERSE_ALL_CHILDREN);
        }

        /*-----------------------------------------
         * METHODS
         *---------------------------------------*/

        /**
         MatrixTransform
         */
        void UpdateVisitor::apply(osg::MatrixTransform& node)
        {
            //std::cout<<"MT "<<node.className()<<"  "<<node.getName()<<std::endl;
            node.setMatrix(_shape._mat);
            traverse(node);
        }

        /**
         Geode
         */
        void UpdateVisitor::apply(osg::Geode& node)
        {
            //std::cout<<"GEODE "<< _shape._id<<" "<<std::endl;
            osg::ref_ptr<osg::StateSet> ss = node.getOrCreateStateSet();

            //its a stl-file
            if (Util::isCADType(_shape._type))
            {
                std::string filename = Util::extractCADFilename(_shape._type);
                osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(filename);
            }
            //its a drawable
            else
            {
                osg::ref_ptr<osg::Drawable> draw = node.getDrawable(0);
                draw->dirtyDisplayList();
                //osg::ref_ptr<osg::ShapeDrawable> shapeDraw = dynamic_cast<osg::ShapeDrawable*>(draw.get());
                //shapeDraw->setColor(osg::Vec4(visAttr.color,1.0));

				if (_shape._type == "pipe") {
					node.removeDrawable(draw);
					draw = (new Pipecylinder((_shape._width.exp * _shape._extra.exp) / 2, (_shape._width.exp) / 2, _shape._length.exp))->asDrawable();
				}
				else if (_shape._type == "pipecylinder") {
					node.removeDrawable(draw);
					draw = (new Pipecylinder((_shape._width.exp * _shape._extra.exp) / 2, (_shape._width.exp) / 2, _shape._length.exp))->asDrawable();
				}
				else if (_shape._type == "spring") {
					node.removeDrawable(draw);
					draw = (new Spring(_shape._width.exp, _shape._height.exp, _shape._extra.exp, _shape._length.exp))->asDrawable();
				}
                else if (_shape._type == "cylinder")
                    draw->setShape(new osg::Cylinder(osg::Vec3f(0.0, 0.0, 0.0), _shape._width.exp / 2.0, _shape._length.exp));
                else if (_shape._type == "box")
                    draw->setShape(new osg::Box(osg::Vec3f(0.0, 0.0, 0.0), _shape._width.exp, _shape._height.exp, _shape._length.exp));
                else if (_shape._type == "cone")
                    draw->setShape(new osg::Cone(osg::Vec3f(0.0, 0.0, 0.0), _shape._width.exp / 2.0, _shape._length.exp));
                else if (_shape._type == "sphere")
                    draw->setShape(new osg::Sphere(osg::Vec3f(0.0, 0.0, 0.0), _shape._length.exp / 2.0));
                else
                {
                    LOGGER_WRITE(std::string("Unknown type, we make a capsule."), Util::LC_GUI, Util::LL_WARNING);
                    //string id = string(visAttr.type.begin(), visAttr.type.begin()+11);
                    draw->setShape(new osg::Capsule(osg::Vec3f(0.0, 0.0, 0.0), 0.1, 0.5));
                }
                //std::cout<<"SHAPE "<<draw->getShape()->className()<<std::endl;
                node.addDrawable(draw.get());
            }
            //osg::Material *material = dynamic_cast<osg::Material*>(ss->getAttribute(osg::StateAttribute::MATERIAL));
            osg::ref_ptr<osg::Material> material = new osg::Material;
            material->setDiffuse(osg::Material::FRONT, osg::Vec4f(_shape._color[0].exp / 255, _shape._color[1].exp / 255, _shape._color[2].exp / 255, 1.0));
            ss->setAttribute(material);

			ss->setAttribute(new osg::Point(50.0f));

            node.setStateSet(ss);
            traverse(node);
        }

    }  // End namespace Model
}  // End namespace OMVIS
