#ifndef OBJECTS_H
#define OBJECTS_H 1

#include "FL/Fl_Table.H"
#include "FL/Fl_draw.h"
#include "FL/Fl_Box.H"
#include "fltk_design.h"
#include <iostream>
#include <vector>
#include <string>

constexpr char context_cell = 'C';
constexpr char context_header = 'H';

struct TableHeader
{
    TableHeader() {};
    TableHeader(std::initializer_list<std::string> header_names);
    std::string operator[](const unsigned int index);
    std::vector<std::string> get();
private:
    std::vector<std::string> nameList;
};

class Table : public Fl_Table
{
public:
    Table
    (
        int x, int y, int w, int h,
        int table_cols,
        int table_rows,
        Fl_Color background_color = FL_WHITE
    ) : Fl_Table(x, y, w, h), x{x}, y{y}, w{w}, h{h},
        background_color{background_color}
    {
        Fl_Table::color(background_color);
        cols = table_cols;
        rows = table_rows;

        // Draw background
        fl_draw_box(FL_FLAT_BOX, x, y, w, h, background_color);

        if ( cols < 0 || rows < 0 )
            throw std::invalid_argument("Table: cols and rows must be non-negative");
        std::cout << "Creating table..." << std::endl;
        std::cout << "Columns: " << cols << ", Rows: " << rows << std::endl;
        cell_w = w / cols;
        cell_h = h / rows;
        for ( int col{0}; col < cols; ++col )
            for ( int row{0}; row < rows; ++row )
            {
                draw_cell(row, col, x + col*cell_w, y + cell_h*row, cell_w, cell_h); 
            }
        
        std::cout << "Table created." << std::endl;
    }

    Table
    (
        int x, int y, int w, int h,
        int table_cols,
        int table_rows,
        TableHeader table_header,
        Fl_Color inner_color,
        Fl_Color outer_color,
        Fl_Color background_color = FL_WHITE
    ) : Fl_Table(x, y, w, h), x{x}, y{y}, w{w}, h{h},
        inner_color{inner_color},
        outer_color{outer_color},
        table_header{table_header},
        background_color{background_color}
    {
        Fl_Table::color(background_color);
        cols = table_cols;
        rows = table_rows;

        // Draw background
        fl_draw_box(FL_FLAT_BOX, x, y, w, h, background_color);

        if ( cols < 0 || rows < 0 )
            throw std::invalid_argument("Table: cols and rows must be non-negative");
        std::cout << "Creating table..." << std::endl;
        std::cout << "Columns: " << cols << ", Rows: " << rows << std::endl;
        cell_w = w / cols;
        cell_h = h / rows;
        for ( int col{0}; col < cols; ++col )
            for ( int row{0}; row < rows; ++row )
            {
                draw_cell(row, col, x + col*cell_w, y + cell_h*row, cell_w, cell_h); 
            }

        draw_borders(inner_color, outer_color);

        std::cout << "Table created." << std::endl;
    }

private:
    int rows, cols;
    int cell_w, cell_h;
    int x, y, w, h;
    TableHeader table_header;
    Fl_Color inner_color, outer_color, background_color;
    virtual void draw_cell(int R, int C, int X, int Y, int W, int H);
    void draw_borders(Fl_Color color);
    void draw_borders(Fl_Color inner_color, Fl_Color outer_color);
};

#endif // OBJECTS_H