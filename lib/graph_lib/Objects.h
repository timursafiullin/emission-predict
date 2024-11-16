#ifndef OBJECTS_H
#define OBJECTS_H 1

#include "FL/Fl_Table.H"
#include "FL/Fl_draw.h"
#include "FL/Fl_Box.H"
#include "fltk_design.h"
#include <iostream>
#include <vector>
#include <string>


constexpr char context_column = 'C';
constexpr char context_row = 'R';


struct Labels
{
    unsigned int row{};
    unsigned int col{};
    const char context{};

    Labels() {};
    Labels(std::initializer_list<std::string> labels, const char context, unsigned int line);

    std::string operator[](const unsigned int index);
    std::vector<std::string> get();
private:
    std::vector<std::string> labelList;
};


struct LabelsList
{
    LabelsList() {};
    LabelsList(std::initializer_list<Labels> labels);

    std::vector<Labels> get();
private:
    std::vector<Labels> labelsList;
};


class Table : public Fl_Table
{
public:
    Table
    (
        int x, int y, int w, int h,
        int table_cols,
        int table_rows,
        Fl_Color inner_color = FL_BLACK,
        Fl_Color outer_color = FL_BLACK,
        Fl_Color background_color = FL_WHITE
    );

    void set_label(Labels& labels);
    void set_label(LabelsList& labels_list);

private:
    int x, y, w, h;
    int rows, cols;
    int cell_w, cell_h;

    Fl_Color inner_color, outer_color, background_color;

    void draw_borders(Fl_Color color);
    void draw_borders(Fl_Color inner_color, Fl_Color outer_color);
};

#endif // OBJECTS_H