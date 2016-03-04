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

#include "Model/PrismAttributes.hpp"
#include "Visualize.hpp"

namespace Model
{

	PrismAttributes::PrismAttributes()
            : VisAttributes(),
			  _type("prism"),
              _length(1.0),
			  _radius(0.1),
              _numEdges(3),
              _r(osg::Vec3f(0.0, 0.0, 0.0)),
              _rShape(osg::Vec3f(0.0, 0.0, 0.0)),
              _lDir(osg::Vec3f(1.0, 0.0, 0.0)),
              _color0(osg::Vec3f(1.0, 1.0, 1.0)),
		      _color1(osg::Vec3f(0.5, 0.5, 0.5)),
              _T(osg::Matrix3(0, 0, 1, 1, 0, 0, 0, 1, 0)),
              _mat(osg::Matrix(0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1.0))
    {
    }

    void PrismAttributes::dumpVisAttributes()
    {
        std::cout << "type " << _type << std::endl;
        std::cout << "length " << _length << std::endl;
        std::cout << "_radius " << _radius << std::endl;
        std::cout << "_numEdges " << _numEdges << std::endl;
        std::cout << "lDir " << _lDir[0] << ", " << _lDir[1] << ", " << _lDir[2] << ", " << std::endl;
        std::cout << "r " << _r[0] << ", " << _r[1] << ", " << _r[2] << ", " << std::endl;
        std::cout << "r_shape " << _rShape[0] << ", " << _rShape[1] << ", " << _rShape[2] << ", " << std::endl;
        std::cout << "T0 " << _T[0] << ", " << _T[1] << ", " << _T[2] << ", " << std::endl;
        std::cout << "   " << _T[3] << ", " << _T[4] << ", " << _T[5] << ", " << std::endl;
        std::cout << "   " << _T[6] << ", " << _T[7] << ", " << _T[8] << ", " << std::endl;
        std::cout << "mat " << _mat(0, 0) << ", " << _mat(0, 1) << ", " << _mat(0, 2) << ", " << _mat(0, 3) << ", " << std::endl;
        std::cout << "    " << _mat(1, 0) << ", " << _mat(1, 1) << ", " << _mat(1, 2) << ", " << _mat(1, 3) << ", " << std::endl;
        std::cout << "    " << _mat(2, 0) << ", " << _mat(2, 1) << ", " << _mat(2, 2) << ", " << _mat(2, 3) << ", " << std::endl;
        std::cout << "    " << _mat(3, 0) << ", " << _mat(3, 1) << ", " << _mat(3, 2) << ", " << _mat(3, 3) << ", " << std::endl;
        std::cout << "color0 " << _color0[0] << ", " << _color0[1] << ", " << _color0[2] << ", " << std::endl;
		std::cout << "color1 " << _color1[0] << ", " << _color1[1] << ", " << _color1[2] << ", " << std::endl;
    }

	void PrismAttributes::updateVisAttributesFMU(rapidxml::xml_node<>* node, double time, fmi1_import_t* fmu)
	{
		// get the values for the scene graph objects
		_type = getNodeType(node);
		_length = getNodeAttrFMU((const char*) "length", node, time, fmu);
		_radius = getNodeAttrFMU((const char*) "radius", node, time, fmu);
		_numEdges = getNodeAttrFMU((const char*) "numEdges", node, time, fmu);
		_r = getNodeVectorFMU((char*) "r", node, time, fmu);
		_rShape = getNodeVectorFMU((char*) "r_shape", node, time, fmu);
		_lDir = getNodeVectorFMU((char*) "lengthDir", node, time, fmu);
		_color0 = getNodeVectorFMU((char*) "color0", node, time, fmu);
		_color1 = getNodeVectorFMU((char*) "color1", node, time, fmu);
		_T = getNodeMatrixFMU((char*) "T", node, time, fmu);
		//rAndT rT = staticRotation(_r, _rShape, _T,_lDir, _wDir, _length, _width, _height, _type);
		//_r = rT._r;
		//_T = rT._T;
		_mat = assemblePokeMatrix(_mat, _T, _r);
	}

