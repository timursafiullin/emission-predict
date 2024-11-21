set(FLTK_ROOT "C:/Program Files (x86)/FLTK/bin")
find_package(FLTK REQUIRED)

add_library(graph_lib_old
    Graph.cpp
    GUI.cpp
    Window.cpp
)

target_include_directories(graph_lib_old PUBLIC
    ${FLTK_INCLUDE_DIR}
)

target_link_directories(graph_lib_old PRIVATE
    ${FLTK_INCLUDE_DIR}/../lib
)

target_link_libraries(graph_lib_old PUBLIC
    ${FLTK_LIBRARIES}
)
