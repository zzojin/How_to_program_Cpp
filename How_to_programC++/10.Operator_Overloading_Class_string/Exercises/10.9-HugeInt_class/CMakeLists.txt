cmake_minimum_required(VERSION 3.13.1)
project(E10.9)

include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})
