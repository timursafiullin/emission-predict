cmake_minimum_required(VERSION 3.20)

project(emission_predict LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# enable training mode
set(ENABLE_TRAINING_MODE 0)

# enable compiler optimizations
set(ENABLE_OPTIMIZATION 1)

# mingw gcc generates illegal instruction when compiling evaluation mode with optimization. nobody knows why
if(ENABLE_TRAINING_MODE EQUAL 0)
    set(ENABLE_OPTIMIZATION 0)
else()
    # enable training mode
    add_compile_definitions(TRAIN=1)
endif()

if(ENABLE_OPTIMIZATION EQUAL 1)
    add_compile_options(-O3)
endif()

find_package(OpenMP REQUIRED)

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
    graph_lib_old
    csv
)

target_include_directories(${TARGET} PUBLIC
    ${CMAKE_SOURCE_DIR}/src/dataset
    ${CMAKE_SOURCE_DIR}/src/gui
    ${CMAKE_SOURCE_DIR}/src/Neural_Network
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/lib
)

if(ENABLE_OPTIMIZATION EQUAL 1)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT supported OUTPUT error)

    if(supported)
        message(STATUS "IPO / LTO enabled")
        set_property(TARGET ${TARGET} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    else()
        message(STATUS "IPO / LTO not supported: <${error}>")
    endif()
endif()