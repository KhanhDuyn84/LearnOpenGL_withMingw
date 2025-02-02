cmake_minimum_required(VERSION 3.5)

project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/ExternalLibs/include)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/ExternalLibs/src)

set(LIB_DIR ${CMAKE_SOURCE_DIR}/ExternalLibs/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(LearnOpenGL ${SOURCES} ${SRC_DIR}/glad.c)

target_include_directories(LearnOpenGL PRIVATE ${INCLUDE_DIR})

target_link_libraries(LearnOpenGL PRIVATE ${LIB_DIR}/GLFW/libglfw3dll.a)
target_link_libraries(LearnOpenGL PRIVATE ${LIB_DIR}/SOIL2/libsoil2.a)
