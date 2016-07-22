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
#include "Util/Util.hpp"

#include <iostream>
#include <stdlib.h>

class TestOMVisualBase : public TestCommon
{
 public:
    OMVIS::Model::OMVisualBase* _omVisualBase;
    static const std::string modelFile;
    static const std::string path;

    TestOMVisualBase()
            : TestCommon(modelFile, path),
              _omVisualBase(nullptr)
    {
        _omVisualBase = new OMVIS::Model::OMVisualBase(constructionPlan->modelFile, constructionPlan->path);
    }

    void SetUp()
    {
    }

    void TearDown()
    {
    }

    ~TestOMVisualBase()
    {
    }
};

const std::string TestOMVisualBase::modelFile = "BouncingBall.fmu";
const std::string TestOMVisualBase::path = "examples/";

/*!
 * Test the constructor for the class OMVisualBase.
 */
TEST_F (TestOMVisualBase, Constructor)
{
    // Not nullptr
    EXPECT_TRUE(_omVisualBase);

    EXPECT_EQ(constructionPlan->modelFile, _omVisualBase->getModelName());
    std::string mpath = OMVIS::Util::makeAbsolutePath(path);
    std::string xmlFile = OMVIS::Util::getXMLFileName(modelFile, mpath);
    EXPECT_EQ(xmlFile, _omVisualBase->getXMLFileName());
    auto a = _omVisualBase->getFirstXMLNode();
    ASSERT_FALSE(_omVisualBase->getFirstXMLNode());
}

/*!
 * Test the initialization of the visual XML file and of the visualization objects.
 */
TEST_F (TestOMVisualBase, initXMLFile)
{
    _omVisualBase->initXMLDoc();
    auto a = _omVisualBase->getFirstXMLNode();
    ASSERT_TRUE(_omVisualBase->getFirstXMLNode());
    int ret = _omVisualBase->initVisObjects();
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(_omVisualBase->_shapes.size(), 1);
}


#endif /* TEST_INCLUDE_TESTOMVISUALBASE_HPP_ */
