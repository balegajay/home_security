cmake_minimum_required(VERSION 3.19.0)
project(home_security VERSION 0.1.0 LANGUAGES C CXX)
include(cmake/fetchFlatBuffers.cmake)

####################################################################################################
##dependencies
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/fbs_generated)

set(flatc_command ${FLATC_DIR} --cpp ${CMAKE_SOURCE_DIR}/fb/message.fbs)

execute_process(
    COMMAND ${flatc_command}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/fbs_generated
    RESULT_VARIABLE result
)
if(result EQUAL 0)
    message("FlatBuffers schema compilation succeeded.")
else()
    message(FATAL_ERROR "FlatBuffers schema compilation failed with result: ${result}")
endif()

####################################################################################################
##install
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)

####################################################################################################
##sub projects
add_subdirectory(server)
add_subdirectory(data)

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