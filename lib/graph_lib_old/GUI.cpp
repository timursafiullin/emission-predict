#include <sstream>
#include <string>

#include "GUI.h"

using namespace Graph_lib;

void Button::attach(Window& win)
{
  pw = new Fl_Button{loc.x, loc.y, width, height, label.c_str()};
  
  pw->color(FL_WHITE);
  pw->color2(COLORS::LIGHT_GRAY);
  pw->labeltype(FL_NORMAL_LABEL);
  pw->labelsize(13);
  pw->box(FL_FLAT_BOX);
  pw->labelfont(FL_HELVETICA);

  pw->callback(reinterpret_cast<Fl_Callback*>(do_it), this);  // pass this widget
  own = &win;
}

int In_box::get_int()
{
  Fl_Input& pi = reference_to<Fl_Input>(pw);
  const char* p = pi.value();
  if (!isdigit(p[0]))
    return -999999;
  return atoi(p);
}

std::string In_box::get_string()
{
  Fl_Input& pi = reference_to<Fl_Input>(pw);
  return std::string(pi.value());
}

void In_box::attach(Window& win)
{
  pw = new Fl_Input{loc.x+5, loc.y+2, width-10, height-2, label.c_str()};
  pw->box(FL_FLAT_BOX);
  pw->align(FL_ALIGN_LEFT);
  pw->set_visible();
  own = &win;
}

void Out_box::put(int i)
{
  Fl_Output& po = reference_to<Fl_Output>(pw);
  std::stringstream ss;
  ss << i;
  po.value(ss.str().c_str());
}

void Out_box::put(const std::string& s) { reference_to<Fl_Output>(pw).value(s.c_str()); }

void Out_box::attach(Window& win)
{
  pw = new Fl_Output{loc.x, loc.y, width, height, label.c_str()};
  own = &win;
}

int Menu::attach(Button& b)
{
  b.width = width;
  b.height = height;

  switch (k)
  {
  case horizontal:
    b.loc = Point{loc.x + offset, loc.y};
    offset += b.width;
    break;
  case vertical:
    b.loc = Point{loc.x, loc.y + offset};
    offset += b.height;
    break;
  }
  selection.push_back(&b);
  return int(selection.size() - 1);
}

void Menu::draw()
{
  for (int i = 0; i < selection.size(); ++i)
    selection[i].draw();
}

int Menu::attach(Button* p)
{
  //  owned.push_back(p);
  return attach(*p);
}

DropdownList::DropdownList(int x, int y, int w, int h, const char* label, Callback callback, int flags) : Fl_Menu_Button(x, y, w, h, label)
{
}