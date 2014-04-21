cmake_minimum_required(VERSION 2.8)
project (steins-gate)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../libnpengine/cmake/Modules")
set(CMAKE_BUILD_TYPE Debug)
find_package(Boost REQUIRED COMPONENTS system)
find_package(SFML 2 REQUIRED COMPONENTS system window graphics)
find_package(GStreamer REQUIRED)
find_package(GLIB REQUIRED)
include_directories(${CMAKE_SOURCE_DIR}/../libnpengine/include
${CMAKE_SOURCE_DIR}/../libnpa/include
${GLIB_INCLUDE_DIRS})

add_definitions(-std=c++11 -O2)

file(
GLOB_RECURSE
STEINSGATE_SRC
${CMAKE_SOURCE_DIR}/src/*
)
link_directories(${CMAKE_SOURCE_DIR}/../libnpengine/)

add_executable(steins-gate ${STEINSGATE_SRC})
target_link_libraries(steins-gate npengine ${Boost_LIBRARIES} ${SFML_LIBRARIES} ${GSTREAMER_LIBRARIES} ${GSTREAMER_APP_LIBRARIES} ${GSTREAMER_VIDEO_LIBRARIES})
set_target_properties(steins-gate PROPERTIES LINKER_LANGUAGE CXX)

# install target
install(TARGETS steins-gate DESTINATION bin)
