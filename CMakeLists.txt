cmake_minimum_required(VERSION 3.30)
project(DataStructures)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(DataStructures ${SOURCES})
target_include_directories(DataStructures PRIVATE ${CMAKE_SOURCE_DIR}/include)