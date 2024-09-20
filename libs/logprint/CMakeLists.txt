cmake_minimum_required(VERSION 3.10)
project(logprint)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(LIBRARY_VERSION 1.0)

add_library(logprint STATIC logprint.cpp)

target_include_directories(logprint PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})