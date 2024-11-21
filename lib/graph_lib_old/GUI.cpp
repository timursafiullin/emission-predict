#include <sstream>
#include <string>

#include "GUI.h"

using namespace Graph_lib;

// ValueTable::ValueTable(
//     int x, int y, int w, int h,
//     int table_cols,
//     int table_rows,
//     Fl_Color inner_color,
//     Fl_Color outer_color,
//     Fl_Color background_color)
// {
//   
// }

void ValueTable::attach(Window& win)
{
  pw = new Table{loc.x, loc.y, width, height, cols, rows, in_color, out_color, bg_color};
  own = &win;
}

void ValueTable::set_label(Labels& labels)
{
    for (unsigned int i{0}; i < labels.get().size(); ++i)
    {
        fl_color(COLORS::BLACK);
        fl_font(FL_HELVETICA, Fl_Fontsize(cell_h/2.25));
        fl_draw(labels[i].c_str(), loc.x + cell_w * (labels.col + 0.075), loc.y + cell_h * labels.row, cell_w, cell_h, FL_ALIGN_LEFT);
        switch (labels.context)
        {
            case context_column:
                ++labels.row;
                break;
            case context_row:
                ++labels.col;
                break;
        }
    }
}


void ValueTable::set_label(LabelsList& labels_list)
{
    for (Labels& labels : labels_list.get())
        set_label(labels);
}

void Button::attach(Window& win)
{
  pw = new Fl_Button{loc.x, loc.y, width, height, label.c_str()};
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
  pw = new Fl_Input{loc.x, loc.y, width, height, label.c_str()};
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

int Menu::attach(Button* p)
{
  //  owned.push_back(p);
  return attach(*p);
}
