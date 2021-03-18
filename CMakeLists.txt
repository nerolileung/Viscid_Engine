cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Viscid_Engine VERSION 0.01)
enable_language(CXX)

# add the executable
add_executable(Viscid_Engine main.cpp)

configure_file(ViscidConfig.h.in ViscidConfig.h)

# sdl
add_library(SDL2 STATIC IMPORTED)
set_target_properties(SDL2 PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/SDL/libSDL2.a)
target_link_libraries(Viscid_Engine INTERFACE SDL2)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_COMPILER clang++)

target_include_directories(Viscid_Engine PUBLIC
                           "${PROJECT_SOURCE_DIR}"
                           "${PROJECT_BINARY_DIR}"
                           )

install(TARGETS Viscid_Engine DESTINATION bin)