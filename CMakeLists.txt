cmake_minimum_required(VERSION 3.10)
project(bot_platform)

# Set standarts
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set libs dir
set(LIBS_DIR ${CMAKE_SOURCE_DIR}/libs)

set(L_LOGPRINT ${LIBS_DIR}/logprint)

set(L_EASY_JSON ${LIBS_DIR}/easy_json)

set(L_THIRD_PARTY_DIR ${LIBS_DIR}/third_party)
set(L_PAWN_DIR ${L_THIRD_PARTY_DIR}/pawn)

# Add logprint library
add_subdirectory(${L_LOGPRINT})

# Add easy_json library
add_subdirectory(${L_EASY_JSON})

# Add Pawn library
set(PAWN_AMX_COMPILE
 ${L_PAWN_DIR}/source/amx/amx.c
 ${L_PAWN_DIR}/source/amx/amxaux.c
)
add_library(amx STATIC ${PAWN_AMX_COMPILE})
set_source_files_properties(${PAWN_AMX_COMPILE} PROPERTIES LANGUAGE C)
target_compile_definitions(amx PRIVATE 
 -DPAWN_CELL_SIZE=32
)

# Set params to build project
set(PROJECT_SOURCES
 main.cpp
 config_controller.cpp
 plugin_manager.cpp
)

# Includes
include_directories(${L_LOGPRINT})

include_directories(${L_EASY_JSON})

include_directories(${L_PAWN_DIR}/source/linux)
include_directories(${L_PAWN_DIR}/source/amx)

add_executable(bot_platform ${PROJECT_SOURCES})
target_link_libraries(bot_platform PRIVATE amx logprint easy_json)