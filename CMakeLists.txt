# ==============================
#          CMake Configuration
# ==============================

# Minimum CMake version requirement
cmake_minimum_required(VERSION 3.10)

# Project Name and Language
project(CampoDeBatalha C)

# Set the C standard (optional)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Compiler Flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g")

# Include Directories
include_directories(
    ${PROJECT_SOURCE_DIR}/src
    C:/msys64/ucrt64/include/SDL
)

# Library Directories
link_directories(
    C:/msys64/ucrt64/lib
)

# Source Files
file(GLOB SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.c")

# Executable Output
add_executable(campo_de_batalha ${SRC_FILES})

# Link Libraries
target_link_libraries(campo_de_batalha mingw32 SDLmain SDL ws2_32)