cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Viscid_Framework VERSION 0.01)
enable_language(CXX)

configure_file("${PROJECT_SOURCE_DIR}/framework/ViscidConfig.h.in" "${PROJECT_SOURCE_DIR}/framework/ViscidConfig.h")

set (ASSET_PATH "${PROJECT_SOURCE_DIR}/data")

# add files to the executable
add_executable(${CMAKE_PROJECT_NAME} main.cpp ${ASSET_PATH})
file(COPY ${ASSET_PATH} DESTINATION "${PROJECT_BINARY_DIR}/")

add_subdirectory(framework)

# game files
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
HatQuest.h
HatQuest.cpp
Scene.h
Scene.cpp
SceneMainMenu.h
SceneMainMenu.cpp
SceneLevel.h
SceneLevel.cpp
SceneEnd.h
SceneEnd.cpp
Character.h
Character.cpp
Tile.h
Tile.cpp
TilePooler.h
TilePooler.cpp
TilePatterns.h
TilePatterns.cpp
)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_COMPILER clang++)

# linking
set(BUILD_SHARED_LIBS OFF)

set(SDL2_MAC_API_LINK_FLAGS "-lm -liconv  -Wl,-framework,CoreAudio -Wl,-framework,AudioToolbox -Wl,-weak_framework,CoreHaptics -Wl,-weak_framework,GameController -Wl,-framework,ForceFeedback -lobjc -Wl,-framework,CoreVideo -Wl,-framework,Cocoa -Wl,-framework,Carbon -Wl,-framework,IOKit -Wl,-weak_framework,QuartzCore -Wl,-weak_framework,Metal")
set(SDL2_TTF_MAC_FREETYPE_FLAGS "-L/opt/X11/lib -lfreetype")

target_link_directories(${CMAKE_PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/framework/lib/")

if (WIN32)
	target_link_libraries(${CMAKE_PROJECT_NAME} SDL2-static user32 gdi32 winmm imm32 ole32 oleaut32 version uuid advapi32 setupapi shell32)
endif()
if (APPLE)
	target_link_libraries(${CMAKE_PROJECT_NAME} SDL2 ${SDL2_MAC_API_LINK_FLAGS})
endif()

target_link_libraries(${CMAKE_PROJECT_NAME} SDL2main SDL2_image SDL2_mixer)

if (WIN32)
	target_link_libraries(${CMAKE_PROJECT_NAME} SDL2_ttf)
endif()
if(APPLE)
	target_link_libraries(${CMAKE_PROJECT_NAME} SDL2_ttf ${SDL2_TTF_MAC_FREETYPE_FLAGS})
endif()

target_include_directories(${CMAKE_PROJECT_NAME}
PUBLIC "${PROJECT_SOURCE_DIR}"
PUBLIC "${PROJECT_SOURCE_DIR}/framework/"
PUBLIC "${PROJECT_BINARY_DIR}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")