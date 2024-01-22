project(Inference LANGUAGES C CXX)

add_library(${PROJECT_NAME} IInference.hpp torchInference.hpp torchInference.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

###################################################################################################################
##dependencies

find_package(OpenCV REQUIRED)

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS} ${TORCH_LIBRARIES})

###################################################################################################################
##installation
install(FILES 
        ${TORCH_DIR}/libtorch/lib/libc10.so 
        ${TORCH_DIR}/libtorch/lib/libtorch.so
        ${TORCH_DIR}/libtorch/lib/libtorch_cpu.so
        ${TORCH_DIR}/libtorch/lib/libgomp-98b21ff3.so.1
        DESTINATION bin)
