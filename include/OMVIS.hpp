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

/** \addtogroup OMVIS
 *  \{
 *  \copyright TU Dresden. All rights reserved.
 *  \authors Volker Waurich, Martin Flehmig
 *  \date Feb 2016
 */


#ifndef INCLUDE_OMVIS_HPP_
#define INCLUDE_OMVIS_HPP_

/*! \file OMVIS.hpp
 *
 * Header file that encapsulates all OMVIS headers in order to provide one header to the application.
 */

#include "View/OMVISViewer.hpp"
#include "Model/SimSettingsFMU.hpp"
#include "Model/VisualizerAbstract.hpp"
#include "Model/InfoVisitor.hpp"
#include "Model/UpdateVisitor.hpp"
#include "Control/TimeManager.hpp"
#include "Initialization/CommandLineArgs.hpp"
#include "Initialization/Factory.hpp"
#include "Util/Visualize.hpp"
#include "Util/Logger.hpp"
#include "Util/Util.hpp"

#include <QApplication>

#endif /* INCLUDE_OMVIS_HPP_ */
/**
 * \}
 */
