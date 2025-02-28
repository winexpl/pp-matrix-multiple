cmake_minimum_required(VERSION 3.28)

project(matrix_multiple)

set(CMAKE_CXX_STANDARD 23)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g ${CMAKE_CXX_FLAGS_DEBUG}")

set (INCLUDE_DIR ../include)

add_executable(main main.cpp)
add_executable(mul multiple.cpp)

target_link_libraries(main PRIVATE ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(mul PRIVATE ${CMAKE_THREAD_LIBS_INIT})

target_include_directories(mul
    PUBLIC
        ${INCLUDE_DIR}
)

target_include_directories(main
    PUBLIC
        ${INCLUDE_DIR}
)

set_target_properties(mul main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ../bin
)