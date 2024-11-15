#include "fltk.h"
#include "Objects.h"
#include <iostream>


void Table::draw_cell(const char context, int X, int Y, int W, int H)
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

void Table::draw_borders(Fl_Color color)
{
    fl_color(color);
    for (int row_border{0}; row_border < rows + 1; ++row_border)
    {
        fl_line(x, y + row_border*cell_h, x + w, y + row_border*cell_h);
    }

    for (int col_border{0}; col_border < cols + 1; ++col_border)
    {
        fl_line(x + col_border*cell_w, y, x + col_border*cell_w, y + h);
    }
}

void Table::draw_borders(Fl_Color inner_color, Fl_Color outer_color)
{
    for (int row_border{0}; row_border < rows + 1; ++row_border)
    {
        fl_color(inner_color);
        if (row_border == 1 || row_border == rows || row_border == 0)
        {
            fl_color(outer_color);
        }
        fl_line(x, y + row_border*cell_h, x + w, y + row_border*cell_h);
    }

    fl_color(outer_color);
    for (int col_border{0}; col_border < cols + 1; ++col_border)
    {
        fl_line(x + col_border*cell_w, y, x + col_border*cell_w, y + h);
    }
}