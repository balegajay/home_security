include(FetchContent)
FetchContent_Declare(
  flatbuffers
  GIT_REPOSITORY https://github.com/google/flatbuffers.git
  GIT_TAG        v23.5.26
)
FetchContent_MakeAvailable(flatbuffers)
FetchContent_GetProperties(flatbuffers)

set(FLATBUFFERS_BUILD_TESTS OFF)
set(FLATBUFFERS_INSTALL OFF)
set(FLATBUFFERS_STATIC_FLATC OFF)
set(FLATBUFFERS_BUILD_FLATC OFF)
set(FLATBUFFERS_BUILD_FLATHASH OFF)

set(FLATC_DIR ${FETCHCONTENT_BASE_DIR}/flatbuffers-flatc/flatc)

if(NOT EXISTS "${FLATC_DIR}")
    execute_process(
        COMMAND ${CMAKE_COMMAND}
        -DCMAKE_BUILD_TYPE:STRING=Release
        -DFLATBUFFERS_BUILD_TESTS:BOOL=FALSE
        -DFLATBUFFERS_INSTALL:BOOL=FALSE
        -DFLATBUFFERS_STATIC_FLATC:BOOL=TRUE
        -DFLATBUFFERS_BUILD_FLATC:BOOL=TRUE
        -DFLATBUFFERS_BUILD_FLATHASH:BOOL=FALSE
        -DFLATBUFFERS_BUILD_FLATLIB:BOOL=FALSE
        -S${flatbuffers_SOURCE_DIR}
        -B${FETCHCONTENT_BASE_DIR}/flatbuffers-flatc
        -G${CMAKE_GENERATOR}
        RESULT_VARIABLE FLATC_CONFIGURE_STATUS)
    if(FLATC_CONFIGURE_STATUS EQUAL 0)
        message("FlatBuffers configure succeeded.")
    else()
        message(FATAL_ERROR "FlatBuffers configure failed with result: ${FLATC_CONFIGURE_STATUS}")
    endif()

    execute_process(
        COMMAND ${CMAKE_COMMAND}
        --build ${FETCHCONTENT_BASE_DIR}/flatbuffers-flatc
        --parallel
        RESULT_VARIABLE FLATC_BUILD_STATUS
    )

    if(FLATC_BUILD_STATUS EQUAL 0)
        message("FlatBuffers configure succeeded.")
    else()
        message(FATAL_ERROR "FlatBuffers configure failed with result: ${FLATC_BUILD_STATUS}")
    endif()
endif()