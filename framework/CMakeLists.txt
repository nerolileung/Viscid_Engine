configure_file(ViscidConfig.h.in ViscidConfig.h)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    Game.h
)