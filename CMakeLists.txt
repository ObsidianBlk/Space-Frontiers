cmake_minimum_required(VERSION 2.8)
project(SpaceFrontiers)

# This project requires the c++11 standard.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Looking for required libraries
include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMG REQUIRED SDL2_image)
find_package(Boost COMPONENTS system REQUIRED)

# Settings those required libraries in the CORELIBS variable.
set(CORELIBS ${SDL2_LIBRARIES} ${SDL2IMG_LIBRARIES} ${Boost_LIBRARIES})

# Set the include and link directories for all required libraries and source code.
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMG_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${SpaceFrontiers_SOURCE_DIR}/src ${SpaceFrontiers_SOURCE_DIR}/src/engine)
link_directories(${SpaceFrontiers_SOURCE_DIR}/src ${SpaceFrontiers_SOURCE_DIR}/src/engine)
link_directories(${Boost_LIBRARY_DIR})

# Let's look in our subdirectories
add_subdirectory(src/engine)
add_subdirectory(src)
