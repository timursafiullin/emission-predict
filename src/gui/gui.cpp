#include "gui.h"
#include "graph_lib.h"
#include <exception>


// void ProgramWindow::draw()
// {
//   Fl_Window::draw();
// 
//   Table* parameters_table = new Table
//   (
//     table_x, table_y,
//     table_width, table_height,
//     table_cols, table_rows,
//     COLORS::LIGHT_GRAY, COLORS::LIGHT_GRAY
//   );
// 
//   parameters_table->set_label(labels_list);
// 
// }


// void run_gui(unsigned int width, unsigned int height, std::string title, Fl_Color background_color = FL_WHITE)
// {
// }


int main()
try
{
  Graph_lib::Window win{window_width, window_height, main_window_title, COLORS::WHITE};
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