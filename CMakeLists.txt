cmake_minimum_required(VERSION 3.20)

project(emission_predict LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra)

set(FLTK_ROOT "C:/Program Files (x86)/FLTK/bin")
find_package(FLTK REQUIRED)

set(TARGET "emission_predict")

add_subdirectory(./lib/)

add_executable(
    ${TARGET}
    ./src/gui/gui.cpp
)

target_link_libraries(${TARGET} PUBLIC 
    ${FLTK_LIBRARIES}
    fltk_jpeg
    fltk_png
    fltk_z
    Window
    Objects
)

target_include_directories(${TARGET} PUBLIC 
    ${CMAKE_SOURCE_DIR}/src
    ${FLTK_INCLUDE_DIR}
    ./lib/
)

target_link_directories(${TARGET} PUBLIC
    ${FLTK_INCLUDE_DIR}/../lib
    ./lib/
)


# add_custom_target(CopyResources ALL
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#    ${PROJECT_SOURCE_DIR}/resources
#    ${CMAKE_BINARY_DIR}/resources
#    COMMENT "Copying resources into runtime output directory"
#)

#add_dependencies(${TARGET} CopyResources)