cmake_minimum_required(VERSION 2.8)
project (steins-gate)

set(CMAKE_BUILD_TYPE Debug)
include_directories(${CMAKE_SOURCE_DIR}/../libnpengine/include)
link_directories(${CMAKE_SOURCE_DIR}/../libnpengine/)

add_executable(steins-gate src/main.c)
target_link_libraries(steins-gate npengine)
