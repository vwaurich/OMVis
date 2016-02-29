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

/**
 OMVis
 expression.h
 Purpose: Evaluates an expression and receives the numerical value from the result file.

 @author Volker Waurich
 @version 0.1
 */

#ifndef INCLUDE_EXPRESSION_HPP_
#define INCLUDE_EXPRESSION_HPP_

#include "WrapperFMILib.hpp"
#include "Util/rapidxml.hpp"
#include "Util/read_matlab4.h"
#include "Visualize.hpp"

/*! \brief Gets the value of a variable at a certain time.
 *
 * @param the root node "visualization"
 * @return the number of shape-nodes
 */
double* omc_get_varValue(ModelicaMatReader* reader, const char* varName, double time);

/*! \brief Evaluates the Binary Exp using a mat-file
*
* @param
* @return the evaluated value
*/
double evaluateBinaryExpMAT(rapidxml::xml_node<>*op, rapidxml::xml_node<> *exp2, rapidxml::xml_node<> *exp1, double time, ModelicaMatReader reader);

/*! \brief Evaluates the Binary Exp using an fmu.
*
* @param
* @return the evaluated value
*/
double evaluateBinaryExpFMU(rapidxml::xml_node<>*op, rapidxml::xml_node<> *exp2, rapidxml::xml_node<> *exp1, double time, fmi1_import_t* fmu);


/*! \brief Evaluates the call-expression. Uses a mat file
 *
 * @param the root node "visualization"
 * @return the value
 */
double evaluateCallExpMAT(rapidxml::xml_node<>* call, rapidxml::xml_node<>* exp1, double time, ModelicaMatReader reader);

/*! \brief Evaluates the call-expression. Uses a FMU.
*
* @param the root node "visualization"
* @return the value
*/
double evaluateCallExpFMU(rapidxml::xml_node<>* call, rapidxml::xml_node<>* exp1, double time, fmi1_import_t* fmu);


/*! \brief Gets the numerical value for the given node usign a callback function. Therefore, the AST is traversed.Uses a Mat-file.
*
* @param the root node "visualization"
* @return the value
*/
double evaluateExpressionMAT(rapidxml::xml_node<>* node, double time, ModelicaMatReader reader);

/*! \brief Gets the numerical value for the given node usign a callback function. Therefore, the AST is traversed.Uses a FMU.
*
* @param the root node "visualization"
* @return the value
*/
double evaluateExpressionFMU(rapidxml::xml_node<>* node, double time, fmi1_import_t* fmu);


#endif /* INCLUDE_EXPRESSION_HPP_ */
