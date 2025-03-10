cmake_minimum_required(VERSION "3.22")
project("allocator")
include(FetchContent)

FetchContent_Declare(googletest
                     REQUIRED
                     GIT_REPOSITORY https://github.com/google/googletest.git
                     GIT_TAG release-1.11.0)

FetchContent_MakeAvailable(googletest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SOURCES ./test.cxx ./allocator.cxx)
include_directories(${googletest_SOURCE_DIR}/googletest/include)
add_executable(test ${SOURCES})
target_link_libraries(test PRIVATE gtest)
