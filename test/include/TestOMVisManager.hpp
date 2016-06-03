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

#ifndef TEST_INCLUDE_TESTOMVISMANAGER_HPP_
#define TEST_INCLUDE_TESTOMVISMANAGER_HPP_

#include "Control/OMVisManager.hpp"

#include <gtest/gtest.h>

/*! \brief Class to test the class \ref Control::OMVisManager.
 *
 * \todo: What else needs to be tested?
 *   - getTimeProgress function
 *
 */
class TestOMVisManager : public ::testing::Test
{
 public:
    OMVIS::Control::OMVisManager _omvManager;

    TestOMVisManager()
            : _omvManager(0.0, 0.0, -1.0, 0.0, 0.1, 0.0, 100.0)
    {
    }

    void SetUp()
    {
    }

    void TearDown()
    {
    }

    ~TestOMVisManager()
    {
    }
};

/*!
 * Test fixture to test the construction and initialization of the OMVisManager class.
 */
TEST_F (TestOMVisManager, TestInitialization)
{
    ASSERT_EQ(0.0, _omvManager.getSimTime());
    ASSERT_EQ(0.0, _omvManager.getRealTime());
    ASSERT_EQ(0.0, _omvManager.getVisTime());
    ASSERT_EQ(0.1, _omvManager.getHVisual());
    ASSERT_EQ(0.0, _omvManager.getStartTime());
    ASSERT_EQ(100.0, _omvManager.getEndTime());
    ASSERT_TRUE(_omvManager.isPaused());
}

/*!
 * Test fixture to test the setter methods of the OMVisManager class.
 */
TEST_F (TestOMVisManager, TestSetters)
{
    double value = -23.12;
    _omvManager.setEndTime(value);
    ASSERT_EQ(value, _omvManager.getEndTime());
    _omvManager.setStartTime(value);
    ASSERT_EQ(value, _omvManager.getStartTime());
    _omvManager.setSimTime(value);
    ASSERT_EQ(value, _omvManager.getSimTime());
    _omvManager.setVisTime(value);
    ASSERT_EQ(value, _omvManager.getVisTime());

    _omvManager.setPause(true);
    ASSERT_TRUE(_omvManager.isPaused());

    _omvManager.setPause(false);
    ASSERT_FALSE(_omvManager.isPaused());
}

#endif /* TEST_INCLUDE_TESTOMVISMANAGER_HPP_ */
