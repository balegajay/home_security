cmake_minimum_required(VERSION 3.19.0)
project(home_security VERSION 0.1.0 LANGUAGES C CXX)

add_subdirectory(detector/inference)
add_subdirectory(communication)

add_executable(${PROJECT_NAME} main.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

##################################################################################################################
##project versioning
find_program(GIT_EXE "git" REQUIRED)

execute_process(COMMAND ${GIT_EXE} describe
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
OUTPUT_VARIABLE APP_VERSION
RESULT_VARIABLE APP_VERSION_RESULT)

string(STRIP ${APP_VERSION} APP_VERSION)
if(NOT ${APP_VERSION_RESULT} MATCHES "0")
message(FATAL_ERROR "Could not calculate app version: ${APP_VERSION_RESULT} ${APP_VERSION}")
endif()

execute_process(COMMAND ${GIT_EXE} rev-list --all --count
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
OUTPUT_VARIABLE COMMIT_COUNT
RESULT_VARIABLE COMMIT_COUNT_RESULT)

string(STRIP ${COMMIT_COUNT} COMMIT_COUNT)
if(NOT ${COMMIT_COUNT_RESULT} MATCHES "0")
message(FATAL_ERROR "Could not calculate app version: ${COMMIT_COUNT_RESULT} ${COMMIT_COUNT}")
endif()

execute_process(COMMAND ${GIT_EXE} show -s --format=%cd "--date=format:%Y-%m-%d %H:%M:%S %z"
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
OUTPUT_VARIABLE COMMIT_DATE
RESULT_VARIABLE COMMIT_DATE_RESULT)

string(STRIP ${COMMIT_DATE} COMMIT_DATE)
if(NOT ${COMMIT_DATE_RESULT} MATCHES "0")
message(FATAL_ERROR "Could not calculate app version: ${COMMIT_DATE_RESULT} ${COMMIT_DATE}")
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC GLIBCXX_USE_CXX17_ABI=0 APP_VERSION="${APP_VERSION}_${COMMIT_COUNT}_${COMMIT_DATE}")


###################################################################################################################
##dependencies

find_package(OpenCV REQUIRED)
set(TORCH_DIR ${CMAKE_SOURCE_DIR}/deps/torch/)
find_package(Torch REQUIRED PATHS ${TORCH_DIR}/libtorch/share/cmake/Torch)

target_include_directories(${PROJECT_NAME} PUBLIC ${OpenCV_INCLUDE_DIRS} ${TORCH_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PUBLIC ${OpenCV_LIBS} Inference streamer)

####################################################################################################
##test
include(CTest)
enable_testing()


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

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