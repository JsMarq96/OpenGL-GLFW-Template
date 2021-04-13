cmake_minimum_required(VERSION 3.13)

SET(CMAKE_CXX_COMPILER g++)
SET(CMAKE_C_COMPILER gcc)

project(GL_GLFW_TEMPLATE)

file(GLOB CPP_SOURCES "src/*.cpp")
file(GLOB C_SOURCES "src/*.c")
set(includes_dir "includes/")

add_executable(GL_GLFW_TEMPLATE ${CPP_SOURCES} ${C_SOURCES})

find_package(glfw3 3.3 REQUIRED)
target_link_libraries(GL_GLFW_TEMPLATE glfw)

set(gl3w_dir "${includes_dir}/GL")
add_library("gl3w" "${gl3w_dir}/src/gl3w.c")
target_include_directories("gl3w" PRIVATE "${glw_dir}/")
target_include_directories(GL_GLFW_TEMPLATE PRIVATE "${gl3w_dir}/")
target_link_libraries(GL_GLFW_TEMPLATE "gl3w" "${CMAKE_DL_LIBS}")

include_directories(
  includes
  gl3w_dir
 )
