#include "fltk.h"
#include "Objects.h"
#include <initializer_list>

//
// Table
//
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

void Table::set_label(Labels& labels)
{
    for (auto& label : labels.get())
    {
        fl_color(COLORS::DARK_GRAY);
        fl_draw(label.c_str(), x + cell_w * labels.col, y + cell_h * labels.row, cell_w, cell_h, FL_ALIGN_CENTER);
    }
}

void Table::set_label(LabelsList& labels_list)
{
    for (Labels& labels : labels_list.get())
    {
        for (unsigned int i{0}; i < labels.get().size(); ++i)
        {
            fl_color(COLORS::BLACK);
            fl_font(FL_HELVETICA, Fl_Fontsize(cell_h/2.25));
            fl_draw(labels[i].c_str(), x + cell_w * labels.col, y + cell_h * labels.row, cell_w, cell_h, FL_ALIGN_CENTER);
            switch (labels.context)
            {
                case context_column:
                    ++labels.row;
                    break;
                case context_row:
                    ++labels.col;
                    break;
            }
        }  
    }
}

//
// Label
//
Labels::Labels(std::initializer_list<std::string> labels, const char context, unsigned int line) : context{context}
{
    switch (context)
    {
        case context_column:
            col = line;
            break;
        case context_row:
            row = line;
            break;
    }

    for (const auto& name : labels)
    {
        labelList.push_back(name);
    }    
}

std::string Labels::operator[](const unsigned int index)
{
    if (index > labelList.size() - 1)
    {
        throw std::runtime_error("Label Names List index out of range");
    }
    return labelList[index];
}

std::vector<std::string> Labels::get()
{
    return labelList;
}

//
// LabelsList
//
LabelsList::LabelsList(std::initializer_list<Labels> labels)
{
    for (const auto& _labels : labels)
    {
        labelsList.push_back(_labels);
    } 
}

std::vector<Labels> LabelsList::get()
{
    return labelsList;
}