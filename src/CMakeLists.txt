set(game_source_files
    main.cpp
    Application.cpp
    Application.h
    MainMenu.cpp
    MainMenu.h
)
#add_library(game ${game_source_files})

add_executable(spacefrontiers ${game_source_files})
target_link_libraries(spacefrontiers ${CORELIBS} engine)
install(TARGETS spacefrontiers DESTINATION bin)
