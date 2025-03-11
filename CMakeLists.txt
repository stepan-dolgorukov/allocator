cmake_minimum_required(VERSION "3.22")
project("allocator" LANGUAGES CXX)
include(FetchContent)

FetchContent_Declare(googletest
                     REQUIRED
                     GIT_REPOSITORY https://github.com/google/googletest.git
                     GIT_TAG release-1.11.0)

FetchContent_MakeAvailable(googletest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SOURCES_ALLOCATOR ./allocator.cxx)
set(SOURCES_TEST ./test.cxx)
add_library(allocator SHARED ${SOURCES_ALLOCATOR})
add_executable(test ${SOURCES_TEST})
include_directories(${googletest_SOURCE_DIR}/googletest/include)
target_link_libraries(test PRIVATE gtest allocator)
find_package(Doxygen REQUIRED)

if (DOXYGEN_FOUND)
  set(DOXYFILE ../Doxyfile)
  add_custom_target(documentation
                    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE} && cd ./documentation/latex && make)

else()
  message("Doxygen isn't installed.")
endif()

add_custom_target(solution ALL DEPENDS allocator test documentation)
