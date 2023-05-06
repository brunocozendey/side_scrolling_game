cmake_minimum_required(VERSION 3.25)
project(side_scrolling_game)

set(CMAKE_CXX_STANDARD 17)

add_executable(side_scrolling_game main.cpp)

#Uncomment_to_use_lib
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

target_link_libraries(${CMAKE_PROJECT_NAME} glut GLU GL m )