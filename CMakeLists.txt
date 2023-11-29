
cmake_minimum_required(VERSION 3.10)

project(tetris)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


file(GLOB SOURCES "src/*.cpp")

add_executable(tetris ${SOURCES})



find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED) # foo
target_link_libraries(tetris PRIVATE GLUT::GLUT OpenGL::GL)

# Debug configuration
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pg")

# Add a custom target for cleaning up extra files
add_custom_target(clean-all
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/CleanAll.cmake
        )