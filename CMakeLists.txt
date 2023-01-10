cmake_minimum_required(VERSION 3.13)

SET(CMAKE_CXX_FLAGS  "-g")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(GL_GLFW_TEMPLATE)

file(GLOB CPP_SOURCES "src/*.cpp")
file(GLOB CPP_SUBFOLDER_SOURCES "src/**/*.cpp")
file(GLOB C_SOURCES "src/*.c")
file(GLOB C_SUBFOLDER_SOURCES "src/**/*.c")
set(includes_dir "includes/")

add_executable(GL_GLFW_TEMPLATE WIN32 ${CPP_SOURCES} ${CPP_SUBFOLDER_SOURCES} ${C_SOURCES} ${C_SUBFOLDER_SOURCES})

add_subdirectory(glm)
target_link_libraries(GL_GLFW_TEMPLATE glm)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory("glfw")
target_link_libraries(GL_GLFW_TEMPLATE "glfw")

set(gl3w_dir "${includes_dir}/GL")
add_library("gl3w" "${gl3w_dir}/src/gl3w.c")
target_include_directories("gl3w" PRIVATE "${glw_dir}/")
target_include_directories(GL_GLFW_TEMPLATE PRIVATE "${gl3w_dir}/")
target_link_libraries(GL_GLFW_TEMPLATE "gl3w" "${CMAKE_DL_LIBS}")

if( MSVC )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" )
endif()


 if( MSVC )
    set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GL_GLFW_TEMPLATE)
endif()

include_directories(
  includes
  gl3w_dir
 )

set_target_properties(GL_GLFW_TEMPLATE PROPERTIES OUTPUT_NAME "template")

add_custom_command(TARGET GL_GLFW_TEMPLATE POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:GL_GLFW_TEMPLATE>/resources)
