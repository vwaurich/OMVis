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

#include "Util/Visualize.hpp"
#include "Util/Algebra.hpp"
#include "Util/Util.hpp"
#include "Util/Expression.hpp"
#include "Model/ShapeObjectAttribute.hpp"

namespace OMVIS
{
    namespace Util
    {

        /******************************
         Calculate Transformations
         *******************************/

        osg::Vec3f normalize(const osg::Vec3f& vec)
        {
            osg::Vec3f vecOut;
            if (vec.length() >= 100 * 1.e-15)
                vecOut = vec / vec.length();
            else
                vecOut = vec / 100 * 1.e-15;

            return vecOut;
        }

        osg::Vec3f cross(const osg::Vec3f& vec1, const osg::Vec3f& vec2)
        {
            return osg::Vec3f(vec1[1] * vec2[2] - vec1[2] * vec2[1],
                              vec1[2] * vec2[0] - vec1[0] * vec2[2],
                              vec1[0] * vec2[1] - vec1[1] * vec2[0]);
        }

        Directions fixDirections(const osg::Vec3f& lDir, const osg::Vec3f& wDir)
        {
            Directions dirs;
            osg::Vec3f e_x;
            osg::Vec3f e_y;

            //lengthDirection
            double abs_n_x = lDir.length();
            if (abs_n_x < 1e-10)
                e_x = osg::Vec3f(1.0, 0.0, 0.0);
            else
                e_x = lDir / abs_n_x;

            //widthDirection
            osg::Vec3f n_z_aux = cross(e_x, wDir);
            osg::Vec3f e_y_aux;
            if (n_z_aux * n_z_aux > 1e-6)
                e_y_aux = wDir;
            else
            {
                if (fabs(e_x[0]) > 1e-6)
                    e_y_aux = osg::Vec3f(0, 1, 0);
                else
                    e_y_aux = osg::Vec3f(1, 0, 0);
            }
            e_y = cross(normalize(cross(e_x, e_y_aux)), e_x);

            /*
             lDir.normalize();
             wDir.normalize();

             //empty length direction
             if (lDir[0]==0.0 && lDir[1]==0.0 && lDir[2]==0.0) {
             lDir = osg::Vec3f(0.0,0.0,1.0);
             wDir = osg::Vec3f(1.0,0.0,0.0);
             }
             //colinear vectors
             if (std::abs((double)lDir[0])==std::abs((double)wDir[0]) && std::abs((double)lDir[1])==std::abs((double)wDir[1]) && std::abs((double)lDir[2])==std::abs((double)wDir[2]))
             {
             if (lDir[0]==0.0)
             {
             wDir = osg::Vec3f(1.0,0.0,0.0);
             }
             else if (lDir[1]==0.0)
             {
             wDir = osg::Vec3f(0.0,1.0,0.0);
             }
             else if (lDir[2]==0.0)
             {
             wDir = osg::Vec3f(0.0,0.0,1.0);
             }
             else
             {
             std::cout<<"fixDirections needs review"<<std::endl;
             }
             }
             */
            dirs._lDir = e_x;
            dirs._wDir = e_y;
            return dirs;
        }

        void assemblePokeMatrix(osg::Matrix& M, const osg::Matrix3& T, const osg::Vec3f& r)
        {
            M(3, 3) = 1.0;
            for (int row = 0; row < 3; ++row)
            {
                M(3, row) = r[row];
                M(row, 3) = 0.0;
                for (int col = 0; col < 3; ++col)
                    M(row, col) = T[row * 3 + col];
            }
        }

