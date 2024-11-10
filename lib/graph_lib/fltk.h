#ifndef FLTK_GUARD
#define FLTK_GUARD 1

#include <cstdlib>  // for exit(0)
#include <string>

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Window.H>

#include <FL/Enumerations.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Table.H>
#include <FL/Fl_Tile.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Float_Input.H>
#include <FL/fl_draw.H>

namespace COLORS
{
    const auto BLACK = fl_rgb_color(0, 0, 0);
    const auto WHITE = fl_rgb_color(255, 255, 255);
    const auto LIGHT_GRAY = fl_rgb_color(232, 232, 232);
    const auto GRAY = fl_rgb_color(198, 198, 198);
    const auto DARK_GRAY = fl_rgb_color(88, 88, 88);
    const auto BRIGHT_BLUE = fl_rgb_color(7, 118, 245);
};

class Font
{
public:
  enum Font_type
  {
    helvetica = FL_HELVETICA,
    helvetica_bold = FL_HELVETICA_BOLD,
    helvetica_italic = FL_HELVETICA_ITALIC,
    helvetica_bold_italic = FL_HELVETICA_BOLD_ITALIC,
    courier = FL_COURIER,
    courier_bold = FL_COURIER_BOLD,
    courier_italic = FL_COURIER_ITALIC,
    courier_bold_italic = FL_COURIER_BOLD_ITALIC,
    symbol = FL_SYMBOL,
  };

  Font(Font_type ff) : f{ff} {}

  Font(int ff) : f{ff} {}

  int as_int () const { return f; }

private:
  int f;
};

#endif  // #ifndef FLTK_GUARD
