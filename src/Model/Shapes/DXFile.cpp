/*
 * Copyright (C) 2016, Volker Waurich
 *
 * This file is part of OMVIS.
 *
 * OMVIS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OMVIS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OMVIS.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Model/Shapes/DXFile.hpp"

#include <QFile>

#define _USE_MATH_DEFINES // for C++
#include <cmath>
#include <math.h>

#include <iostream>

namespace OMVIS
{
    namespace Model
    {

        osg::Vec4f getAutoCADRGB(int colorCode)
        {
            osg::Vec4f col;
            switch (colorCode)
            {
                case (0):
                    col = osg::Vec4f(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 1.0);
                    break;
                case (1):
                    col = osg::Vec4f(255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 1.0);
                    break;
                case (2):
                    col = osg::Vec4f(255.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f, 1.0);
                    break;
                case (3):
                    col = osg::Vec4f(0.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f, 1.0);
                    break;
                case (4):
                    col = osg::Vec4f(0.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0);
                    break;
                case (30):
                    col = osg::Vec4f(255.0f / 255.0f, 127.0f / 255.0f, 0.f / 255.0f, 1.0);
                    break;
                case (251):
                    col = osg::Vec4f(80.0f / 255.0f, 80.0f / 255.0f, 80.0f / 255.0f, 1.0);
                    break;
                default:
                    col = osg::Vec4f(0 / 255, 0 / 255, 0 / 255, 1.0);
                    break;
            }
            return col;
        }

        DXF3dFace::DXF3dFace()
                : vec1(),
                  vec2(),
                  vec3(),
                  vec4(),
                  layer(""),
                  colorCode(0),
                  color()
        {
        }

        void DXF3dFace::dumpDXF3DFace()
        {
            /*
             std::cout << "3-DFACE (" << vec1[0] <<", " << vec1[1]<<", "<< vec1[2]<<")"
             <<"(" << vec2[0] <<", " << vec2[1]<<", "<< vec2[2]<<")"
             << "("<< vec3[0] << ", "<< vec3[1]<<", "<< vec3[2] << ")"
             <<"(" << vec4[0] << ", "<< vec4[1]<<", "<< vec4[2]<< ")" <<std::endl;
             */
            std::cout << "3-DFACE " << "colorCode: " << colorCode << "(" << color[0] << ", " << color[1] << ", "
                      << color[2] << ")" << std::endl;
        }

        QString DXF3dFace::fill3dFace(QTextStream* stream)
        {
            QString line = "";
            int done = 0;
            int lineCode = 0;
            while (!done)
            {
                line = stream->readLine();
                if (!line.compare("3DFACE"))
                {
                    done = 1;
                }
                lineCode = line.toInt();

                switch (lineCode)
                {
                    case (8):
                        //layer name
                        layer = stream->readLine().toInt();
                        break;
                    case (62):
                        //color number
                        colorCode = stream->readLine().toInt();
                        color = getAutoCADRGB(colorCode);
                        break;
                    case (10):
                        //first corner x
                        vec1[0] = stream->readLine().toDouble();
                        break;
                    case (20):
                        //first corner y
                        vec1[1] = stream->readLine().toDouble();
                        break;
                    case (30):
                        //first corner z
                        vec1[2] = stream->readLine().toDouble();
                        break;
                    case (11):
                        //second corner x
                        vec2[0] = stream->readLine().toDouble();
                        break;
                    case (21):
                        //second corner y
                        vec2[1] = stream->readLine().toDouble();
                        break;
                    case (31):
                        //second corner z
                        vec2[2] = stream->readLine().toDouble();
                        break;
                    case (12):
                        //third corner x
                        vec3[0] = stream->readLine().toDouble();
                        break;
                    case (22):
                        //third corner y
                        vec3[1] = stream->readLine().toDouble();
                        break;
                    case (32):
                        //third corner z
                        vec3[2] = stream->readLine().toDouble();
                        break;
                    case (13):
                        //fourth corner x
                        vec4[0] = stream->readLine().toDouble();
                        break;
                    case (23):
                        //fourth corner y
                        vec4[1] = stream->readLine().toDouble();
                        break;
                    case (33):
                        //fourth corner z
                        vec4[2] = stream->readLine().toDouble();
                        break;
                    case (70):
                        //invisible edge flag
                        stream->readLine().toInt();
                        break;
                    default:
                        done = 1;
                        break;
                }
            }
            return line;
        }

        osg::Vec3f DXF3dFace::calcNormals()
        {
            osg::Vec3f v1 = osg::Vec3f(vec1[0] - vec2[0], vec1[1] - vec2[1], vec1[2] - vec2[2]);
            osg::Vec3f v2 = osg::Vec3f(vec1[0] - vec3[0], vec1[1] - vec3[1], vec1[2] - vec3[2]);
            osg::Vec3f normal = Util::normalize(Util::cross(Util::normalize(v1), Util::normalize(v2)));
            return normal;
        }

        DXFile::DXFile(std::string filename)
                : osg::Geometry(),
                  fileName(filename)
        {
            // parse dxf file and fill 3dface objects
            QFile* dxfFile = new QFile(QString::fromStdString(filename));
            if (dxfFile->open(QIODevice::ReadOnly))
            {
                QTextStream* in = new QTextStream(dxfFile);
                //count all 3d faces
                int num3dFaces = in->readAll().count(QString("3DFACE"));

                //reset textstream
                in->seek(0);

                // prepare drawing objects
                osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array(num3dFaces * 4);
                osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array(num3dFaces * 4);
                osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array(num3dFaces * 4);

                // fill face objects
                DXF3dFace* faces = new DXF3dFace[num3dFaces];
                QString line = in->readLine();
                int faceIdx = 0;

                int done = 0;
                while (!done)
                {
                    if (!line.compare("SECTION"))
                    {
                        int secID = in->readLine().toInt();
                        //std::cout << "enter section" << secID << std::endl;
                        line = in->readLine();
                    }
                    else if (!line.compare("ENTITIES"))
                    {
                        int entityID = in->readLine().toInt();
                        //std::cout << "enter entity" << entityID << std::endl;
                        line = in->readLine();
                    }
                    else if (!line.compare("3DFACE"))
                    {
                        //std::cout << "fill face entity" << std::endl;

                        //add vertices
                        line = faces[faceIdx].fill3dFace(in);
                        (*vertices)[(faceIdx * 4) + 0] = faces[faceIdx].vec1;
                        (*vertices)[(faceIdx * 4) + 1] = faces[faceIdx].vec2;
                        (*vertices)[(faceIdx * 4) + 2] = faces[faceIdx].vec3;
                        (*vertices)[(faceIdx * 4) + 3] = faces[faceIdx].vec4;
                        //add colors
                        (*colors)[(faceIdx * 4) + 0] = faces[faceIdx].color;
                        (*colors)[(faceIdx * 4) + 1] = faces[faceIdx].color;
                        (*colors)[(faceIdx * 4) + 2] = faces[faceIdx].color;
                        (*colors)[(faceIdx * 4) + 3] = faces[faceIdx].color;
                        //add normals
                        (*normals)[(faceIdx * 4) + 0] = faces[faceIdx].calcNormals();
                        (*normals)[(faceIdx * 4) + 1] = faces[faceIdx].calcNormals();
                        (*normals)[(faceIdx * 4) + 2] = faces[faceIdx].calcNormals();
                        (*normals)[(faceIdx * 4) + 3] = faces[faceIdx].calcNormals();

                        faceIdx = faceIdx + 1;
                    }
                    else if (!line.compare("ENDSEC"))
                    {
                        //std::cout << "close section" << std::endl;
                        line = in->readLine();
                    }
                    else if (!line.compare("EOF"))
                    {
                        done = 1;
                    }
                    else
                    {
                        line = in->readLine();
                    }
                }
                dxfFile->close();

                //add planes
                this->setVertexArray(vertices);
                for (int i = 0; i < num3dFaces; i++)
                {
                    if (faces[i].vec1 == faces[i].vec4)
                    {
                        //std::cout << "its a triangle" << std::endl;
                        //faces[i].dumpDXF3DFace();
                        osg::ref_ptr<osg::DrawElementsUInt> facette = new osg::DrawElementsUInt(
                                osg::PrimitiveSet::TRIANGLES, 3);
                        (*facette)[0] = (i * 4) + 0;
                        (*facette)[1] = (i * 4) + 1;
                        (*facette)[2] = (i * 4) + 2;
                        this->addPrimitiveSet(facette);

                        //normal calculation
                        osg::Vec3f normal = faces[i].calcNormals();

                    }
                    else
                    {
                        //std::cout << "its a quad" << std::endl;
                        osg::ref_ptr<osg::DrawElementsUInt> facette = new osg::DrawElementsUInt(
                                osg::PrimitiveSet::QUADS, 4);
                        (*facette)[0] = (i * 4) + 0;
                        (*facette)[1] = (i * 4) + 1;
                        (*facette)[2] = (i * 4) + 2;
                        (*facette)[3] = (i * 4) + 3;
                        this->addPrimitiveSet(facette);
                    }
                }
                //add normals
                this->setNormalArray(normals);
                this->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
                //add colors
                this->setColorArray(colors);
                this->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
            }
        }

    }  // namespace Model
}  // namespace OMVIS
