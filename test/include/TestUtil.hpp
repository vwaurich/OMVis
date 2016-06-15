/*
 * TestUtil.hpp
 *
 *  Created on: 14.06.2016
 *      Author: mf
 */

#ifndef TEST_INCLUDE_TESTUTIL_HPP_
#define TEST_INCLUDE_TESTUTIL_HPP_

#include "Util/Util.hpp"

#include <gtest/gtest.h>

#include <string>


/*! \brief This class contains tests for the methods provided in Util/Util.hpp .
 *
 */
class TestUtil : public ::testing::Test
{
 public:
    TestUtil()
            : _modelFile("/home/mf/opt/HPCOM/OMVis.git_eclipse/examples/BouncingBall.fmu")
    {
    }

    ~TestUtil()
    {
    }

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

 protected:
    std::string _modelFile;
};

/*! \brief Test the method OMVIS::Util::getPath which returns the path from a file. */
TEST_F (TestUtil, TestGetPath)
{
    ASSERT_EQ(std::string("/home/mf/opt/HPCOM/OMVis.git_eclipse/examples/"), OMVIS::Util::getPath(_modelFile));
}

/*! \brief Test the method OMVIS::Util::getFileName which returns the name of the model file. */
TEST_F (TestUtil, TestGetFileName)
{
    ASSERT_EQ(std::string("BouncingBall.fmu"), OMVIS::Util::getFileName(_modelFile));
}

/*! \brief Test the method OMVIS::Util::getPath which returns true if the model file is a FMU. */
TEST_F (TestUtil, TestIsFMU)
{
    ASSERT_TRUE(OMVIS::Util::isFMU(_modelFile));
}

/*! \brief Test the method OMVIS::Util::isMAT which returns true if the model file is a MAT file. */
TEST_F (TestUtil, TestIsMAT)
{
    ASSERT_FALSE(OMVIS::Util::isMAT(_modelFile));
}

/*! \brief Test the method OMVIS::Util::isValidIPv4 which checks a string to be a valid IPv4 address. */
TEST_F (TestUtil, TestIsValidIpAddressV4)
{
    ASSERT_TRUE(OMVIS::Util::isValidIPv4("141.30.73.103"));
    ASSERT_FALSE(OMVIS::Util::isValidIPv4("haferdrink"));
}

/*! \brief Test the method OMVIS::Util::isValidIPv6 which checks a string to be a valid IPv6 address. */
TEST_F (TestUtil, TestIsValidIpAddressV6)
{
    ASSERT_TRUE(OMVIS::Util::isValidIPv6("2001:0db8:85a3:08d3:1319:8a2e:0370:7344"));
    ASSERT_FALSE(OMVIS::Util::isValidIPv6("haferdrink"));
}

#endif /* TEST_INCLUDE_TESTUTIL_HPP_ */
