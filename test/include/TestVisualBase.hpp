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

#ifndef TEST_INCLUDE_TESTVISUALBASE_HPP_
#define TEST_INCLUDE_TESTVISUALBASE_HPP_

#include <Model/VisualBase.hpp>
#include "TestCommon.hpp"
#include "Util/Util.hpp"

#include <stdlib.h>

class TestOMVisualBase : public TestCommon
{
 public:
    OMVIS::Model::VisualBase* _omVisualBase;
    static const std::string modelFile;
    static const std::string path;

    TestOMVisualBase()
            : TestCommon(modelFile, path),
              _omVisualBase(nullptr)
    {
        _omVisualBase = new OMVIS::Model::VisualBase(constructionPlan->modelFile, constructionPlan->path);
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
 * Test the non default constructor for the class OMVisualBase and check that the members are set accordingly.
 */
TEST_F (TestOMVisualBase, Constructor)
{
    // Not nullptr
    EXPECT_TRUE(_omVisualBase);

    EXPECT_EQ(constructionPlan->modelFile, _omVisualBase->getModelFile());
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
    _omVisualBase->initVisObjects();
    EXPECT_EQ(_omVisualBase->_shapes.size(), 1);

    std::vector<std::string> viVars = _omVisualBase->getVisualizationVariables();
    EXPECT_EQ(1, viVars.size());
    EXPECT_EQ("shape.r[2]", viVars.at(0));
}

/*!
 * Test the method to reset/clear the visual XML file.
 */
TEST_F (TestOMVisualBase, clearXMLDoc)
{
    _omVisualBase->clearXMLDoc();
    auto a = _omVisualBase->getFirstXMLNode();
    ASSERT_EQ(0, _omVisualBase->getFirstXMLNode());
    EXPECT_EQ(_omVisualBase->_shapes.size(), 0);
}


#endif /* TEST_INCLUDE_TESTVISUALBASE_HPP_ */