	void PrismAttributes::updateVisAttributesMAT(rapidxml::xml_node<>* node, double time, ModelicaMatReader matReader)
	{
		// get the values for the scene graph objects
		_type = getNodeType(node);
		_length = getNodeAttrMAT((const char*) "length", node, time, matReader);
		_radius = getNodeAttrMAT((const char*) "radius", node, time, matReader);
		_numEdges = getNodeAttrMAT((const char*) "numEdges", node, time, matReader);
		_r = getNodeVectorMAT((char*) "r", node, time, matReader);
		_rShape = getNodeVectorMAT((char*) "r_shape", node, time, matReader);
		_lDir = getNodeVectorMAT((char*) "lengthDir", node, time, matReader);
		_color0 = getNodeVectorMAT((char*) "color0", node, time, matReader);
		_color1 = getNodeVectorMAT((char*) "color1", node, time, matReader);
		_T = getNodeMatrixMAT((char*) "T", node, time, matReader);
		rAndT rT = staticRotation(_r, _rShape, _T, _lDir, osg::Vec3(0.0,1.0,0.0), _length, 0.0, 0.0, _type);
		_r = rT._r;
		_T = rT._T;
		_mat = assemblePokeMatrix(_mat, _T, _r);
	}

	osg::Matrix PrismAttributes::getTransformationMatrix()
	{
		return _mat;
	}

	std::string PrismAttributes::getType()
	{
		return _type;
	}

	bool PrismAttributes::isShapeVisualizer()
	{
		return false;
	}

	bool PrismAttributes::isPrismVisualizer()
	{
		return true;
	}
	void PrismAttributes::updateGeode(osg::Geode* prismGeode)
	{
		osg::Geometry* prismGeometry = new osg::Geometry;
		prismGeode->addDrawable(prismGeometry);
		//the base points
		osg::Vec3Array* prismVertices = new osg::Vec3Array;
		osg::Vec4Array* colors = new osg::Vec4Array;

		for (int i = 0; i < _numEdges; i++)
		{
			float x = sin(2 * M_PI / _numEdges * i)*_radius;
			float y = cos(2 * M_PI / _numEdges * i)*_radius;
			prismVertices->push_back(osg::Vec3(x, y, 0));
			colors->push_back(osg::Vec4((_color0 / 255.0), 1.0f));
		}
		//the top points
		for (int i = 0; i < _numEdges; i++)
		{
			float x = sin(2 * M_PI / _numEdges * i)*_radius;
			float y = cos(2 * M_PI / _numEdges * i)*_radius;
			prismVertices->push_back(osg::Vec3(x, y, _length));
			colors->push_back(osg::Vec4((_color1 / 255.0), 1.0f));
		}

		prismGeometry->setVertexArray(prismVertices);

		//the side faces
		for (int i = 0; i < _numEdges; i++)
		{
			if (i == _numEdges - 1)
			{
				osg::DrawElementsUInt* sideFace =
					new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
				sideFace->push_back(i);
				sideFace->push_back(0);
				sideFace->push_back(_numEdges);
				sideFace->push_back(i+_numEdges);
				prismGeometry->addPrimitiveSet(sideFace);
			}
			else
			{
				osg::DrawElementsUInt* sideFace =
					new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
				sideFace->push_back(i);
				sideFace->push_back(i+1);
				sideFace->push_back(i+_numEdges+1);
				sideFace->push_back(i + _numEdges);
				prismGeometry->addPrimitiveSet(sideFace);
			}
		}
		//the base face
		osg::DrawElementsUInt* baseFace =
			new osg::DrawElementsUInt(osg::PrimitiveSet::POLYGON, 0);
		for (int i = 0; i < _numEdges; i++)
		{
			std::cout << "base " << i << std::endl;
			baseFace->push_back(i);
		}
		prismGeometry->addPrimitiveSet(baseFace);

		//the top face
		osg::DrawElementsUInt* topFace =
			new osg::DrawElementsUInt(osg::PrimitiveSet::POLYGON, 0);
		for (int i = 0; i < _numEdges; i++)
		{
			std::cout << "top " << i << std::endl;
			topFace->push_back(i+_numEdges);
		}
		prismGeometry->addPrimitiveSet(topFace);

		//set color for vertexes
		prismGeometry->setColorArray(colors);
		osg::Vec4Array* arr = (osg::Vec4Array*)prismGeometry->getColorArray();
		std::cout << "COLOR0: " << arr->at(0)[0] <<" "<< arr->at(0)[1] <<" "<< arr->at(0)[2] << std::endl;

		prismGeometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	}
	
	void PrismAttributes::setMaterial(osg::ref_ptr<osg::StateSet> stateSet)
	{
		//TODO: DEFINE NORMALS FOR VERTEXES
		stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	}

}  // End namespace Model
