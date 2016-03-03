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
 * UpdateVisitor.cpp
 *
 *  Created on: 03.02.2016
 *      Author: mf
 */

#include "Model/UpdateVisitor.hpp"
#include "Visualize.hpp"

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
        node.setMatrix(_visAttr->getTransformationMatrix());
        traverse(node);
    }

    /**
     Geode
     */
    void UpdateVisitor::apply(osg::Geode& node)
    {
        //std::cout<<"GEODE "<<_visAttr->getType()<<" "<<std::endl;
        osg::ref_ptr<osg::StateSet> ss = node.getOrCreateStateSet();

        //its a stl-file
        if (isCADType(_visAttr->getType()))
        {
            std::string filename = extractCADFilename(_visAttr->getType());
            osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(filename);

        }
        //its a drawable
        else
        {
            osg::ref_ptr<osg::Drawable> draw = node.getDrawable(0);
            draw->dirtyDisplayList();

			//udpate the attributes for the drawable
			_visAttr->updateDrawable(draw);

            //cout<<"SHAPE "<<draw->getShape()->className()<<endl;
            node.addDrawable(draw.get());
        }
        //osg::Material *material = dynamic_cast<osg::Material*>(ss->getAttribute(osg::StateAttribute::MATERIAL));
        osg::ref_ptr<osg::Material> material = new osg::Material;

        material->setDiffuse(osg::Material::FRONT, osg::Vec4(_visAttr->getColor() / 255, 1.0));
        ss->setAttribute(material);
        node.setStateSet(ss);
        traverse(node);
    }

}  // End namespace Model
