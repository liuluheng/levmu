cmake_minimum_required(VERSION 2.6)

project(levmu CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CXX_FLAGS
 -g
 # -DVALGRIND
 -DMUDUO_STD_STRING
 -DCHECK_PTHREAD_RETURN_VALUE
 -D_FILE_OFFSET_BITS=64
 -Wall
 -Wextra
 -Werror
 -Wconversion
 -Wno-unused-parameter
 -Wold-style-cast
 -Woverloaded-virtual
 -Wpointer-arith
 -Wshadow
 -Wwrite-strings
 -march=native
 # -MMD
 -std=c++11
 -rdynamic
 )
if(CMAKE_BUILD_BITS EQUAL 32)
  list(APPEND CXX_FLAGS "-m32")
endif()
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -finline-limit=1000")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

find_package(Boost REQUIRED)
find_path(GMP_INCLUDE_DIR gmp.h)
find_library(GMP_LIBRARY NAMES gmp)

if(GMP_INCLUDE_DIR AND GMP_LIBRARY)
  message(STATUS "found gmp")
endif()

include_directories(${Boost_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR})

include_directories(${PROJECT_SOURCE_DIR}/vendor/muduo ${PROJECT_SOURCE_DIR}/vendor/leveldb/include)

string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
message(STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE}})

add_subdirectory(src)
