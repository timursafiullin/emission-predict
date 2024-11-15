cmake_minimum_required(VERSION 3.20)

project(emission_predict LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#add_compile_options(-Wall -Wextra -pedantic)

set(TARGET "emission_predict")

add_subdirectory(lib)

add_executable(
    ${TARGET}
    src/gui/gui.cpp
)

target_link_libraries(${TARGET} PUBLIC
    graph_lib
)

target_include_directories(${TARGET} PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/lib
)
