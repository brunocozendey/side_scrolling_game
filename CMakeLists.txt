cmake_minimum_required(VERSION 3.25)
project(side_scrolling_game)

add_executable(side_scrolling_game main.cpp)

#Uncomment_to_use_lib
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

target_link_libraries(${CMAKE_PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
