#ifndef OBJECTS_H
#define OBJECTS_H 1

#include "FL/Fl_Table.H"
#include "FL/Fl_draw.h"
#include "fltk_design.h"
#include <iostream>

constexpr char context_cell = 'C';
constexpr char context_header = 'H';

class Table : public Fl_Table
{
public:
    Table
    (
        int x, int y, int w, int h,
        int cols,
        int rows,
        const char *l = 0
    ) : Fl_Table(x, y, w, h, l) 
    {
        if (cols < 0 || rows < 0)
            throw std::invalid_argument("Table: cols and rows must be non-negative");

        std::cout << "Creating table..." << std::endl;
        std::cout << "Columns: " << cols << ", Rows: " << rows << std::endl;

        int cell_w { w / cols };
        int cell_h { h / rows };

        for (int col{0}; col < cols; ++col)
            for (int row{0}; row < rows; ++row)
            {
                if (row == 0)
                    draw_cell(context_header, row, col, x + col*cell_w, y, cell_w, cell_h); 
                else
                    draw_cell(context_cell, row, col, x + cell_w*col, y + cell_h*row, cell_w, cell_h);
            }


        for (int row_border{0}; row_border < rows + 1; ++row_border)
        {
            fl_color(COLORS::GRAY);
            if (row_border == 1 || row_border == rows || row_border == 0)
            {
                fl_color(COLORS::DARK_GRAY);
            }
            fl_line(x, y + row_border*cell_h, x + w, y + row_border*cell_h);
        }

        fl_color(COLORS::DARK_GRAY);
        for (int col_border{0}; col_border < cols + 1; ++col_border)
        {
            fl_line(x + col_border*cell_w, y, x + col_border*cell_w, y + h);
        }

        std::cout << "Table created." << std::endl;

    }

private:
    virtual void draw_cell(const char context, int R, int C, int X, int Y, int W, int H);
};

#endif // OBJECTS_H