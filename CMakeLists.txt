# ==============================
#          CMake Configuration
# ==============================

# Minimum CMake version requirement
cmake_minimum_required(VERSION 3.10)

# Project Name and Language
project(CampoDeBatalha CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Compiler Flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")

# Find SDL2
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Include Directories
include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
)

# Source Files (using .cpp files)
file(GLOB SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# Executable Output
add_executable(campo_de_batalha ${SRC_FILES})

# Link Libraries
if(WIN32)
    # Windows-specific libraries
    target_link_libraries(
        campo_de_batalha
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        SDL2_ttf
        mingw32
        ws2_32
    )
else()
    # Linux or other systems
    target_link_libraries(
        campo_de_batalha
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        SDL2_ttf
    )

endif()