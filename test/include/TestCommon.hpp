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

#ifndef TEST_INCLUDE_TESTCOMMON_HPP_
#define TEST_INCLUDE_TESTCOMMON_HPP_

#include <gtest/gtest.h>
#include <Initialization/OMVisFactory.hpp>

#include "Model/OMVisualizerAbstract.hpp"

#include <string>
#include <iostream>
#include <stdlib.h>

/*! Base class for testing of OMVis. */
class TestCommon : public ::testing::Test
{
 public:
    std::string _modelName, _path;
    bool _useFMU;

    TestCommon(std::string modelName, std::string path, bool useFMU)
            : _modelName(modelName),
              _useFMU(useFMU)
    {
        // We need the absolute path to the directory. Otherwise the FMUlibrary can not open the shared objects.
        char fullPath[PATH_MAX];
        realpath(path.c_str(), fullPath);
        _path = std::string(fullPath) + "/";
    }

    ~TestCommon()
    {
    }

    virtual void SetUp()
    {
//        _omVisualizer = _factory.createVisualizationObject(_modelName, _path, _useFMU);
//        _omVisualizer->initialize();
//        std::cout << "hier" << std::endl;
    }

    virtual void TearDown()
    {
        //?_omVisualizer.reset();
    }

    void reset(std::string modelName, std::string path, bool useFMU)
    {
        _modelName = modelName;
        _useFMU = useFMU;
        // We need the absolute path to the directory. Otherwise the FMUlibrary can not open the shared objects.
        char fullPath[PATH_MAX];
        realpath(path.c_str(), fullPath);
        _path = std::string(fullPath) + "/";
    }
};

#endif /* TEST_INCLUDE_TESTCOMMON_HPP_ */
