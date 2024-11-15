#include "gui.h"
#include "graph_lib.h"
#include <exception>

int main()
try
{
    ProgrammWindow* window = create_window(window_width, window_height, main_window_title, COLORS::LIGHT_GRAY);
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