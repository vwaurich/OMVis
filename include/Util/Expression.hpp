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

/** \file
 *
 *  \copyright TU Dresden. All rights reserved.
 *  \authors Volker Waurich, Martin Flehmig
 *  \date Feb 2016
 */

#ifndef INCLUDE_EXPRESSION_HPP_
#define INCLUDE_EXPRESSION_HPP_

#include "WrapperFMILib.hpp"
#include "Util/Visualize.hpp"

#include <read_matlab4.h>
#include <rapidxml.hpp>

/*! \brief Evaluates the Binary Exp using an fmu.
 *
 * \return the evaluated value
 */
double evaluateBinaryExpFMU(rapidxml::xml_node<>* op, rapidxml::xml_node<>* exp2, rapidxml::xml_node<>* exp1,
                            double time, fmi1_import_t* fmu);

/*! \brief Evaluates the call-expression. Uses a FMU.
 *
 * \param call The root node "visualization"
 * \return the value
 */
double evaluateCallExpFMU(rapidxml::xml_node<>* call, rapidxml::xml_node<>* exp1, double time, fmi1_import_t* fmu);

/*! \brief Gets the numerical value for the given node using a callback function. Therefore, the AST is traversed.Uses a FMU.
 *
 * \param node  The root node "visualization"
 * \return the value
 */
double evaluateExpressionFMU(rapidxml::xml_node<>* node, double time, fmi1_import_t* fmu);

#endif /* INCLUDE_EXPRESSION_HPP_ */
