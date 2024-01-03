project(Inference LANGUAGES C CXX)

add_library(${PROJECT_NAME} IInference.hpp torchInference.hpp torchInference.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

###################################################################################################################
##dependencies
set(TORCH_DIR ${CMAKE_SOURCE_DIR}/deps/torch/)
find_package(Torch REQUIRED PATHS ${TORCH_DIR}/libtorch/share/cmake/Torch)

find_package(OpenCV REQUIRED)

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS} ${TORCH_LIBRARIES})
