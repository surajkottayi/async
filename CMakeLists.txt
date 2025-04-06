cmake_minimum_required(VERSION 3.16)
project(
    "Async"
    VERSION 0.1
    DESCRIPTION "How to use async in your code"
    LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(
    ${PROJECT_NAME}
    ${SRC}
)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
