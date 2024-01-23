cmake_minimum_required(VERSION 3.19.0)
project(home_security VERSION 0.1.0 LANGUAGES C CXX)

####################################################################################################
##install
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)

####################################################################################################
##sub projects
add_subdirectory(server)

######################################################################################################
##playground

add_executable(play playground.c)
find_package(PkgConfig REQUIRED)
#pkg_check_modules(gtk3 REQUIRED IMPORTED_TARGET gtk+-3.0)
pkg_search_module(gstreamer REQUIRED IMPORTED_TARGET gstreamer-1.0>=1.4)
pkg_search_module(gstreamer-sdp REQUIRED IMPORTED_TARGET gstreamer-sdp-1.0>=1.4)
pkg_search_module(gstreamer-app REQUIRED IMPORTED_TARGET gstreamer-app-1.0>=1.4)
pkg_search_module(gstreamer-video REQUIRED IMPORTED_TARGET gstreamer-video-1.0>=1.4)

target_link_libraries(play
 #   PkgConfig::gtk3
    PkgConfig::gstreamer
    PkgConfig::gstreamer-sdp
    PkgConfig::gstreamer-app
    PkgConfig::gstreamer-video
)