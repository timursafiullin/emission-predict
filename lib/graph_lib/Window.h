#ifndef WINDOW_H
#define WINDOW_H 1

#include "fltk.h"
#include <string>
#include "Objects.h"

class ProgrammWindow : public Fl_Window
{
public:
  ProgrammWindow(int w, int h, const std::string& title);
  virtual ~ProgrammWindow() {}

  void draw() override;

  void show();
  void hide();
  void resize(int x, int y, int w, int h);
  void display();
};

ProgrammWindow* create_window(unsigned int width, unsigned int height, std::string title, Fl_Color background_color);

#endif // WINDOW_H