#include "fltk.h"
#include "Objects.h"
#include <initializer_list>

// Table
Table::Table
(
    int x, int y, int w, int h,
    int table_cols,
    int table_rows,
    Fl_Color inner_color,
    Fl_Color outer_color,
    Fl_Color background_color
) : Fl_Table(x, y, w, h), x{x}, y{y}, w{w}, h{h},
    inner_color{inner_color},
    outer_color{outer_color},
    background_color{background_color}
{
    cols = table_cols;
    rows = table_rows;

    if ( cols <= 0 || rows <= 0 )
        throw std::invalid_argument("Table: cols and rows must be more than 0");
    
    cell_w = w / cols;
    cell_h = h / rows;

    // Draw background
    fl_draw_box(FL_FLAT_BOX, x, y, w, h, background_color);

    // Draw borders
    draw_borders(inner_color, outer_color);

    std::cout << "Creating table..." << std::endl;
    std::cout << "Columns: " << cols << ", Rows: " << rows << std::endl;
    std::cout << "Table created." << std::endl;
} 


void Table::draw_borders(Fl_Color color)
{
    draw_borders(color, color);
}


void Table::draw_borders(Fl_Color inner_color, Fl_Color outer_color)
{
    for (int row_border{0}; row_border < rows + 1; ++row_border)
    {
        fl_color(inner_color);

        if (row_border == 1 || row_border == rows || row_border == 0)
            fl_color(outer_color);

        fl_line(x, y + row_border*cell_h, x + w, y + row_border*cell_h);
    }

    fl_color(outer_color);
    for (int col_border{0}; col_border < cols + 1; ++col_border)
        fl_line(x + col_border*cell_w, y, x + col_border*cell_w, y + h);
}


void Table::set_label(Labels& labels)
{
    for (unsigned int i{0}; i < labels.get().size(); ++i)
    {
        fl_color(COLORS::BLACK);
        fl_font(FL_HELVETICA, Fl_Fontsize(cell_h/2.25));
        fl_draw(labels[i].c_str(), x + cell_w * (labels.col + 0.075), y + cell_h * labels.row, cell_w, cell_h, FL_ALIGN_LEFT);
        switch (labels.context)
        {
            case context_column:
                ++labels.row;
                break;
            case context_row:
                ++labels.col;
                break;
            default:
                throw std::invalid_argument("Invalid context");
        }
    }
}


void Table::set_label(LabelsList& labels_list)
{
    for (Labels& labels : labels_list.get())
        set_label(labels);
}


// Label
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
        default:
            throw std::invalid_argument("Invalid context");
    }

    for (const auto& name : labels)
        labelList.push_back(name);
}


std::string Labels::operator[](const unsigned int index)
{
    if (index > labelList.size() - 1)
        throw std::runtime_error("Label Names List index out of range");

    return labelList[index];
}


std::vector<std::string> Labels::get() const
{
    return labelList;
}


// LabelsList
LabelsList::LabelsList(std::initializer_list<Labels> labels)
{
    for (const auto& _labels : labels)
        labelsList.push_back(_labels);
}


std::vector<Labels> LabelsList::get() const
{
    return labelsList;
}
