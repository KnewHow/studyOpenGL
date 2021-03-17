cmake_minimum_required(VERSION 3.0.0)
project(StudyOpenGL VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
find_package(glfw3 REQUIRED)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")

file(GLOB SOURCES
    "${SRC_DIR}/*.h"
    "${SRC_DIR}/*.cpp"
)

# Executable definition and properties
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE "${SRC_DIR}")

# STB
set(STB_DIR "${LIB_DIR}")
target_include_directories(${PROJECT_NAME} PRIVATE "${STB_DIR}")

target_link_libraries(${PROJECT_NAME}
    -lglfw
    -lGL
    )