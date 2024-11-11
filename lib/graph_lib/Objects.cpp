#include "fltk.h"
#include "Objects.h"
#include <iostream>

void Table::draw_cell(const char context, int R, int C, int X, int Y, int W, int H) {
    static char s[40];
    sprintf(s, "%d/%d", R, C);
    switch (context)
    {
        case context_header:
            fl_draw_box(FL_FLAT_BOX, X, Y, W, H, COLORS::BRIGHT_BLUE);
            return;
 
        case context_cell:
            fl_draw_box(FL_FLAT_BOX, X, Y, W, H, COLORS::DARK_GRAY);
            fl_color(COLORS::BRIGHT_BLUE);
            fl_draw(s, X, Y, W, H, FL_ALIGN_CENTER);
            return;
 
        default:
            std::cout << C << " " << R << std::endl;
            return;
    }
}