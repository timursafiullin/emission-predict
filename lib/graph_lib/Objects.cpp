#include "fltk.h"
#include "Objects.h"
#include <iostream>

void Table::draw_cell(const char context, int R, int C, int X, int Y, int W, int H)
{
    fl_push_clip(X, Y, W, H);
    switch (context)
    {
    case (context_header):
        fl_draw_box(FL_FLAT_BOX, X, Y, W, H, COLORS::WHITE);
        break;
    default:
        fl_draw_box(FL_FLAT_BOX, X, Y, W, H, COLORS::WHITE);
        break;
    }
    fl_pop_clip();
}