        rAndT rotation(const osg::Vec3f& r, const osg::Vec3f& r_shape, const osg::Matrix3& T,
                       const osg::Vec3f& lDirIn, const osg::Vec3f& wDirIn,
                       const float length, const std::string& type)
        {
            rAndT res;

            Directions dirs = fixDirections(lDirIn, wDirIn);
            osg::Vec3f hDir = dirs._lDir ^ dirs._wDir;

            //std::cout<<"lDir1 "<<dirs.lDir[0]<<", "<<dirs.lDir[1]<<", "<<dirs.lDir[2]<<", "<<std::endl;
            //std::cout<<"wDir1 "<<dirs.wDir[0]<<", "<<dirs.wDir[1]<<", "<<dirs.wDir[2]<<", "<<std::endl;
            //std::cout<<"hDir "<<hDir[0]<<", "<<hDir[1]<<", "<<hDir[2]<<", "<<std::endl;

            osg::Vec3f r_offset = osg::Vec3f(0.0, 0.0, 0.0);  // since in osg, the rotation starts in the symmetric centre and in msl at the end of the body, we need an offset here of l/2 for some geometries
            osg::Matrix3 T0 = osg::Matrix3(dirs._wDir[0], dirs._wDir[1], dirs._wDir[2], hDir[0], hDir[1], hDir[2], dirs._lDir[0], dirs._lDir[1], dirs._lDir[2]);

            if (type == "cylinder" || type == "box")
            {
                /*
                 r = r + r_shape;
                 r_offset = dirs.lDir*length/2.0;
                 r_offset = V3mulMat3(r_offset,T);
                 res.r = r+r_offset;
                 */
                r_offset = dirs._lDir * length / 2.0;
                res._r = Util::V3mulMat3(r_shape + r_offset, T) + r;
                res._T = Util::Mat3mulMat3(T0, T);
            }
            else if (type == "sphere")
            {
                T0 = osg::Matrix3(dirs._lDir[0], dirs._lDir[1], dirs._lDir[2], dirs._wDir[0], dirs._wDir[1], dirs._wDir[2], hDir[0], hDir[1], hDir[2]);
                r_offset = dirs._lDir * length / 2.0;
                res._r = Util::V3mulMat3(r_shape + r_offset, T) + r;
                res._T = Util::Mat3mulMat3(T0, T);
            }
            else if (Util::isCADType(type))
            {
                //!r = r + r_shape;
                res._T = T;
                res._r += r_shape;
                //r_offset = dirs.lDir*length/2.0;
            }
			else if (type == "stl" || type == "dxf")
			{
				res._r = r + r_shape;
				res._T = T;
			}
            else if (type == "pipecylinder" || type == "spring" || type == "cone")
            {
                res._r = V3mulMat3(r_shape, T) + r;
                res._T = Mat3mulMat3(T0, T);
            }
            else
            {
                r_offset = dirs._lDir * length / 2.0;
                res._r = Util::V3mulMat3(r_shape + r_offset, T) + r;
                res._T = Util::Mat3mulMat3(T0, T);
            }

//            std::cout<<"res_r "<<res._r[0]<<", "<<res._r[1]<<", "<<res._r[2]<<", "<<std::endl;
//            std::cout << "------------------------------\n";
            return res;
        }



        /****************************
         * Extract Shape information
         *****************************/

//X13 unused?
//        osg::Matrix3 getShapeMatrixMAT(char* attr, rapidxml::xml_node<>* node, double time, ModelicaMatReader reader)
//        {
//            rapidxml::xml_node<>* x1Node = node->first_node(attr)->first_node();
//            rapidxml::xml_node<>* x2Node = x1Node->next_sibling();
//            rapidxml::xml_node<>* x3Node = x2Node->next_sibling();
//            rapidxml::xml_node<>* y1Node = x3Node->next_sibling();
//            rapidxml::xml_node<>* y2Node = y1Node->next_sibling();
//            rapidxml::xml_node<>* y3Node = y2Node->next_sibling();
//            rapidxml::xml_node<>* z1Node = y3Node->next_sibling();
//            rapidxml::xml_node<>* z2Node = z1Node->next_sibling();
//            rapidxml::xml_node<>* z3Node = z2Node->next_sibling();
//            double x1 = evaluateExpressionMAT(x1Node, time, reader);
//            double x2 = evaluateExpressionMAT(x2Node, time, reader);
//            double x3 = evaluateExpressionMAT(x3Node, time, reader);
//            double y1 = evaluateExpressionMAT(y1Node, time, reader);
//            double y2 = evaluateExpressionMAT(y2Node, time, reader);
//            double y3 = evaluateExpressionMAT(y3Node, time, reader);
//            double z1 = evaluateExpressionMAT(z1Node, time, reader);
//            double z2 = evaluateExpressionMAT(z2Node, time, reader);
//            double z3 = evaluateExpressionMAT(z3Node, time, reader);
//            return osg::Matrix3(x1, x2, x3, y1, y2, y3, z1, z2, z3);
//        }

//X13 unused?
//        osg::Matrix3 getShapeMatrixFMU(char* attr, rapidxml::xml_node<>* node, double time, fmi1_import_t* fmu)
//        {
//            rapidxml::xml_node<>* x1Node = node->first_node(attr)->first_node();
//            rapidxml::xml_node<>* x2Node = x1Node->next_sibling();
//            rapidxml::xml_node<>* x3Node = x2Node->next_sibling();
//            rapidxml::xml_node<>* y1Node = x3Node->next_sibling();
//            rapidxml::xml_node<>* y2Node = y1Node->next_sibling();
//            rapidxml::xml_node<>* y3Node = y2Node->next_sibling();
//            rapidxml::xml_node<>* z1Node = y3Node->next_sibling();
//            rapidxml::xml_node<>* z2Node = z1Node->next_sibling();
//            rapidxml::xml_node<>* z3Node = z2Node->next_sibling();
//            double x1 = evaluateExpressionFMU(x1Node, time, fmu);
//            double x2 = evaluateExpressionFMU(x2Node, time, fmu);
//            double x3 = evaluateExpressionFMU(x3Node, time, fmu);
//            double y1 = evaluateExpressionFMU(y1Node, time, fmu);
//            double y2 = evaluateExpressionFMU(y2Node, time, fmu);
//            double y3 = evaluateExpressionFMU(y3Node, time, fmu);
//            double z1 = evaluateExpressionFMU(z1Node, time, fmu);
//            double z2 = evaluateExpressionFMU(z2Node, time, fmu);
//            double z3 = evaluateExpressionFMU(z3Node, time, fmu);
//            return osg::Matrix3(x1, x2, x3, y1, y2, y3, z1, z2, z3);
//        }

//X13 unused?
//        osg::Vec3f getShapeVectorMAT(char* attr, rapidxml::xml_node<>* node, double time, ModelicaMatReader reader)
//        {
//            rapidxml::xml_node<>* xNode = node->first_node(attr)->first_node();
//            rapidxml::xml_node<>* yNode = xNode->next_sibling();
//            rapidxml::xml_node<>* zNode = yNode->next_sibling();
//            double x = evaluateExpressionMAT(xNode, time, reader);
//            double y = evaluateExpressionMAT(yNode, time, reader);
//            double z = evaluateExpressionMAT(zNode, time, reader);
//            return osg::Vec3f(x, y, z);
//        }

//X13 unused?
//        osg::Vec3f getShapeVectorFMU(char* attr, rapidxml::xml_node<>* node, double time, fmi1_import_t* fmu)
//        {
//            rapidxml::xml_node<>* xNode = node->first_node(attr)->first_node();
//            rapidxml::xml_node<>* yNode = xNode->next_sibling();
//            rapidxml::xml_node<>* zNode = yNode->next_sibling();
//            double x = evaluateExpressionFMU(xNode, time, fmu);
//            double y = evaluateExpressionFMU(yNode, time, fmu);
//            double z = evaluateExpressionFMU(zNode, time, fmu);
//            return osg::Vec3f(x, y, z);
//        }

//        // Todo pass by const ref
//        void updateObjectAttributeMAT(Model::ShapeObjectAttribute* attr, double time, ModelicaMatReader reader)
//        {
//            if (!attr->isConst)
//            {
//                attr->exp = *omc_get_varValue(&reader, attr->cref.c_str(), time);
//            }
//        }

//        // Todo pass by const ref
//        void updateObjectAttributeFMU(Model::ShapeObjectAttribute* attr, double time, fmi1_import_t* fmu)
//        {
//            if (!attr->isConst)
//            {
//                fmi1_real_t a = attr->exp;
//                fmi1_import_get_real(fmu, &attr->fmuValueRef, 1, &a);
//                attr->exp = (float) a;
//            }
//        }

