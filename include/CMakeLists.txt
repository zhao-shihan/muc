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

# Add include folders to the library and targets that consume it
# the SYSTEM keyword suppresses warnings for users of the library
#
# By adding this directory as an include directory the user gets a
# namespace effect.
#
# IE:
#   #include "muc/type_traits"
if(PROJECT_IS_TOP_LEVEL)
    target_include_directories(muc INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
else()
    target_include_directories(muc SYSTEM INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
endif()
