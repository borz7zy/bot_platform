cmake_minimum_required(VERSION 3.10)
project(bot_platform)

# Set standarts
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set libs dir
set(LIBS_DIR ${CMAKE_SOURCE_DIR}/libs)
set(L_THIRD_PARTY_DIR ${LIBS_DIR}/third_party)
set(L_PAWN_DIR ${L_THIRD_PARTY_DIR}/pawn)

# Add Pawn library to compile
set(PAWN_AMX_COMPILE
 ${L_PAWN_DIR}/amx/amx.c
 ${L_PAWN_DIR}/amx/amxaux.c
)
add_library(amx STATIC ${PAWN_AMX_COMPILE})
set_source_files_properties(${PAWN_AMX_COMPILE} PROPERTIES LANGUAGE C)

# Set params to build project
set(PROJECT_SOURCES
 main.cpp
)

include_directories(${L_PAWN_DIR}/linux)
include_directories(${L_PAWN_DIR}/amx)

add_executable(bot_platform ${PROJECT_SOURCES})
target_link_libraries(bot_platform PRIVATE amx)