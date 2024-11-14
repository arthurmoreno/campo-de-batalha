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

# Find SDL
find_package(SDL REQUIRED)

# Include Directories
include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${SDL_INCLUDE_DIR}
)

# Source Files (using .cpp files)
file(GLOB SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# Executable Output
add_executable(campo_de_batalha ${SRC_FILES})

# Link Libraries
if(WIN32)
    # Windows-specific libraries
    target_link_libraries(campo_de_batalha ${SDL_LIBRARY} ${SDLMAIN_LIBRARY} mingw32 ws2_32)
else()
    # Linux or other systems
    target_link_libraries(campo_de_batalha ${SDL_LIBRARY} ${SDLMAIN_LIBRARY})
endif()