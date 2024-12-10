cmake_minimum_required(VERSION 3.19.0)
project(streamer VERSION 0.1.0 LANGUAGES CXX)

include(CTest)
enable_testing()

add_library(${PROJECT_NAME} streamer.cpp)

####################################################################################################
##dependencies
find_package(PkgConfig REQUIRED)
pkg_search_module(gstreamer REQUIRED IMPORTED_TARGET gstreamer-1.0>=1.4)
pkg_search_module(gstreamer-sdp REQUIRED IMPORTED_TARGET gstreamer-sdp-1.0>=1.4)
pkg_search_module(gstreamer-app REQUIRED IMPORTED_TARGET gstreamer-app-1.0>=1.4)
pkg_search_module(gstreamer-video REQUIRED IMPORTED_TARGET gstreamer-video-1.0>=1.4)

target_link_libraries(${PROJECT_NAME}
PkgConfig::gstreamer
PkgConfig::gstreamer-sdp
PkgConfig::gstreamer-app
PkgConfig::gstreamer-video

)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
