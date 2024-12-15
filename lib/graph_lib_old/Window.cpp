#include "GUI.h"

namespace Graph_lib {

Window::Window(int ww, int hh, const std::string& title, Fl_Color background_color) : Fl_Double_Window{ww, hh, title.c_str()}, w{ww}, h{hh}
{
  this->color(background_color);
  init(); 
}

Window::Window(int ww, int hh, const std::string& title) : Fl_Double_Window{ww, hh, title.c_str()}, w{ww}, h{hh} { init(); }

Window::Window(Point xy, int ww, int hh, const std::string& title)
    : Fl_Double_Window{xy.x, xy.y, ww, hh, title.c_str()}, w{ww}, h{hh}
{
  init();
}

void Window::init()
{
  show();
}

//----------------------------------------------------

void Window::draw()
{
  Fl_Double_Window::draw();
  for (unsigned int i = 0; i < shapes.size(); ++i)
  {
    shapes[i]->draw();
  }

  for (unsigned int i = 0; i < widgets.size(); ++i)
  {
    if (widgets[i]->visible())
      widgets[i]->draw();
  }
}

void Window::attach(Widget& w)
{
  begin();          // FTLK: begin attaching new Fl_Widgets to this window
  w.attach(*this);  // let the Widget create its Fl_Widgets
  end();            // FTLK: stop attaching new Fl_Widgets to this window
  widgets.push_back(&w);
}

void Window::detach(Widget& w)
{
  w.hide();
  for (unsigned int i = widgets.size(); 0 < i; --i)  // guess last attached will be first released
    if (widgets[i - 1] == &w)
    {
      widgets.erase(widgets.begin() + (i - 1));
      w.clean_pointer();
    }
}

void Window::attach(Shape& s) { shapes.push_back(&s); }

void Window::detach(Shape& s)
{
  for (unsigned int i = shapes.size(); 0 < i; --i)  // guess last attached will be first released
    if (shapes[i - 1] == &s)
      shapes.erase(shapes.begin() + (i - 1));  // &shapes[i-1]);
}

void Window::put_on_top(Shape& s)
{
  detach(s);
  attach(s);
}

int gui_main () { return Fl::run(); }

}  // namespace Graph_lib
