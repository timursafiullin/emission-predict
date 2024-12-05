cmake_minimum_required(VERSION 3.20)

project(emission_predict LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#add_compile_options(-O3)

# enable training mode
add_compile_definitions(TRAIN=1)

find_package(OpenMP)

if(OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

set(TARGET "emission_predict")

add_subdirectory(lib)

add_executable(
    ${TARGET}
    src/main.cpp
    src/gui/gui.cpp
    src/dataset/dataset_reader.cpp
    src/Neural_Network/Network.cpp
)

target_link_libraries(${TARGET} PUBLIC
    graph_lib
    csv
)

target_include_directories(${TARGET} PUBLIC
    ${CMAKE_SOURCE_DIR}/src/dataset
    ${CMAKE_SOURCE_DIR}/src/gui
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/lib
)
