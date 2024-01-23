cmake_minimum_required(VERSION 3.19.0)
project(tcp_server VERSION 0.1.0 LANGUAGES CXX)

include(CTest)
enable_testing()

add_library(${PROJECT_NAME} tcp_server.cpp tcp_server.hpp)

####################################################################################################
##dependencies
find_package(Boost REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})