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

namespace Model
{

    UpdateVisitor::UpdateVisitor()
            : _visAttr()
    {
        setTraversalMode(NodeVisitor::TRAVERSE_ALL_CHILDREN);
    }

    /**
     MatrixTransform
     */
    void UpdateVisitor::apply(osg::MatrixTransform& node)
    {
        //std::cout<<"MT "<<node.className()<<"  "<<node.getName()<<std::endl;
        node.setMatrix(_visAttr._mat);
        traverse(node);
    }

    /**
     Geode
     */
    void UpdateVisitor::apply(osg::Geode& node)
    {
        //std::cout<<"GEODE "<<visAttr.type<<" "<<std::endl;
        osg::ref_ptr<osg::StateSet> ss = node.getOrCreateStateSet();

        //its a stl-file
        if (isCADType(_visAttr._type))
        {
            std::string filename = extractCADFilename(_visAttr._type);
            osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(filename);

        }
        //its a drawable
        else
        {
            osg::ref_ptr<osg::Drawable> draw = node.getDrawable(0);
            draw->dirtyDisplayList();
            //osg::ref_ptr<osg::ShapeDrawable> shapeDraw = dynamic_cast<osg::ShapeDrawable*>(draw.get());
            //shapeDraw->setColor(osg::Vec4(visAttr.color,1.0));

            if (_visAttr._type == "pipecylinder")
                draw->setShape(new osg::Cylinder(osg::Vec3f(0.0, 0.0, 0.0), _visAttr._width / 2.0, _visAttr._length));
            else if (_visAttr._type == "cylinder")
                draw->setShape(new osg::Cylinder(osg::Vec3f(0.0, 0.0, 0.0), _visAttr._width / 2.0, _visAttr._length));
            else if (_visAttr._type == "box")
                draw->setShape(new osg::Box(osg::Vec3f(0.0, 0.0, 0.0), _visAttr._width, _visAttr._height, _visAttr._length));
            else if (_visAttr._type == "cone")
                draw->setShape(new osg::Cone(osg::Vec3f(0.0, 0.0, 0.0), _visAttr._width / 2.0, _visAttr._length));
            else if (_visAttr._type == "sphere")
                draw->setShape(new osg::Sphere(osg::Vec3f(0.0, 0.0, 0.0), _visAttr._length / 2.0));
            else
            {
                LOGGER_WRITE(std::string("Unknown type, we make a capsule."), Util::LC_GUI, Util::LL_WARNING);
                //string id = string(visAttr.type.begin(), visAttr.type.begin()+11);
                draw->setShape(new osg::Capsule(osg::Vec3f(0.0, 0.0, 0.0), 0.1, 0.5));
            }

            //cout<<"SHAPE "<<draw->getShape()->className()<<endl;
            node.addDrawable(draw.get());
        }
        //osg::Material *material = dynamic_cast<osg::Material*>(ss->getAttribute(osg::StateAttribute::MATERIAL));
        osg::ref_ptr<osg::Material> material = new osg::Material;

        material->setDiffuse(osg::Material::FRONT, osg::Vec4(_visAttr._color / 255, 1.0));
        ss->setAttribute(material);
        node.setStateSet(ss);
        traverse(node);
    }

}  // End namespace Model
