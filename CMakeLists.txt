#cmake version required
cmake_minimum_required(VERSION 2.8)

project(clownfish C CXX)

enable_testing()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

set(CXX_FLAGS
  -g
  -Wall
  -std=c++11
  )

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
  message(STATUS "current platform: Linux")
  set(CMAKE_CXX_COMPILER "g++")
  add_definitions(-DOS_PLATFORM_LINUX)
elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
  message(STATUS "current platform: Mac OSX (Darwin)")
  set(CMAKE_CXX_COMPILER "clang++")
  add_definitions(-DOS_PLATFORM_OSX_DARWIN)
else ()
  message(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")
endif()


set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR})

string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
message(STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE}})

add_subdirectory(clownfish/base)
