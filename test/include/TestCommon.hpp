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

#include "Initialization/OMVisFactory.hpp"

#include <gtest/gtest.h>

#include <string>
#include <cstdlib>
#include <climits>

/*! \brief Base class for testing OMVis.
 *
 * This class encapsulates a model file name and path and provides a "reload"
 * functionality in the sense, that model file and path can be changed. The
 * given (absolute or relative) path is transformed to be absolute. Thus, if a
 * testing class needs a model file name or path, it should inherit from
 * this class.
 *  */
class TestCommon : public ::testing::Test
{
 public:
    /*! Constructs a TestCommon object from the passed model file name and path.
     *
     * The model file name has to be passed without extension, e.g., BouncingBall.
     * The extension is implicitly specified via boolean value useFMU: If useBool
     * is true, we look for MODEL.fmu, otherwise we assume to handle a MAT file
     * named MODEL.mat.
     * The path is made to be absolute.
     *
     * @param modelName Name of the model file without extension
     * @param path Relative or absolute path to the model file.
     * @param useFMU True, if model is a FMU, false if the model is a MAT file.
     */
    TestCommon(std::string modelName, std::string path, bool useFMU)
    {
        // We need the absolute path to the directory. Otherwise the FMUlibrary can not open the shared objects.
        char fullPath[PATH_MAX];
        realpath(path.c_str(), fullPath);
        std::string modelPath = std::string(fullPath) + "/";
        plan.fileName = modelName;
        plan.dirPath = modelPath;
        plan.isFMU = useFMU;
    }

    ~TestCommon()
    {
    }

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

    /*! \brief Resets the member variables to the given values.
     *
     * This serves as first step of a reload functionality.
     */
    void reset(std::string modelName, std::string path, bool useFMU)
    {
        // We need the absolute path to the directory. Otherwise the FMUlibrary can not open the shared objects.
        char fullPath[PATH_MAX];
        realpath(path.c_str(), fullPath);
        std::string modelPath = std::string(fullPath) + "/";
        plan.fileName = modelName;
        plan.dirPath = modelPath;
        plan.isFMU = useFMU;
    }

 protected:
    OMVIS::Initialization::VisualizationConstructionPlan plan;
};

#endif /* TEST_INCLUDE_TESTCOMMON_HPP_ */
