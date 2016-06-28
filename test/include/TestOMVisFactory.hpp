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

#ifndef TEST_INCLUDE_TESTOMVISFACTORY_HPP_
#define TEST_INCLUDE_TESTOMVISFACTORY_HPP_

#include "Model/OMVisualizerFMU.hpp"
#include "Model/OMVisualizerAbstract.hpp"

#include <gtest/gtest.h>
#include <Initialization/Factory.hpp>

/*! \brief Class to test the class \ref Control::OMVisFactory.
 *
 * The test fixtures tests that the factory creates the right "product", i.e., if we visualize a
 * FMU we want a OMVisualizerFMU object to be created and if we visualize a MAT file we want a
 * OMVisualizerMAT object to be created. The default case is to create OMVisualizerAbstract object.
 */
class TestOMVisFactory : public TestCommon
{
 public:
    OMVIS::Initialization::Factory* _factory;
    std::shared_ptr<OMVIS::Model::OMVisualizerAbstract> _omvisualizerAbstract;

    TestOMVisFactory()
            : TestCommon("BouncingBall.fmu", "./examples/", true),
              _factory(new OMVIS::Initialization::Factory()),
              _omvisualizerAbstract(nullptr)
    {
    }

    void SetUp()
    {
    }

    void TearDown()
    {
    }

    ~TestOMVisFactory()
    {
    }
};

/*! Test the creation of an OMVisualizerFMU object by the factory. */
TEST_F (TestOMVisFactory, TestCreateOMVisualizerFMU)
{
    // FMU.
    _omvisualizerAbstract = _factory->createVisualizationObject(plan);
    ASSERT_EQ("fmu", _omvisualizerAbstract->getType());
}

/*! Test the creation of an OMVisualizerMAT object by the factory. */
TEST_F (TestOMVisFactory, TestCreateOMVisualizerMAT)
{
    // MAT.
    reset("pendulum_res.mat", "./examples/", false);
    _omvisualizerAbstract = _factory->createVisualizationObject(plan);
    ASSERT_EQ("mat", _omvisualizerAbstract->getType());
}

//TEST_F (TestOMVisFactory, TestCreateOMVisualizerAbstract)
//{
//    // Abstract -> nullptr
//    reset("", "", false);
//    _omvisualizerAbstract = _factory->createVisualizationObject(plan);
//    ASSERT_FALSE(_omvisualizerAbstract);
//}

#endif /* TEST_INCLUDE_TESTOMVISFACTORY_HPP_ */

