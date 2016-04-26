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

#ifndef TEST_INCLUDE_TESTOMVISUALBASE_HPP_
#define TEST_INCLUDE_TESTOMVISUALBASE_HPP_

#include "Model/OMVisualBase.hpp"
#include "TestCommon.hpp"

#include <gtest/gtest.h>

#include <iostream>
#include <stdlib.h>

class TestOMVisualBase : public TestCommon
{
 public:
    OMVIS::Model::OMVisualBase* _omVisualBase;

    TestOMVisualBase()
        : TestCommon("BouncingBall", "./examples/", false),
          _omVisualBase(nullptr)
    {
    }

    void SetUp()
    {
        _omVisualBase = new OMVIS::Model::OMVisualBase(_modelName, _path);
    }

    void TearDown()
    {
    }

    ~TestOMVisualBase()
    {
    }
};

TEST_F (TestOMVisualBase, TestInitialization)
{
    // Not nullptr
    ASSERT_TRUE(_omVisualBase);

    ASSERT_EQ(_modelName, _omVisualBase->_modelName);
    std::string xmlFile = _path + _modelName + "_visual.xml";
    ASSERT_EQ(xmlFile, _omVisualBase->_xmlFileName);
}

#endif /* TEST_INCLUDE_TESTOMVISUALBASE_HPP_ */
