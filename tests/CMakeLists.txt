cmake_minimum_required(VERSION 3.14)
include(FetchContent)

FetchContent_Declare(googletest
			GIT_REPOSITORY https://github.com/google/googletest
			GIT_TAG release-1.11.0)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
	FetchContent_MakeAvailable(googletest)
endif()

add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)


add_executable(test_Vec2 test_Vec2.cpp)
target_link_libraries(test_Vec2 RC gtest_main gmock_main)

add_test(Vec2 test_Vec2)
