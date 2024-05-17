cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(muc)

include(CTest)
if(BUILD_TESTING)
    add_subdirectory(test)
endif()
