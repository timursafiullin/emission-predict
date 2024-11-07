#include "fltk.h"

int create_window(unsigned int width, unsigned int height) {
  Fl_Window *window = new Fl_Window(width, height, "Emission predict");
  window->color(COLORS::LIGHT_GRAY);
  window->end();
  window->show();
  return Fl::run();
}