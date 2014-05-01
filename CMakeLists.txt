cmake_minimum_required(VERSION 2.8)

# add modules
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

project (steins-gate CXX)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")

find_package(Boost REQUIRED COMPONENTS system)
find_package(SFML 2 REQUIRED COMPONENTS system window graphics)
find_package(GStreamer REQUIRED)
find_package(GLIB REQUIRED)
find_package(NPA REQUIRED)
find_package(NPENGINE REQUIRED)

# add libraries
include_directories(
    ${NPA_INCLUDE_DIR}
    ${NPENGINE_INCLUDE_DIR}
    ${GLIB_INCLUDE_DIRS}
)
link_directories(${NPENGINE_LIBRARY_DIR} ${NPA_LIBRARY_DIR})

add_executable(steins-gate
    src/main.cpp
    src/nsbconstants.cpp
    src/phone.cpp
    src/phonemodeaddressbook.cpp
    src/phonemodedefaultoperatable.cpp
    src/phonemodemailmenu.cpp
    src/phonemodesendmailedit.cpp
    src/phonesd.cpp
    src/sginterpreter.cpp
    src/steinsgate.cpp
)
target_link_libraries(steins-gate
    npengine npa
    ${Boost_LIBRARIES}
    ${SFML_LIBRARIES}
    ${GSTREAMER_LIBRARIES} ${GSTREAMER_APP_LIBRARIES} ${GSTREAMER_VIDEO_LIBRARIES}
)

# install target
install(TARGETS steins-gate DESTINATION bin)
