# Copyright (c) 2020 Emanuele Bellocchia
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
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

cmake_minimum_required (VERSION 2.8.1)
project (factory_injector CXX)

# Set output folder to ./bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
# Set general include directories
include_directories (${PROJECT_SOURCE_DIR}/src)

#
# Example application
#

# Source files
add_executable (factory_injector_example
                ./example/main.cpp)
# Set include directories
target_include_directories (factory_injector_example PRIVATE ${PROJECT_SOURCE_DIR}/example)
# Set compiler options
#target_compile_options (factory_injector_example PRIVATE -std=c++14)

#
# Unit tests
#

# Source files
add_executable (ut_factory_injector
                ./tests/ut_main.cpp
                ./tests/ut_factory_injector.cpp)
# Set include directories
target_include_directories (ut_factory_injector PRIVATE ${PROJECT_SOURCE_DIR}/test)
# Set compiler options
target_compile_options (ut_factory_injector PRIVATE -O0 -std=c++14)
# Set link libraries
target_link_libraries (ut_factory_injector gtest pthread)
