//
// This is a GUI support code to the chapters 12-16 of the book
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//
#ifndef GUI_GUARD
#define GUI_GUARD

#include "Graph.h"
#include "Window.h"

namespace Graph_lib {

//------------------------------------------------------------------------------

using Address = void*;                        // Address is a synonym for void*
using Callback = void (*)(Address, Address);  // FLTK's required function type for all callbacks

//------------------------------------------------------------------------------

template <class W> W& reference_to (Address pw)
// treat an address as a reference to a W
{
  return *static_cast<W*>(pw);
}

//------------------------------------------------------------------------------

class Widget
{
  // Widget is a handle to an Fl_widget - it is *not* an Fl_widget
  // We try to keep our interface classes at arm's length from FLTK

public:
  Widget(Point xy, int w, int h, const std::string& s, Callback cb) : loc{xy}, width{w}, height{h}, label{s}, do_it{cb}
  {
  }

  Widget(Point xy, int w, int h) : loc{xy}, width{w}, height{h} {};

  virtual void move (int dx, int dy)
  {
    hide();
    pw->position(loc.x += dx, loc.y += dy);
    show();
  }

  virtual void hide () { pw->hide(); }

  virtual void show () { pw->show(); }

  virtual void attach (Window&) = 0;

  void clean_pointer () { delete pw; }

  virtual void draw()
  {
    pw->draw();
  };  // Add virtual draw method

  bool visible () { return pw->visible(); }

  Window& window () { return *own; }

  Point loc;
  int width;
  int height;
  std::string label;
  Callback do_it;

  virtual ~Widget() {}

  Widget& operator= (const Widget&) = delete;  // don't copy Widgets
  Widget(const Widget&) = delete;

protected:
  Window* own;    // every Widget belongs to a Window
  Fl_Widget* pw;  // connection to the FLTK Widget
};

//------------------------------------------------------------------------------

struct Button : Widget
{
  Button(Point xy, int w, int h, const std::string& label, Callback cb) : Widget{xy, w, h, label, cb} {}

  void attach (Window&);

  void set_box_color(Fl_Color color) { box_color = color; };
  void set_box_color2(Fl_Color color) { box_color2 = color; };
  void set_labeltype(Fl_Labeltype label_type) { labeltype = label_type; };
  void set_labelsize(unsigned int size) { labelsize = size; };
  void set_box_type(Fl_Boxtype type) { boxtype = type; };
  void set_font(Fl_Font font) { labelfont = font; };

private:
  Fl_Color box_color = FL_WHITE;
  Fl_Color box_color2 = COLORS::LIGHT_GRAY;
  Fl_Labeltype labeltype = FL_NORMAL_LABEL;
  unsigned int labelsize = 13;
  Fl_Boxtype boxtype = FL_FLAT_BOX;
  Fl_Font labelfont = FL_HELVETICA;
};

//------------------------------------------------------------------------------

struct In_box : Widget
{
  In_box(Point xy, int w, int h, const std::string& s) : Widget{xy, w, h, s, nullptr} {}

  int get_int ();
  virtual std::string get_string ();
  virtual void set_string (std::string s)
  {
    reinterpret_cast<Fl_Input *>(pw)->value(s.c_str());
  }

  void set_color (Fl_Color c) { pw->color(c); }

  virtual void attach (Window& win);
};

//------------------------------------------------------------------------------

struct Out_box : Widget
{
  Out_box(Point xy, int w, int h, const std::string& s) : Widget{xy, w, h, s, nullptr} {}

  void put (int);
  void put (const std::string&);

  void attach (Window& win);
};

//------------------------------------------------------------------------------
struct Menu : Widget
{
  enum Kind
  {
    horizontal,
    vertical
  };

  Menu(Point xy, int w, int h, Kind kk, const std::string& label) : Widget{xy, w, h, label, nullptr}, k{kk}, offset{0}
  {
  }

  Vector_ref<Button> selection;
  Kind k;
  int offset;

  int attach (Button& b);  // Menu does not delete &b
  int attach (Button* p);  // Menu deletes p

  void show ()  // show all buttons
  {
    for (int i = 0; i < selection.size(); ++i)
      selection[i].show();
  }

  void hide ()  // hide all buttons
  {
    for (int i = 0; i < selection.size(); ++i)
      selection[i].hide();
  }

  void move (int dx, int dy)  // move all buttons
  {
    for (int i = 0; i < selection.size(); ++i)
      selection[i].move(dx, dy);
  }

  void attach (Window& win)  // attach all buttons
  {
    for (int i = 0; i < selection.size(); ++i)
      win.attach(selection[i]);
    own = &win;
  }

  void draw() override;
};

//------------------------------------------------------------------------------

class DropdownList : public Fl_Menu_Button
{
public:
  DropdownList(int x, int y, int w, int h, const char* label = 0, Callback callback = 0, int flags = 0);
};

}  // namespace Graph_lib

#endif  // GUI_GUARD
