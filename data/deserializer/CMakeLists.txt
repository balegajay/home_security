cmake_minimum_required(VERSION 3.19.0)
project(deserializer VERSION 0.1.0 LANGUAGES CXX)


add_library(${PROJECT_NAME} deserializer.cpp)

####################################################################################################
##dependencies
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                                  $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
                                                  ${Boost_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PUBLIC flatbuffers)