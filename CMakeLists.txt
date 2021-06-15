cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Viscid_Framework VERSION 0.01)
enable_language(CXX)

configure_file("${PROJECT_SOURCE_DIR}/framework/ViscidConfig.h.in" "${PROJECT_SOURCE_DIR}/framework/ViscidConfig.h")

set (ASSET_PATH "${PROJECT_SOURCE_DIR}/data")

# add files to the executable
add_executable(${CMAKE_PROJECT_NAME} main.cpp ${ASSET_PATH})
if (APPLE)
  file(COPY ${ASSET_PATH} DESTINATION "${PROJECT_BINARY_DIR}/")
endif()
if (WIN32)
  file(COPY ${ASSET_PATH} DESTINATION "${PROJECT_BINARY_DIR}/Debug/")
endif()

add_subdirectory(framework)

# game files
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
HatQuest.h
HatQuest.cpp
Scene.h
Scene.cpp
SceneMainMenu.h
SceneMainMenu.cpp
)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_COMPILER clang++)

# sdl
set(SDL2_EXTRA_LINK_FLAGS_STATIC "-lm -liconv  -Wl,-framework,CoreAudio -Wl,-framework,AudioToolbox -Wl,-weak_framework,CoreHaptics -Wl,-weak_framework,GameController -Wl,-framework,ForceFeedback -lobjc -Wl,-framework,CoreVideo -Wl,-framework,Cocoa -Wl,-framework,Carbon -Wl,-framework,IOKit -Wl,-weak_framework,QuartzCore -Wl,-weak_framework,Metal")

add_library(SDL2::SDL2 STATIC IMPORTED)
set_target_properties(SDL2::SDL2 PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/framework/include/SDL2"
  IMPORTED_LINK_INTERFACE_LANGUAGES "C"
  IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/framework/lib/${CMAKE_STATIC_LIBRARY_PREFIX}SDL2${CMAKE_STATIC_LIBRARY_SUFFIX}")

if (APPLE)
  set_property(TARGET SDL2::SDL2 PROPERTY INTERFACE_LINK_LIBRARIES "${SDL2_EXTRA_LINK_FLAGS_STATIC}")
endif()

add_library(SDL2::SDL2main STATIC IMPORTED)
set_target_properties(SDL2::SDL2main PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES "C"
  IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/framework/lib/${CMAKE_STATIC_LIBRARY_PREFIX}SDL2main${CMAKE_STATIC_LIBRARY_SUFFIX}")

# sdl_image
add_library(SDL2::SDL2image STATIC IMPORTED)
set_target_properties(SDL2::SDL2image PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES "C"
  IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/framework/lib/${CMAKE_STATIC_LIBRARY_PREFIX}SDL2_image${CMAKE_STATIC_LIBRARY_SUFFIX}")

# sdl_mixer
add_library(SDL2::SDL2mixer STATIC IMPORTED)
set_target_properties(SDL2::SDL2mixer PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES "C"
  IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/framework/lib/${CMAKE_STATIC_LIBRARY_PREFIX}SDL2_mixer${CMAKE_STATIC_LIBRARY_SUFFIX}")

# sdl_ttf
add_library(SDL2::SDL2ttf STATIC IMPORTED)
set(SDL_TTF_FREETYPE_FLAG "-L/opt/X11/lib -lfreetype")
if (APPLE)
  set_property(TARGET SDL2::SDL2ttf PROPERTY INTERFACE_LINK_LIBRARIES "${SDL_TTF_FREETYPE_FLAG}")
endif()
set_target_properties(SDL2::SDL2ttf PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES "C"
  IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/framework/lib/${CMAKE_STATIC_LIBRARY_PREFIX}SDL2_ttf${CMAKE_STATIC_LIBRARY_SUFFIX}")

target_link_libraries(${CMAKE_PROJECT_NAME} SDL2::SDL2)
target_link_libraries(${CMAKE_PROJECT_NAME} SDL2::SDL2main)
target_link_libraries(${CMAKE_PROJECT_NAME} SDL2::SDL2image)
target_link_libraries(${CMAKE_PROJECT_NAME} SDL2::SDL2mixer)
target_link_libraries(${CMAKE_PROJECT_NAME} SDL2::SDL2ttf)

if (WIN32)
	file(COPY "${PROJECT_SOURCE_DIR}/framework/lib/dll/" DESTINATION "${PROJECT_BINARY_DIR}/Debug")
endif()

target_include_directories(${CMAKE_PROJECT_NAME}
PUBLIC "${PROJECT_SOURCE_DIR}"
PUBLIC "${PROJECT_SOURCE_DIR}/framework/"
PUBLIC "${PROJECT_SOURCE_DIR}/include/"
PUBLIC "${PROJECT_BINARY_DIR}")