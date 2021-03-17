cmake_minimum_required(VERSION 3.0.0)
project(StudyOpenGL VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 14)
find_package(glfw3 REQUIRED)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB SOURCES
    "${SRC_DIR}/*.h"
    "${SRC_DIR}/*.c"
    "${SRC_DIR}/*.cpp"
)

# Executable definition and properties
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE "${SRC_DIR}")

target_link_libraries(${PROJECT_NAME}
    -lglfw
    -lGL
    -lm
    -lXrandr
    -lXi
    -lX11
    -lpthread
    -ldl
    -lXinerama
    -lXcursor
    )