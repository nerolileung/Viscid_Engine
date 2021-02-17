cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Viscid_Engine VERSION 0.01)
enable_language(CXX)

option(USE_MYMATH "Use CMake tutorial math library" ON)

# conditional add library
if(USE_MYMATH)
    add_subdirectory(MathFunctions)
    list(APPEND EXTRA_LIBS MathFunctions)
endif()

# add the executable
add_executable(Viscid_Engine helloworld.cpp)

target_link_libraries(Viscid_Engine PUBLIC ${EXTRA_LIBS})

configure_file(ViscidConfig.h.in ViscidConfig.h)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_COMPILER clang++)

target_include_directories(Viscid_Engine PUBLIC
                           "${PROJECT_SOURCE_DIR}"
                           "${PROJECT_BINARY_DIR}"
                           )

install(TARGETS Viscid_Engine DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/TutorialConfig.h"
  DESTINATION include
  )