project(wasmparser-cpp CXX)
add_library(${PROJECT_NAME} INTERFACE instruction_decoder.h)

target_include_directories(${PROJECT_NAME}
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})