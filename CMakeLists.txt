cmake_minimum_required(VERSION 3.12)

project(muc)

include(CMakeDependentOption)

cmake_dependent_option(MUC_ENABLE_UBSAN_IN_DEBUG_BUILD "Enable UndefinedBehaviorSanitizer in debug build" ON BUILD_TESTING OFF)

include(CTest)
if(BUILD_TESTING)
    add_subdirectory(test)
endif()
