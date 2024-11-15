#ifndef WINDOW_H
#define WINDOW_H 1

#include "fltk.h"
#include <string>
#include "Objects.h"

class ProgrammWindow : public Fl_Window
{
public:
  ProgrammWindow(int w, int h, const std::string& title) : Fl_Window(w, h) 
  {
    label(title.c_str());
  }

  virtual ~ProgrammWindow() {}

  void draw() override 
  {
    Fl_Window::draw();
    Table* tbl = new Table(561, 21, 260, 406, 2, 14, TableHeader(std::initializer_list<std::string>{"Parameters", "Values"}), COLORS::GRAY, COLORS::DARK_GRAY);
  }

  void show() 
  {
    Fl_Window::show();
  }

  void hide() 
  {
    Fl_Window::hide();
  }

  void resize(int x, int y, int w, int h) 
  {
    Fl_Window::resize(x, y, w, h);
  }

  void display() 
  {
    show();
    Fl::run();
  }
};

int create_window(unsigned int width, unsigned int height, std::string title, Fl_Color background_color = FL_WHITE) {
  ProgrammWindow* window = new ProgrammWindow(width, height, title);
  window->color(background_color);
  window->display();
  return 0;
}

#endif // WINDOW_H