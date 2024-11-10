#include "gui.h"
#include "graph_lib.h"
#include <exception>

int main()
try
{
    create_window(window_width, window_height, main_window_title);
    //Table parameters_table(561, 21, 260, 406, 2, 14);
    return 0;
}
catch (std::exception& e)
{
    std::cerr << e.what() << std::endl;
}
catch (...)
{
    std::cerr << "Unknown exception." << std::endl;
}