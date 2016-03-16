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
 expression.cpp
 Purpose: Evaluates an expression and recieves the numerical value from the result file.

 @author Volker Waurich
 @version 0.1
 */

#include <iostream>
#include <cmath>
#include "Expression.hpp"

using namespace rapidxml;

double* omc_get_varValue(ModelicaMatReader* reader, const char* varName, double time)
{
    double val = 0.0;
    double* res = &val;
    ModelicaMatVariable_t* var = NULL;
    var = omc_matlab4_find_var(reader, varName);
    if (var == NULL)
        std::cout << "Error: Did not get variable from result file! " << varName << std::endl;
    else
        omc_matlab4_val(res, reader, var, time);

    return res;
}


double evaluateBinaryExpMAT(xml_node<>*op, xml_node<> *exp2, xml_node<> *exp1, double time, ModelicaMatReader reader)
{

	char* opType = op->value();
	double value1 = evaluateExpressionMAT(exp1, time, reader);
	double value2 = evaluateExpressionMAT(exp2, time, reader);
	double value = -1.0;

	if (strcmp("add", opType) == 0)
		value = value1 + value2;
	else if (strcmp("sub", opType) == 0)
		value = value1 - value2;
	else if (strcmp("mul", opType) == 0)
		value = value1 * value2;
	else if (strcmp("div", opType) == 0)
		value = value1 / value2;
	else if (strcmp("pow", opType) == 0)
		value = pow(value1, value2);
	else if (strcmp("greater", opType) == 0)
	{
		if (value1 < value2)
			value = 1.0;
		else
			value = 0.0;
	}
	else
		std::cout << "UNKNOWN BINARY " << opType << std::endl;

	return value;
}


double evaluateBinaryExpFMU(xml_node<>*op, xml_node<> *exp2, xml_node<> *exp1, double time, fmi1_import_t* fmu)
{

	char* opType = op->value();
	double value1 = evaluateExpressionFMU(exp1, time, fmu);
	double value2 = evaluateExpressionFMU(exp2, time, fmu);
	double value = -1.0;

	if (strcmp("add", opType) == 0)
		value = value1 + value2;
	else if (strcmp("sub", opType) == 0)
		value = value1 - value2;
	else if (strcmp("mul", opType) == 0)
		value = value1 * value2;
	else if (strcmp("div", opType) == 0)
		value = value1 / value2;
	else if (strcmp("pow", opType) == 0)
		value = pow(value1, value2);
	else if (strcmp("greater", opType) == 0)
	{
		if (value1 < value2)
			value = 1.0;
		else
			value = 0.0;
	}
	else
		std::cout << "UNKNOWN BINARY " << opType << std::endl;

	return value;
}


double evaluateCallExpMAT(xml_node<>* call, xml_node<>* exp1, double time, ModelicaMatReader reader)
{

	char* callType = call->value();
	double value1 = evaluateExpressionMAT(exp1, time, reader);
	double value = -1.0;

	if (strcmp("sqrt", callType) == 0)
		value = sqrt(value1);
	else if (strcmp("abs", callType) == 0)
		value = std::abs(value1);
	else if (strcmp("noEvent", callType) == 0)
		value = value1;
	else
		std::cout << "UNKNOWN CALL " << callType << std::endl;

	return value;
}

double evaluateCallExpFMU(xml_node<>* call, xml_node<>* exp1, double time, fmi1_import_t* fmu)
{

	char* callType = call->value();
	double value1 = evaluateExpressionFMU(exp1, time, fmu);
	double value = -1.0;

	if (strcmp("sqrt", callType) == 0)
		value = sqrt(value1);
	else if (strcmp("abs", callType) == 0)
		value = std::abs(value1);
	else if (strcmp("noEvent", callType) == 0)
		value = value1;
	else
		std::cout << "UNKNOWN CALL " << callType << std::endl;

	return value;
}


