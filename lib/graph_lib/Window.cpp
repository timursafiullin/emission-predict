#include "fltk.h"
#include "Objects.h"

int create_window(unsigned int width, unsigned int height, std::string title = "Unnamed")
{
  Fl_Window *window = new Fl_Window(width, height, title.c_str());
  window->color(COLORS::LIGHT_GRAY);
  Table parameters_table(561, 21, 260, 406, 2, 14, COLORS::GRAY, COLORS::DARK_GRAY);
  window->end();
  window->show();
  return Fl::run();
}
