# Copyright (C) 2021-2025  Shihan Zhao
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.14)

project(muc VERSION 0.25.827 LANGUAGES CXX)

add_library(muc INTERFACE)
add_library(muc::muc ALIAS muc)

include(CMakeDependentOption)

# https://cmake.org/cmake/help/latest/variable/PROJECT_IS_TOP_LEVEL.html
string(COMPARE EQUAL ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR} PROJECT_IS_TOP_LEVEL)

option(MUC_INSTALL "Generate and install muc target" ${PROJECT_IS_TOP_LEVEL})
option(MUC_TEST "Build and perform muc tests" ${PROJECT_IS_TOP_LEVEL})
cmake_dependent_option(MUC_ENABLE_UBSAN_IN_DEBUG_BUILD "Enable UndefinedBehaviorSanitizer in debug build" ON MUC_TEST OFF)

# The implementation generally assumes a platform that implements at least C++17 support
target_compile_features(muc INTERFACE "cxx_std_17")

# Setup include directory
add_subdirectory(include)

if (MUC_TEST)
    enable_testing()
    add_subdirectory(test)
endif()

if (MUC_INSTALL)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/muc" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

    set(export_name "mucConfig")
    set(namespace "muc::")
    set(cmake_files_install_dir ${CMAKE_INSTALL_DATADIR}/cmake/muc)

    install(TARGETS muc EXPORT ${export_name} INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install(EXPORT ${export_name} NAMESPACE ${namespace} DESTINATION ${cmake_files_install_dir})
    export(TARGETS muc NAMESPACE ${namespace} FILE ${export_name}.cmake)

    set(muc_config_version "${CMAKE_CURRENT_BINARY_DIR}/mucConfigVersion.cmake")

    write_basic_package_version_file(${muc_config_version} COMPATIBILITY SameMajorVersion ARCH_INDEPENDENT)

    install(FILES ${muc_config_version} DESTINATION ${cmake_files_install_dir})
endif()
