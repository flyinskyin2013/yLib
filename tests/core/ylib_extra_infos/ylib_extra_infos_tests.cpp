/*
 * @Author: Sky
 * @Date: 2021-08-26 17:42:10
 * @LastEditors: Sky
 * @LastEditTime: 2021-08-27 16:37:18
 * @Description: 
 */

#include "catch2/catch.hpp"
#include "ylib.hpp"
#include "test_common.hpp"

#include <iostream>

DEFINE_TEST_CASE_FOR_CLASS_INFO(yLibExtraInfos)


TEST_CASE( "Test yCommon basic attributes" , "[yCommon_BasicAttributes]" ){

    SECTION("test GetyLibVersion() ") {

        REQUIRE_THAT( yLib::yLibExtraInfos::GetyLibVersion(), Catch::Equals ( std::string("V") + std::to_string(yLib::yLibExtraInfos::GetyLibBuildVersionMajor()) + "." + \
        std::to_string(yLib::yLibExtraInfos::GetyLibBuildVersionMinor()) + "." + std::to_string(yLib::yLibExtraInfos::GetyLibBuildVersionPatch()) ));
    }

    SECTION("test GetyLibBuildInfo() ") {

        std::cout << yLib::yLibExtraInfos::GetyLibBuildInfo() <<std::endl;
    }

    SECTION("test GetyLibBuildType(void) ") {

        std::cout << yLib::yLibExtraInfos::GetyLibBuildType() <<std::endl;
    }

    SECTION("test GetyLibBuildPlatform(void) ") {

        std::cout << yLib::yLibExtraInfos::GetyLibBuildPlatform() <<std::endl;
    }
    
    SECTION("test GetyLibBuildVersionMajor() ") {

        std::cout << yLib::yLibExtraInfos::GetyLibBuildVersionMajor() <<std::endl;
    }

    SECTION("test GetyLibBuildVersionMinor() ") {

        std::cout << yLib::yLibExtraInfos::GetyLibBuildVersionMinor() <<std::endl;
    }

        SECTION("test GetyLibBuildVersionPatch() ") {

        std::cout << yLib::yLibExtraInfos::GetyLibBuildVersionPatch() <<std::endl;
    }

        SECTION("test GetyLibBuildTimeStamp() ") {

        std::cout << yLib::yLibExtraInfos::GetyLibBuildTimeStamp() <<std::endl;
    }

        SECTION("test GetyLibBuildGitCommitId() ") {

        std::cout << yLib::yLibExtraInfos::GetyLibBuildGitCommitId() <<std::endl;
    }

        SECTION("test GetyLibBuildBuiltinModule() ") {

        std::cout << yLib::yLibExtraInfos::GetyLibBuildBuiltinModule() <<std::endl;
    }
}