        void updateObjectAttributeFMUClient(Model::ShapeObjectAttribute& attr, const NetOff::ValueContainer& _outputCont)
        {
            if (!attr.isConst)
                attr.exp = (float)(_outputCont.getRealValues()[attr.fmuValueRef]);
                //attr.exp = reinterpret_cast<float>(_outputCont.getRealValues()[attr.fmuValueRef]);
        }

//X13 unused?
//        double getShapeAttrMAT(const char* attr, rapidxml::xml_node<>* node, double time, ModelicaMatReader reader)
//        {
//            rapidxml::xml_node<>* expNode = node->first_node(attr)->first_node();
//            double val = evaluateExpressionMAT(expNode, time, reader);
//            return val;
//        }

        double getShapeAttrFMU(const char* attr, rapidxml::xml_node<>* node, double time, fmi1_import_t* fmu)
        {
            rapidxml::xml_node<>* expNode = node->first_node(attr)->first_node();
            return evaluateExpressionFMU(expNode, time, fmu);
        }

        Model::ShapeObjectAttribute getObjectAttributeForNode(const rapidxml::xml_node<>* node)
        {
            Model::ShapeObjectAttribute oa;
            if (strcmp("exp", node->name()) == 0)
            {
                oa.exp = std::strtod(node->value(), nullptr);
                oa.cref = "NONE";
                oa.isConst = true;
            }
            else if (strcmp("cref", node->name()) == 0)
            {
                char* cref = node->value();
                oa.cref = std::string(cref);
                oa.exp = -1.0;
                oa.isConst = false;
            }
            return oa;
        }

        /**
         Gets the number of shapes from the xml-file.

         @param the root node "visualization"
         @return the number of shape-nodes
         */
        unsigned int numShapes(rapidxml::xml_node<>* rootNode)
        {
            unsigned int num = 0;

            if (NULL == rootNode->first_node("shape"))
                return num;
            else
                for (rapidxml::xml_node<>* shapeNode = rootNode->first_node("shape"); shapeNode; shapeNode = shapeNode->next_sibling())
                    ++num;

            return num;
        }

    }  // End namespace Util
}  // End namespace OMVIS
