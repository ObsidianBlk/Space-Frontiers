set(game_source_files
    Application.cpp
    Application.h
);
add_library(game ${game_source_files})

add_executable(spacefrontiers main.cpp)
target_link_libraries(spacefrontiers ${CORELIBS} engine game)
install(TARGETS spacefrontiers DESTINATION bin)
