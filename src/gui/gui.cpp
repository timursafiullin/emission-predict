#include "gui.h"
#include <exception>

#define GLib Graph_lib

int main()
try
{
  GLib::Window win{window_width, window_height, main_window_title, COLORS::LIGHT_GRAY};
  GLib::Table parameters_table
  {
    table_x, table_y,
    table_width, table_height,
    table_cols, table_rows,
    COLORS::LIGHT_GRAY, COLORS::LIGHT_GRAY
  };
  parameters_table.set_label(labels_list);
  win.attach(parameters_table);
  return Fl::run();
}
catch (std::exception& e)
{
  std::cerr << e.what() << std::endl;
}
catch (...)
{
  std::cerr << "Unknown exception." << std::endl;
}