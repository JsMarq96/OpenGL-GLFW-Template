cmake_minimum_required(VERSION 3.13)

SET(CMAKE_CXX_COMPILER g++)
SET(CMAKE_C_COMPILER gcc)

SET(CMAKE_CXX_FLAGS  "-g")

project(GL_GLFW_TEMPLATE)

file(GLOB CPP_SOURCES "src/*.cpp")
file(GLOB CPP_IMGUI_SOURCES "src/imgui/*.cpp")
file(GLOB C_SOURCES "src/*.c")
set(includes_dir "includes/")

add_executable(PROJECT ${CPP_SOURCES} ${CPP_IMGUI_SOURCES} ${C_SOURCES})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("includes/glfw")
target_link_libraries(PROJECT "glfw")

set(gl3w_dir "${includes_dir}/GL")
add_library("gl3w" "${gl3w_dir}/src/gl3w.c")
target_include_directories("gl3w" PRIVATE "${glw_dir}/")
target_include_directories(PROJECT PRIVATE "${gl3w_dir}/")
target_link_libraries(PROJECT "gl3w" "${CMAKE_DL_LIBS}")

set_target_properties(PROJECT PROPERTIES OUTPUT_NAME "template")

include_directories(
  includes
  gl3w_dir
 )
