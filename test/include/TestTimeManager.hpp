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

#ifndef TEST_INCLUDE_TESTTIMEMANAGER_HPP_
#define TEST_INCLUDE_TESTTIMEMANAGER_HPP_

#include <Control/TimeManager.hpp>
#include <gtest/gtest.h>

/*! \brief Class to test the class \ref Control::TimeManager.
 *
 * \todo: What else needs to be tested?
 *   - getTimeProgress function
 *
 */
class TestTimeManager : public ::testing::Test
{
 public:
    OMVIS::Control::TimeManager _timeManager;

    TestTimeManager()
            : _timeManager(0.0, 0.0, -1.0, 0.0, 0.1, 0.0, 100.0)
    {
    }

    void SetUp()
    {
    }

    void TearDown()
    {
    }

    ~TestTimeManager()
    {
    }
};

/*!
 * Test fixture to test the construction and initialization of the OMVisManager class.
 */
TEST_F (TestTimeManager, Initialization)
{
    ASSERT_EQ(0.0, _timeManager.getSimTime());
    ASSERT_EQ(0.0, _timeManager.getRealTime());
    ASSERT_EQ(0.0, _timeManager.getVisTime());
    ASSERT_EQ(0.1, _timeManager.getHVisual());
    ASSERT_EQ(0.0, _timeManager.getStartTime());
    ASSERT_EQ(100.0, _timeManager.getEndTime());
    ASSERT_TRUE(_timeManager.isPaused());
    ASSERT_EQ(0.0, _timeManager.getSliderPosition());
}

/*!
 * Test fixture to test the setter methods of the OMVisManager class work correct.
 */
TEST_F (TestTimeManager, Setters)
{
    double value = 23.0;
    _timeManager.setEndTime(value);
    ASSERT_EQ(value, _timeManager.getEndTime());

    value = 2.0;
    _timeManager.setStartTime(value);
    ASSERT_EQ(value, _timeManager.getStartTime());

    value = 5.0;
    _timeManager.setSimTime(value);
    ASSERT_EQ(value, _timeManager.getSimTime());

    value = 4.0;
    _timeManager.setVisTime(value);
    ASSERT_EQ(value, _timeManager.getVisTime());

    // simTime >= visTime
    ASSERT_TRUE(_timeManager.visTimeReady());

    _timeManager.setPause(true);
    ASSERT_TRUE(_timeManager.isPaused());

    _timeManager.setPause(false);
    ASSERT_FALSE(_timeManager.isPaused());

    _timeManager.setSliderRange(3, 77);
    EXPECT_EQ(14, _timeManager.getSliderPosition());

    value = 8.8;
    _timeManager.setRealTimeFactor(value);
    ASSERT_EQ(value, _timeManager.getRealTimeFactor());
}

/*!
 * Test fixture to test that the method updateTick does "something".
 */
TEST_F (TestTimeManager, Timeing)
{
    ASSERT_EQ(0.0, _timeManager.getRealTime());
    for(int i = 0; i< 10; ++i)
        _timeManager.updateTick();
    ASSERT_LT(0.0, _timeManager.getRealTime());
}

#endif /* TEST_INCLUDE_TESTTIMEMANAGER_HPP_ */
