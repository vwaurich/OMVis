/*
 * TestVisualizationConstructionPlans.hpp
 *
 *  Created on: 20.07.2016
 *      Author: mf
 */

#ifndef TEST_INCLUDE_TESTVISUALIZATIONCONSTRUCTIONPLANS_HPP_
#define TEST_INCLUDE_TESTVISUALIZATIONCONSTRUCTIONPLANS_HPP_

#include "Initialization/VisualizationConstructionPlans.hpp"

#include <gtest/gtest.h>

/*! \file There we test, that the constructor of the class VisualizationConstructionPlan throws the right exceptions.
 */

/*!
 * Test that the constructor of \ref OMVIS::Initialization::VisualizationConstructionPlan throws a exception if the
 * model file name is empty.
 */
TEST (TestVisualizationConstructionPlans, noModelFile)
{
    try {
        OMVIS::Initialization::VisualizationConstructionPlan("", "");
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(),std::string("No model file specified."));
    }
    catch(...) {
        FAIL() << "Expected std::invalid_argument";
    }
}

/*!
 * Test that the constructor of \ref OMVIS::Initialization::VisualizationConstructionPlan throws a exception if the
 * path is empty.
 */
TEST (TestVisualizationConstructionPlans, noPath)
{
    try {
        OMVIS::Initialization::VisualizationConstructionPlan("BouncingBall.fmu", "");
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(),std::string("No path specified."));
    }
    catch(...) {
        FAIL() << "Expected std::invalid_argument";
    }
}

/*!
 * Test that the constructor of \ref OMVIS::Initialization::VisualizationConstructionPlan throws a exception if the
 * visualization type cannot be determined from the model file prefix.
 */
TEST (TestVisualizationConstructionPlans, undefinedVisualizationType)
{
    try {
        OMVIS::Initialization::VisualizationConstructionPlan("BouncingBall", "/home/");
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(),std::string("VisualizationType is NONE."));
    }
    catch(...) {
        FAIL() << "Expected std::invalid_argument";
    }
}

/*!
 * Test that the constructor of \ref OMVIS::Initialization::RemoteVisualizationConstructionPlan throws a exception if
 * the IP address is not valid.
 */
TEST (TestVisualizationConstructionPlans, invalidIPAddress)
{
    try {
        OMVIS::Initialization::RemoteVisualizationConstructionPlan("BouncingBall.fmu", "/home/", "127.0.0", 4444,
                                                                   "/tmp/");
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(),std::string("Invalid IP address."));
    }
    catch(...) {
        FAIL() << "Expected std::invalid_argument";
    }
}

/*!
 * Test that the constructor of \ref OMVIS::Initialization::RemoteVisualizationConstructionPlan throws a exception if
 * the local working directory is empty.
 */
TEST (TestVisualizationConstructionPlans, noWorkingDirectory)
{
    try {
        OMVIS::Initialization::RemoteVisualizationConstructionPlan("BouncingBall.fmu", "/home/", "141.30.73.103",
                                                                   4444, "");
        FAIL() << "Expected std::invalid_argument";
    }
    catch(std::invalid_argument const & err) {
        EXPECT_EQ(err.what(),std::string("No local working directory specified."));
    }
    catch(...) {
        FAIL() << "Expected std::invalid_argument";
    }
}

/*!
 * Test the constructor of \ref OMVIS::Initialization::VisualizationConstructionPlan.
 */
TEST (TestVisualizationConstructionPlans, ConstructVisualizationConstructionPlan)
{
    OMVIS::Initialization::VisualizationConstructionPlan cP("BouncingBall.fmu", "/home/");
    EXPECT_EQ(OMVIS::VisualizationType::FMU, cP.visType);
    EXPECT_EQ("BouncingBall.fmu", cP.modelFile);
}

/*!
 * Test the constructor of \ref OMVIS::Initialization::RemoteVisualizationConstructionPlan.
 */
TEST (TestVisualizationConstructionPlans, ConstructRemoteVisualizationConstructionPlan)
{
    OMVIS::Initialization::RemoteVisualizationConstructionPlan cP("BouncingBall.fmu", "/home/", "141.30.73.103",
                                                                  4444, "/tmp");
    EXPECT_EQ(OMVIS::VisualizationType::FMU, cP.visType);
    EXPECT_EQ("BouncingBall.fmu", cP.modelFile);
    EXPECT_EQ("/tmp", cP.workingDirectory);
    EXPECT_EQ(4444, cP.portNumber);
    EXPECT_EQ("141.30.73.103", cP.ipAddress);
}


#endif /* TEST_INCLUDE_TESTVISUALIZATIONCONSTRUCTIONPLANS_HPP_ */
