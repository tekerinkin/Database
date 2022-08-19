cmake_minimum_required(VERSION 3.21)
project(Database)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(Database main.cpp ${SOURCES})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include
        )