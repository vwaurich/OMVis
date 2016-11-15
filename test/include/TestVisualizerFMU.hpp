/*
 * Copyright (C) 2016, Volker Waurich
 *
 * This file is part of OMVIS.
 *
 * OMVIS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OMVIS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OMVIS.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TEST_INCLUDE_TESTVISUALIZERFMU_HPP_
#define TEST_INCLUDE_TESTVISUALIZERFMU_HPP_

#include "Control/TimeManager.hpp"
#include "TestCommon.hpp"
#include "Model/VisualizerFMU.hpp"


/*! \brief Class to test the class \ref Control::VisualizerFMU.
 *
 */
class TestVisualizerFMU : public TestCommon
{
 public:
    std::shared_ptr<OMVIS::Model::VisualizerFMU> _visualizerFMU;

    TestVisualizerFMU()
            : TestCommon("BouncingBall.fmu", "./examples/"),
              _visualizerFMU(nullptr)
    {
    }

    /*!
     * This method is called before every test.
     */
    void SetUp()
    {
        _visualizerFMU = std::shared_ptr<OMVIS::Model::VisualizerFMU>(
                                 new OMVIS::Model::VisualizerFMU(constructionPlan->modelFile, constructionPlan->path));
    }

    void TearDown()
    {
    }

    ~TestVisualizerFMU()
    {
    }
};

/*!
 * Fixture to test the constructor of the class \ref OMVIS::Model::VisualizerFMU which is called in the SetUp method.
 */
TEST_F (TestVisualizerFMU, Constructor)
{
    // Not nullptr.
    ASSERT_TRUE(_visualizerFMU.get());
    // Knows its type.
    ASSERT_EQ(OMVIS::Model::VisType::FMU, _visualizerFMU->getVisType());
}

/*!
 * This fixture tests the initialization methods of class \ref OMVIS::Model::VisualizerFMU.
 */
TEST_F (TestVisualizerFMU, Initialization)
{    // Initialize.
    ASSERT_TRUE(_visualizerFMU.get());
    _visualizerFMU->initialize();

    // Check FMU.
    const OMVIS::Model::FMUWrapper* fmu = _visualizerFMU->getFMU();
    ASSERT_TRUE(fmu);

    // Check FMUData.
    const OMVIS::Model::FMUData* fmuData = fmu->getFMUData();
    ASSERT_EQ(2, fmuData->_nStates);
    ASSERT_EQ(2, fmuData->_nEventIndicators);

    // Check OMVisManager.
    std::shared_ptr<OMVIS::Control::TimeManager> omvManager = _visualizerFMU->getTimeManager();
    ASSERT_EQ(0.0, omvManager->getStartTime());
    ASSERT_EQ(0.0, omvManager->getVisTime());
    ASSERT_EQ(0.0, omvManager->getSimTime());
    ASSERT_EQ(0.0, omvManager->getRealTime());
    ASSERT_EQ(100.0, omvManager->getEndTime());
    ASSERT_EQ(0.1, omvManager->getHVisual());
    ASSERT_TRUE(omvManager->isPaused());
}

#endif /* TEST_INCLUDE_TESTVISUALIZERFMU_HPP_ */
