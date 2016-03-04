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

#include "Model/ShapeAttributes.hpp"
#include "Visualize.hpp"

namespace Model
{

	ShapeAttributes::ShapeAttributes()
            : VisAttributes(),
			  _type("box"),
              _length(1.0),
              _width(0.1),
              _height(0.1),
              _r(osg::Vec3f(0.0, 0.0, 0.0)),
              _rShape(osg::Vec3f(0.0, 0.0, 0.0)),
              _lDir(osg::Vec3f(1.0, 0.0, 0.0)),
              _wDir(osg::Vec3f(0.0, 1.0, 0.0)),
              _color(osg::Vec3f(1.0, 1.0, 1.0)),
              _T(osg::Matrix3(0, 0, 1, 1, 0, 0, 0, 1, 0)),
              _mat(osg::Matrix(0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1.0))
    {
    }

    void ShapeAttributes::dumpVisAttributes()
    {
        std::cout << "type " << _type << std::endl;
        std::cout << "length " << _length << std::endl;
        std::cout << "width " << _width << std::endl;
        std::cout << "height " << _height << std::endl;
        std::cout << "lDir " << _lDir[0] << ", " << _lDir[1] << ", " << _lDir[2] << ", " << std::endl;
        std::cout << "wDir " << _wDir[0] << ", " << _wDir[1] << ", " << _wDir[2] << ", " << std::endl;
        std::cout << "r " << _r[0] << ", " << _r[1] << ", " << _r[2] << ", " << std::endl;
        std::cout << "r_shape " << _rShape[0] << ", " << _rShape[1] << ", " << _rShape[2] << ", " << std::endl;
        std::cout << "lDir0 " << _lDir[0] << ", " << _lDir[1] << ", " << _lDir[2] << ", " << std::endl;
        std::cout << "wDir0 " << _wDir[0] << ", " << _wDir[1] << ", " << _wDir[2] << ", " << std::endl;
        std::cout << "T0 " << _T[0] << ", " << _T[1] << ", " << _T[2] << ", " << std::endl;
        std::cout << "   " << _T[3] << ", " << _T[4] << ", " << _T[5] << ", " << std::endl;
        std::cout << "   " << _T[6] << ", " << _T[7] << ", " << _T[8] << ", " << std::endl;
        std::cout << "mat " << _mat(0, 0) << ", " << _mat(0, 1) << ", " << _mat(0, 2) << ", " << _mat(0, 3) << ", " << std::endl;
        std::cout << "    " << _mat(1, 0) << ", " << _mat(1, 1) << ", " << _mat(1, 2) << ", " << _mat(1, 3) << ", " << std::endl;
        std::cout << "    " << _mat(2, 0) << ", " << _mat(2, 1) << ", " << _mat(2, 2) << ", " << _mat(2, 3) << ", " << std::endl;
        std::cout << "    " << _mat(3, 0) << ", " << _mat(3, 1) << ", " << _mat(3, 2) << ", " << _mat(3, 3) << ", " << std::endl;
        std::cout << "color " << _color[0] << ", " << _color[1] << ", " << _color[2] << ", " << std::endl;
    }

	void ShapeAttributes::updateVisAttributesFMU(rapidxml::xml_node<>* node, double time, fmi1_import_t* fmu)
	{
		// get the values for the scene graph objects
		_type = getNodeType(node);
		_length = getNodeAttrFMU((const char*) "length", node, time, fmu);
		_width = getNodeAttrFMU((const char*) "width", node, time, fmu);
		_height = getNodeAttrFMU((const char*) "height", node, time, fmu);
		_r = getNodeVectorFMU((char*) "r", node, time, fmu);
		_rShape = getNodeVectorFMU((char*) "r_shape", node, time, fmu);
		_lDir = getNodeVectorFMU((char*) "lengthDir", node, time, fmu);
		_wDir = getNodeVectorFMU((char*) "widthDir", node, time, fmu);
		_color = getNodeVectorFMU((char*) "color", node, time, fmu);
		_T = getNodeMatrixFMU((char*) "T", node, time, fmu);
		rAndT rT = staticRotation(_r, _rShape, _T,_lDir, _wDir, _length, _width, _height, _type);
		_r = rT._r;
		_T = rT._T;
		_mat = assemblePokeMatrix(_mat, _T, _r);
	}

	void ShapeAttributes::updateVisAttributesMAT(rapidxml::xml_node<>* node, double time, ModelicaMatReader matReader)
	{
		// get the values for the scene graph objects
		_type = getNodeType(node);
		_length = getNodeAttrMAT((const char*) "length", node, time, matReader);
		_width = getNodeAttrMAT((const char*) "width", node, time, matReader);
		_height = getNodeAttrMAT((const char*) "height", node, time, matReader);
		_r = getNodeVectorMAT((char*) "r", node, time, matReader);
		_rShape = getNodeVectorMAT((char*) "r_shape", node, time, matReader);
		_lDir = getNodeVectorMAT((char*) "lengthDir", node, time, matReader);
		_wDir = getNodeVectorMAT((char*) "widthDir", node, time, matReader);
		_color = getNodeVectorMAT((char*) "color", node, time, matReader);
		_T = getNodeMatrixMAT((char*) "T", node, time, matReader);
		rAndT rT = staticRotation(_r, _rShape, _T, _lDir, _wDir, _length, _width, _height, _type);
		_r = rT._r;
		_T = rT._T;
		_mat = assemblePokeMatrix(_mat, _T, _r);
	}

	osg::Matrix ShapeAttributes::getTransformationMatrix()
	{
		return _mat;
	}

	std::string ShapeAttributes::getType()
	{
		return _type;
	}

	osg::Vec3f ShapeAttributes::getColor()
	{
		return _color;
	}


	bool ShapeAttributes::isShapeVisualizer()
	{
		return true;
	}

	void ShapeAttributes::updateGeode(osg::Geode* node)
	{
		osg::ref_ptr<osg::Drawable> draw = node->getDrawable(0);
		draw->dirtyDisplayList();

		if (_type == "pipecylinder")
			draw->setShape(new osg::Cylinder(osg::Vec3f(0.0, 0.0, 0.0), _width / 2, _length));
		else if (_type == "cylinder")
			draw->setShape(new osg::Cylinder(osg::Vec3f(0.0, 0.0, 0.0), _width / 2, _length));
		else if (_type == "box")
			draw->setShape(new osg::Box(osg::Vec3f(0.0, 0.0, 0.0), _width, _height, _length));
		else if (_type == "cone")
			draw->setShape(new osg::Cone(osg::Vec3f(0.0, 0.0, 0.0), _width / 2, _length));
		else if (_type == "sphere")
			draw->setShape(new osg::Sphere(osg::Vec3f(0.0, 0.0, 0.0), _length / 2));
		else
		{
			std::cout << "UNKNOWN TYPE, WE MAKE A CAPSULE " << std::endl;
			//string id = string(visAttr.type.begin(), visAttr.type.begin()+11);
			draw->setShape(new osg::Capsule(osg::Vec3f(0.0, 0.0, 0.0), 0.1, 0.5));
		}

		node->addDrawable(draw.get());
	}

	void ShapeAttributes::setMaterial(osg::ref_ptr<osg::StateSet> stateSet)
	{ 
		//osg::Material *material = dynamic_cast<osg::Material*>(ss->getAttribute(osg::StateAttribute::MATERIAL));
		osg::ref_ptr<osg::Material> material = new osg::Material;

		material->setDiffuse(osg::Material::FRONT, osg::Vec4(_color / 255, 1.0));
		stateSet->setAttribute(material);
	}

}  // End namespace Model
