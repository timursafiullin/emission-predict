#ifndef FLTK_GUARD
#define FLTK_GUARD 1

#include <cstdlib>  // for exit(0)
#include <string>

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Window.H>
#include <FL/Enumerations.H>

namespace COLORS
{
    const auto BLACK = fl_rgb_color(0, 0, 0);
    const auto WHITE = fl_rgb_color(255, 255, 255);
    const auto LIGHT_GRAY = fl_rgb_color(232, 232, 232);
    const auto GRAY = fl_rgb_color(198, 198, 198);
    const auto DARK_GRAY = fl_rgb_color(88, 88, 88);
    const auto BRIGHT_BLUE = fl_rgb_color(7, 118, 245);
};

#endif  // #ifndef FLTK_GUARD
