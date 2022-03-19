cmake_minimum_required(VERSION 3.12)

set(PROJECT_NAME "CompilerTest")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "..")
set(CMAKE_CXX_STANDARD 17)

project(${PROJECT_NAME} VERSION 1.0.0)
link_directories(${CMAKE_BINARY_DIR})

include_directories(
    
)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME}
    
)

add_compile_definitions()
