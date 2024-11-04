# ==============================
#          CMake Configuration
# ==============================

# Minimum CMake version requirement
cmake_minimum_required(VERSION 3.10)

# Project Name and Language
project(CampoDeBatalha CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Compiler Flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")

# Include Directories
include_directories(
    ${PROJECT_SOURCE_DIR}/src
    C:/msys64/ucrt64/include/SDL
)

# Library Directories
link_directories(
    C:/msys64/ucrt64/lib
)

# Source Files (using .cpp files)
file(GLOB SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# Executable Output
add_executable(campo_de_batalha ${SRC_FILES})

# Link Libraries
target_link_libraries(campo_de_batalha mingw32 SDLmain SDL ws2_32)