# FindSDL2_ttf.cmake

find_package(SDL2 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2TTF REQUIRED SDL2_ttf)

set(SDL2_TTF_INCLUDE_DIRS ${SDL2TTF_INCLUDE_DIRS})
set(SDL2_TTF_LIBRARIES ${SDL2TTF_LIBRARIES})
set(SDL2_TTF_FOUND TRUE)