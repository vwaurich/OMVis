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

/** \addtogroup Model
 *  \{
 *  \copyright TU Dresden. All rights reserved.
 *  \authors Volker Waurich, Martin Flehmig
 *  \date Feb 2016
 */

#ifndef INCLUDE_DXFILE_HPP
#define INCLUDE_DXFILE_HPP

#include "Util/Visualize.hpp"

#include <osg/Geometry>
#include <string>
#include <QString>
#include <QTextStream>
//#include <qfile.h>

namespace OMVIS
{
    namespace Model
    {

		class DXF3dFace
		{
		public:
			DXF3dFace();
			~DXF3dFace();
			QString fill3dFace(QTextStream* stream);
			void dumpDXF3DFace();
			osg::Vec3f calcNormals();
		public:
			osg::Vec3 vec1;
			osg::Vec3 vec2;
			osg::Vec3 vec3;
			osg::Vec3 vec4;
			std::string layer;
			int colorCode;
			osg::Vec4f color;
		};

        class DXFile : public osg::Geometry
        {
         public:
            /*-----------------------------------------
             * CONSTRUCTORS
             *---------------------------------------*/
			 DXFile(std::string filename);
             ~DXFile() = default;

			//members
		public:
		    std::string fileName;
        };

    }  // namespace Model
}  // namespace OMVIS

#endif /* INCLUDE_DXFILE_HPP */
/**
 * \}
 */
