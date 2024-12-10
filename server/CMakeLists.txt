cmake_minimum_required(VERSION 3.19.0)
project(server VERSION 0.1.0 LANGUAGES C CXX)

add_executable(${PROJECT_NAME} main.cpp)
##set this to the linker flag so that the exe looks for so files in the current dir
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath='$ORIGIN'")

####################################################################################################
##install
install(TARGETS ${PROJECT_NAME})
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/detector/models/ DESTINATION bin)

###################################################################################################################
##dependencies

# set(TORCH_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps/torch/)
# file(ARCHIVE_EXTRACT INPUT ${TORCH_DIR}/libtorch-cxx11-abi-shared-with-deps-latest.zip DESTINATION ${TORCH_DIR})

# find_package(Torch REQUIRED PATHS ${TORCH_DIR}/libtorch/share/cmake/Torch)

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost REQUIRED) 

target_include_directories(${PROJECT_NAME} PUBLIC ${OpenCV_INCLUDE_DIRS} ${TORCH_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PUBLIC streamer)
target_link_libraries(${PROJECT_NAME} PRIVATE flatbuffers)
####################################################################################################
##sub projects
# add_subdirectory(detector/inference)
add_subdirectory(communication)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

##################################################################################################################
##project versioning
# find_program(GIT_EXE "git" REQUIRED)

# execute_process(COMMAND ${GIT_EXE} describe
# WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
# OUTPUT_VARIABLE APP_VERSION
# RESULT_VARIABLE APP_VERSION_RESULT)

# string(STRIP ${APP_VERSION} APP_VERSION)
# if(NOT ${APP_VERSION_RESULT} MATCHES "0")
# message(FATAL_ERROR "Could not calculate app version: ${APP_VERSION_RESULT} ${APP_VERSION}")
# endif()

# execute_process(COMMAND ${GIT_EXE} rev-list --all --count
# WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
# OUTPUT_VARIABLE COMMIT_COUNT
# RESULT_VARIABLE COMMIT_COUNT_RESULT)

# string(STRIP ${COMMIT_COUNT} COMMIT_COUNT)
# if(NOT ${COMMIT_COUNT_RESULT} MATCHES "0")
# message(FATAL_ERROR "Could not calculate app version: ${COMMIT_COUNT_RESULT} ${COMMIT_COUNT}")
# endif()

# execute_process(COMMAND ${GIT_EXE} show -s --format=%cd "--date=format:%Y-%m-%d %H:%M:%S %z"
# WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
# OUTPUT_VARIABLE COMMIT_DATE
# RESULT_VARIABLE COMMIT_DATE_RESULT)

# string(STRIP ${COMMIT_DATE} COMMIT_DATE)
# if(NOT ${COMMIT_DATE_RESULT} MATCHES "0")
# message(FATAL_ERROR "Could not calculate app version: ${COMMIT_DATE_RESULT} ${COMMIT_DATE}")
# endif()

# target_compile_definitions(${PROJECT_NAME} PUBLIC GLIBCXX_USE_CXX17_ABI=0 APP_VERSION="${APP_VERSION}_${COMMIT_COUNT}_${COMMIT_DATE}")

####################################################################################################
##test
include(CTest)
enable_testing()


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

