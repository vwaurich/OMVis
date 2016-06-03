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

#include "Initialization/OMVisFactory.hpp"
#include "Model/OMVisualizerFMU.hpp"
#include "Model/OMVisualizerAbstract.hpp"

#include <gtest/gtest.h>

/*! \brief Class to test the class \ref Control::OMVisFactory.
 *
 *
 */
class TestOMVisFactory : public TestCommon
{
 public:
    OMVIS::Initialization::Factory* _factory;

    TestOMVisFactory()
            : TestCommon("BouncingBall", "./examples/", true),
              _factory(nullptr)
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

/*!
 * Test fixture to test that the factory creates the right "product", i.e., if we
 * visualize a FMU we want a OMVisualizerFMU object to be created and if we
 * visualize a MAT file we want a OMVisualizerMAT object to be created. Default
 * case is to create OMVisualizerAbstract object.
 */
TEST_F (TestOMVisFactory, TestCreateVisualizationObject)
{
    _factory = new OMVIS::Initialization::Factory();
    std::shared_ptr<OMVIS::Model::OMVisualizerAbstract> omvisualizerAbstract(nullptr);

    // FMU.
    omvisualizerAbstract = _factory->createVisualizationObject(plan);
    ASSERT_EQ("fmu", omvisualizerAbstract->getType());

    // MAT.
    reset("pendulum", "./examples/", false);
    omvisualizerAbstract = _factory->createVisualizationObject(plan);
    ASSERT_EQ("mat", omvisualizerAbstract->getType());

    // Abstract -> nullptr
    omvisualizerAbstract = _factory->createVisualizationObject("as", "as/as", false);
    ASSERT_FALSE(omvisualizerAbstract);
}

#endif /* TEST_INCLUDE_TESTOMVISFACTORY_HPP_ */

