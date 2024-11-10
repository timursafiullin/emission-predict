#ifndef OBJECTS_H
#define OBJECTS_H 1

#include "FL/Fl_Scroll.H"
#include "FL/Fl_Table.H"
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
            
        for (int row{0}; row < rows; ++row)
        {
            for (int col{0}; col < cols; ++col)
            {
                if (row == 0)
                {
                    draw_cell(context_header, row, col, x, y, cell_w, cell_h); 
                }
                else
                {
                    draw_cell(context_cell, row, col, x + cell_w*col, y - cell_h*row, cell_w, cell_h);
                }
            }
        }
        std::cout << "Table created." << std::endl;
    }
private:
    void draw_cell(const char context, int R, int C, int X, int Y, int W, int H);
};

#endif // OBJECTS_H