double evaluateExpressionMAT(xml_node<>* node, double time, ModelicaMatReader reader)
{
	double value, condValue;
	char* expType = node->name();
	char* cref;
	xml_node<>* exp1;
	xml_node<>* exp2;
	xml_node<>* op;
	xml_node<>* cond;

	//its a const exp.
	if (strcmp("exp", expType) == 0)
	{
		value = atof(node->value());
		//std::cout<<"ITS AN EXP1 "<< value <<std::endl;
	}
	//its a cref to be looked up in the results
	else if (strcmp("cref", expType) == 0)
	{
		cref = node->value();
		value = *omc_get_varValue(&reader, cref, time);
	}

	//its a call
	else if (strcmp("call", expType) == 0)
	{
		op = node->first_node();
		exp1 = op->next_sibling();
		value = evaluateCallExpMAT(op, exp1, time, reader);
		//std::cout<<"ITS A CALL "<< value <<std::endl;
	}

	//its a unary operator (-1)
	else if (strcmp("unary", expType) == 0)
	{
		exp1 = node->first_node("op")->next_sibling();
		value = evaluateExpressionMAT(exp1, time, reader);
		if (strcmp(node->first_node("op")->value(), "uminus") == 0)
			value = -1.0 * value;
		//std::cout<<"ITS A UNARY "<< value <<std::endl;
	}
	//its a binary operator
	else if (strcmp("binary", expType) == 0)
	{
		exp1 = node->first_node();
		op = node->first_node("op");
		exp2 = op->next_sibling();
		value = evaluateBinaryExpMAT(op, exp2, exp1, time, reader);
		//std::cout<<"ITS A BINARY "<< value <<std::endl;
	}
	//its an if exp
	else if (strcmp("ifexp", expType) == 0)
	{
		cond = node->first_node();
		condValue = evaluateExpressionMAT(cond, time, reader);

		if (condValue == 1.0)
			value = evaluateExpressionMAT(cond->next_sibling(), time, reader);
		else
			value = evaluateExpressionMAT(cond->next_sibling()->next_sibling(), time, reader);
		//std::cout<<"ITS A IFEXP "<< value <<std::endl;
	}
	//its a relation
	else if (strcmp("relation", expType) == 0)
	{
		exp1 = node->first_node();
		op = node->first_node("op");
		exp2 = op->next_sibling();
		value = evaluateBinaryExpMAT(op, exp2, exp1, time, reader);
		//std::cout<<"ITS A RELATION "<< value <<std::endl;
	}
	//just continue
	else if (strcmp("cond", expType) == 0 || strcmp("then", expType) == 0 || strcmp("else", expType) == 0)
	{
		//std::cout<<"SOMETHING1 "<< node->value() <<std::endl;
		value = evaluateExpressionMAT(node->first_node(), time, reader);
	}
	//just continue
	else if (strcmp("exp1", expType) == 0 || strcmp("exp2", expType) == 0 || strcmp("expLst", expType) == 0)
	{
		value = evaluateExpressionMAT(node->first_node(), time, reader);
		//std::cout<<"SOMETHING2 "<< value <<std::endl;
	}
	else
	{
		std::cout << "THE EXPRESSION IS UNKNOWN :" << expType << std::endl;
		value = -1.0;
	}
	return value;
}

	double evaluateExpressionFMU(xml_node<>* node, double time, fmi1_import_t* fmu)
	{
		double value, condValue;
		char* expType = node->name();
		char* cref;
		xml_node<>* exp1;
		xml_node<>* exp2;
		xml_node<>* op;
		xml_node<>* cond;

		//its a const exp.
		if (strcmp("exp", expType) == 0)
		{
			value = atof(node->value());
			//std::cout<<"ITS AN EXP1 "<< value <<std::endl;
		}
		//its a cref to be looked up in the results
		else if (strcmp("cref", expType) == 0)
		{
			cref = node->value();
			fmi1_import_variable_t* var = fmi1_import_get_variable_by_name(fmu, cref);
			const fmi1_value_reference_t vr = fmi1_import_get_variable_vr(var);
			fmi1_import_get_real(fmu, &vr, 1, &value); 
		}

		//its a call
		else if (strcmp("call", expType) == 0)
		{
			op = node->first_node();
			exp1 = op->next_sibling();
			value = evaluateCallExpFMU(op, exp1, time, fmu);
			//std::cout<<"ITS A CALL "<< value <<std::endl;
		}

		//its a unary operator (-1)
		else if (strcmp("unary", expType) == 0)
		{
			exp1 = node->first_node("op")->next_sibling();
			value = evaluateExpressionFMU(exp1, time, fmu);
			if (strcmp(node->first_node("op")->value(), "uminus") == 0)
				value = -1.0 * value;
			//std::cout<<"ITS A UNARY "<< value <<std::endl;
		}
		//its a binary operator
		else if (strcmp("binary", expType) == 0)
		{
			exp1 = node->first_node();
			op = node->first_node("op");
			exp2 = op->next_sibling();
			value = evaluateBinaryExpFMU(op, exp2, exp1, time, fmu);
			//std::cout<<"ITS A BINARY "<< value <<std::endl;
		}
		//its an if exp
		else if (strcmp("ifexp", expType) == 0)
		{
			cond = node->first_node();
			condValue = evaluateExpressionFMU(cond, time, fmu);

			if (condValue == 1.0)
				value = evaluateExpressionFMU(cond->next_sibling(), time, fmu);
			else
				value = evaluateExpressionFMU(cond->next_sibling()->next_sibling(), time, fmu);
			//std::cout<<"ITS A IFEXP "<< value <<std::endl;
		}
		//its a relation
		else if (strcmp("relation", expType) == 0)
		{
			exp1 = node->first_node();
			op = node->first_node("op");
			exp2 = op->next_sibling();
			value = evaluateBinaryExpFMU(op, exp2, exp1, time, fmu);
			//std::cout<<"ITS A RELATION "<< value <<std::endl;
		}
		//just continue
		else if (strcmp("cond", expType) == 0 || strcmp("then", expType) == 0 || strcmp("else", expType) == 0)
		{
			//std::cout<<"SOMETHING1 "<< node->value() <<std::endl;
			value = evaluateExpressionFMU(node->first_node(), time, fmu);
		}
		//just continue
		else if (strcmp("exp1", expType) == 0 || strcmp("exp2", expType) == 0 || strcmp("expLst", expType) == 0)
		{
			value = evaluateExpressionFMU(node->first_node(), time, fmu);
			//std::cout<<"SOMETHING2 "<< value <<std::endl;
		}
		else
		{
			std::cout << "THE EXPRESSION IS UNKNOWN :" << expType << std::endl;
			value = -1.0;
		}

	return value;
}