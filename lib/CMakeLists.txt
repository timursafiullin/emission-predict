set(FLTK_ROOT "C:/Program Files (x86)/FLTK/bin")
find_package(FLTK REQUIRED)

add_library(Window
    ./Window.cpp
)

add_library(Objects
    ./Objects.cpp
)

target_include_directories(Objects PUBLIC
    ${FLTK_INCLUDE_DIR}
)

target_include_directories(Window PUBLIC
    ${FLTK_INCLUDE_DIR}
)