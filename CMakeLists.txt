cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Viscid_Engine VERSION 0.01)
enable_language(CXX)

# add the executable
add_executable(Viscid_Engine main.cpp)

configure_file(ViscidConfig.h.in ViscidConfig.h)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_COMPILER clang++)

# sdl
add_library(SDL STATIC IMPORTED)
link_directories(SDL "${PROJECT_SOURCE_DIR}/lib")
if (APPLE)
  set_property(TARGET SDL PROPERTY IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/lib/libSDL2.a")
  target_link_libraries(SDL INTERFACE "${PROJECT_SOURCE_DIR}/lib/libSDL2.a")
elseif (WIN32)
  set_property(TARGET SDL PROPERTY IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/lib/SDL2.lib") 
  target_link_libraries(SDL INTERFACE "${PROJECT_SOURCE_DIR}/lib/SDL2.lib")
endif()

target_link_libraries(Viscid_Engine SDL)

target_include_directories(Viscid_Engine
PUBLIC "${PROJECT_SOURCE_DIR}"
PUBLIC "${PROJECT_BINARY_DIR}"
PUBLIC "${PROJECT_SOURCE_DIR}/includes/")

install(TARGETS Viscid_Engine DESTINATION bin)