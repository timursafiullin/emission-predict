#include "fltk.h"
#include "Objects.h"
#include <initializer_list>

TableHeader::TableHeader(std::initializer_list<std::string> header_names)
{
    for (const auto& name : header_names)
    {
        nameList.push_back(name);
    }
}

std::string TableHeader::operator[](const unsigned int index)
{
    if (index > nameList.size() - 1)
    {
        throw std::runtime_error("Header Names List index out of range");
    }
    return nameList[index];
}

std::vector<std::string> TableHeader::get()
{
    return nameList;
}

void Table::draw_cell(const char context, int R, int C, int X, int Y, int W, int H)
{
    fl_push_clip(X, Y, W, H);
    switch (context)
    {
    case (context_header):
        fl_color(COLORS::DARK_GRAY);
        fl_draw(table_header[C].c_str(), X, Y, W, H, FL_ALIGN_CENTER);
        break;
    default:
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