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

#include <gtest/gtest.h>
#include <TestFactory.hpp>
#include <TestVisualBase.hpp>
#include <TestVisualizerFMU.hpp>

#include "Util/Logger.hpp"
#include "TestUtil.hpp"
#include "TestVisualizationConstructionPlans.hpp"
#include "TestCommon.hpp"
#include "TestTimeManager.hpp"


int main(int argc, char **argv)
{
    OMVIS::Util::LogSettings logSettings;
    logSettings.setAll(OMVIS::Util::LL_ERROR);
    OMVIS::Util::Logger::initialize(logSettings);

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
