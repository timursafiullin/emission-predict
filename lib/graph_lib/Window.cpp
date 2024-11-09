#include "fltk.h"
#include "Objects.h"

int create_window(unsigned int width, unsigned int height, std::string title) {
  Fl_Double_Window *window = new Fl_Double_Window(width, height, title.c_str());
  window->color(COLORS::LIGHT_GRAY);
  RateTable rate(561, 21, 260, 406);
  rate.color(COLORS::WHITE);
  window->end();
  window->show();
  return Fl::run();
}