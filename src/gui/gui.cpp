#include "gui.h"
#include <exception>


void ProgramWindow::draw()
{
  Fl_Window::draw();

  Table* parameters_table = new Table
  (
    table_x, table_y,
    table_width, table_height,
    table_cols, table_rows,
    COLORS::GRAY, COLORS::DARK_GRAY
  );

  parameters_table->set_label(labels_list);

}


ProgramWindow* create_window(unsigned int width, unsigned int height, std::string title, Fl_Color background_color = FL_WHITE)
{
  ProgramWindow* window = new ProgramWindow(width, height, title);
  window->color(background_color);
  window->display();
  return window;
}


int main()
try
{
    ProgramWindow* window = create_window
    (
        window_width, 
        window_height,
        main_window_title,
        COLORS::LIGHT_GRAY
    );
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