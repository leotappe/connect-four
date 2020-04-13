cmake_minimum_required(VERSION 3.7)

add_compile_options(-Wall -Wextra -pedantic -Werror)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

project(connect-four)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} src)

add_executable(connect4 src/main.cpp src/game.cpp src/gui.cpp src/grid.cpp src/ai.cpp)
string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)
target_link_libraries(connect4 ${SDL2_LIBRARIES})
