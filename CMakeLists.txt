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
set(SDL2_EXTRA_LINK_FLAGS_STATIC "-lm -liconv  -Wl,-framework,CoreAudio -Wl,-framework,AudioToolbox -Wl,-weak_framework,CoreHaptics -Wl,-weak_framework,GameController -Wl,-framework,ForceFeedback -lobjc -Wl,-framework,CoreVideo -Wl,-framework,Cocoa -Wl,-framework,Carbon -Wl,-framework,IOKit -Wl,-weak_framework,QuartzCore -Wl,-weak_framework,Metal")

add_library(SDL2::SDL2 STATIC IMPORTED)
set_target_properties(SDL2::SDL2 PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/include/SDL2"
  IMPORTED_LINK_INTERFACE_LANGUAGES "C"
  IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}SDL2${CMAKE_STATIC_LIBRARY_SUFFIX}")

if (APPLE)
  set_property(TARGET SDL2::SDL2 PROPERTY INTERFACE_LINK_LIBRARIES "${SDL2_EXTRA_LINK_FLAGS_STATIC}")
endif()

add_library(SDL2::SDL2main STATIC IMPORTED)
set_target_properties(SDL2::SDL2main PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES "C"
  IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}SDL2main${CMAKE_STATIC_LIBRARY_SUFFIX}")

target_link_libraries(Viscid_Engine SDL2::SDL2)
target_link_libraries(Viscid_Engine SDL2::SDL2main)

target_include_directories(Viscid_Engine
PUBLIC "${PROJECT_SOURCE_DIR}"
PUBLIC "${PROJECT_BINARY_DIR}"
PUBLIC "${PROJECT_SOURCE_DIR}/include/")

install(TARGETS Viscid_Engine DESTINATION bin)