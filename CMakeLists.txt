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
add_executable(a-test ${SOURCES_TEST})

target_link_libraries(a-test PRIVATE
  GTest::gtest_main
  allocator
)

enable_testing()

include(GoogleTest)
gtest_discover_tests(a-test)

find_package(Doxygen REQUIRED dot)
if (DOXYGEN_FOUND)
  set(DOXYGEN_GENERATE_HTML YES)
  set(DOXYGEN_GENERATE_MAN NO)
  set(DOXYGEN_GENERATE_LATEX NO)

  set(DOXYGEN_EXCLUDE_PATTERNS _deps)
  set(DOXYGEN_RECURSIVE YES)

  set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${CMAKE_SOURCE_DIR}/readme.md")
  # set(DOXYGEN_OUTPUT_DIRECTORY ${DOC_BUILD_DIR})

  # set(DOXYGEN_PROJECT_LOGO "${CMAKE_SOURCE_DIR}/doc/images/o2_logo.png")
  set(DOXYGEN_GENERATE_TREEVIEW YES)
  set(DOXYGEN_HIDE_UNDOC_RELATIONS NO)
  set(DOXYGEN_HAVE_DOT YES)
  set(DOXYGEN_DOT_NUM_THREADS 1)
  set(DOXYGEN_UML_LOOK YES)
  set(DOXYGEN_UML_LIMIT_NUM_FIELDS 50)
  set(DOXYGEN_TEMPLATE_RELATIONS YES)
  set(DOXYGEN_DOT_IMAGE_FORMAT svg)
  set(DOXYGEN_INTERACTIVE_SVG YES)
  set(DOXYGEN_DOT_GRAPH_MAX_NODES 100)
  set(DOXYGEN_DOT_TRANSPARENT YES)

  doxygen_add_docs( doc
    ${CMAKE_SOURCE_DIR}
    COMMENT "Generating doxygen documentation for ${PROJECT_NAME}"
  )

  add_dependencies(allocator doc)

else()
  message("Doxygen isn't installed.")
  add_custom_target(solution ALL DEPENDS allocator test)
endif()
