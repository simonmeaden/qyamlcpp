cmake_minimum_required(VERSION 2.8.2)
cmake_policy(VERSION 2.8.2)
cmake_policy(SET CMP0048 NEW)
cmake_policy(SET CMP0054 NEW)
#cmake_policy(SET CMP0079 NEW)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_INSTALL_PREFIX)
   set(CMAKE_INSTALL_PREFIX /usr/local)
endif()

# The project version number.
set(VERSION_MAJOR   0   CACHE STRING "Project major version number.")
set(VERSION_MINOR   1   CACHE STRING "Project minor version number.")
set(VERSION_PATCH   0   CACHE STRING "Project patch version number.")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

project(QYamlCpp
        VERSION     0.1.0
        DESCRIPTION "A Qt aware addon to Yaml-Cpp"
        LANGUAGES   CXX C
        )

include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
   ${CMAKE_BINARY_DIR}/../../../${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
   ${CMAKE_BINARY_DIR}/../../../${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
   ${CMAKE_BINARY_DIR}/../../../${CMAKE_INSTALL_BINDIR})
set(INCLUDE_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/../include/)

# Make certain the project is set BEFORE find_package
# Only google test stuff as of now.
#list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

#==== Qt 5 core gui and widgets ==================================
include("CMakeLists.qt5.in")
#==== DOxygen ====================================================
#include("CMakeLists.doxy.in")
#====GoogleTest ==================================================
#include("CMakeLists.gtest.in")s
#==== end of standard includes====================================


# yaml-cpp library
find_library(yamlcpp NAMES yaml-cpp)

set(HEADER_FILES
   ${CMAKE_CURRENT_SOURCE_DIR}/include/qyamlcpp/collection.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/qyamlcpp/comment.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/qyamlcpp/emitter.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/qyamlcpp/node.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/qyamlcpp/parse.h
   )
set(EXTRA_FILES
   ${CMAKE_CURRENT_SOURCE_DIR}/include/qyamlcpp/qyamlcpp.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/qyamlcpp/QYamlCpp
   )

add_library(qyamlcpp INTERFACE)
target_sources(qyamlcpp INTERFACE
   "$<BUILD_INTERFACE:${HEADER_FILES};${EXTRA_FILES}>"
   "$<INSTALL_INTERFACE:${HEADER_FILES};${EXTRA_FILES}")
target_include_directories(qyamlcpp INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
target_link_libraries(qyamlcpp INTERFACE yamlcpp Qt5::Core Qt5::Gui)

# Install target
install(DIRECTORY include/qyamlcpp DESTINATION ${INCLUDE_OUTPUT_DIRECTORY})

# other subdirectories
# only add if not inside add_subdirectory()
option(TYPE_SAFE_BUILD_TEST_EXAMPLE "build test and example" OFF)
if(${TYPE_SAFE_BUILD_TEST_EXAMPLE} OR (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR))
    enable_testing()
    add_subdirectory(examples/)
    add_subdirectory(test/)
endif()

option(TYPE_SAFE_BUILD_DOC "generate documentation" OFF)
if(TYPE_SAFE_BUILD_DOC)
    add_subdirectory(doc/)
endif()
