add_executable(spacefrontiers main.cpp)
target_link_libraries(spacefrontiers ${CORELIBS} engine)
install(TARGETS spacefrontiers DESTINATION bin)
