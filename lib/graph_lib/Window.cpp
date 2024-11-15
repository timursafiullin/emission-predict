#include "fltk.h"
#include "Objects.h"

int create_window(unsigned int width, unsigned int height, std::string title = "Unnamed")
{
  Fl_Window *window = new Fl_Window(width, height, title.c_str());
  window->color(COLORS::LIGHT_GRAY);
  window->end();
  window->show();
  return Fl::run();
}
