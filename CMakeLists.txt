cmake_minimum_required(VERSION 2.8)
project (libnpengine)

if (CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=gnu++11)
endif()

set(CMAKE_BUILD_TYPE Debug)
include_directories(${CMAKE_SOURCE_DIR}/../libnpengine/include)
link_directories(${CMAKE_SOURCE_DIR}/../libnpengine/)

add_executable(steins-gate src/main.cpp)
target_link_libraries(steins-gate npengine)
