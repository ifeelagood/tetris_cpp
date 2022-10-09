cmake_minimum_required(VERSION 3.10)
project(tetris)

# set (CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -ltcmalloc -g -pg -O3")


# find packages
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(X11 REQUIRED)
# find_package(GLEW REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(CMAKE_COMPILER /usr/bin/clang++)

file(GLOB SRC src/*.cpp)
include_directories(include)
include_directories(${GLM_INCLUDE_DIRS})

add_library(GLAD include/glad/gl.h include/glad/gl.c)

add_executable(tetris ${SRC})
target_link_libraries(tetris GLAD)
target_link_libraries(tetris glfw3)
target_link_libraries(tetris OpenGL::GL OpenGL::GLU)
target_link_libraries(tetris ${X11_LIBRARIES})

add_custom_target(copy-resource-files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/res
)
