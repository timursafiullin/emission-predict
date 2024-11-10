#include "fltk.h"
#include "Objects.h"
#include <iostream>

class RateTable : public Fl_Scroll {
    #define COLS 2
    #define ROWS 6
    void *w[ROWS][COLS];        // widget pointers
public:
    RateTable(int X, int Y, int W, int H, const char *L=0) : Fl_Scroll(X, Y, W, H, L) {
        static const char *header[COLS] = {
            "Parameter", "Value"
        };
        int cellw = W / 2;
        int cellh = 29;
        int xx = X, yy = Y;
        Fl_Tile *tile = new Fl_Tile(X, Y, cellw*COLS, cellh*ROWS);
        // Create widgets
        for ( int r{0}; r < ROWS; ++r ) {
            for ( int c{0}; c < COLS; ++c ) {
                if ( r == 0 ) {
                    Fl_Box *box = new Fl_Box(xx, yy, cellw, cellh, header[c]);
                    box->labelfont(FL_HELVETICA);
                    box->labelcolor(COLORS::DARK_GRAY);
                    box->box(FL_BORDER_BOX);
                    box->color(COLORS::WHITE);
                    box->color2(COLORS::DARK_GRAY);
                    w[r][c] = (void*)box;
                } else if ( c == 0 ) {
                    Fl_Input *in = new Fl_Input(xx, yy, cellw, cellh);
                    in->box(FL_BORDER_BOX);
                    in->color(COLORS::WHITE);
                    in->value("");
                    w[r][c] = (void*)in;
                } else {
                    Fl_Float_Input *in = new Fl_Float_Input(xx, yy, cellw, cellh);
                    in->box(FL_BORDER_BOX);
                    in->align(FL_ALIGN_RIGHT);
                    in->textfont(FL_HELVETICA);
                    in->textcolor(COLORS::DARK_GRAY);
                    in->textsize(Fl_Fontsize(cellh / 1.5));
                    in->color(COLORS::WHITE);
                    in->value("0.00");
                    w[r][c] = (void*)in;
                }
                xx += cellw;
            }
            xx = X;
            yy += cellh;
        }
        tile->end();
        end();
        #undef COLS
        #undef ROWS
    }
};

int create_ratetable()
{
    RateTable rate(10,10,720-20,486-20);
    return 0;
};

 // This is called whenever Fl_Table wants you to draw a cell
void Table::draw_cell(const char context, int R, int C, int X, int Y, int W, int H) {
    static char s[40];
    sprintf(s, "%d/%d", R, C);
    switch (context)
    {
        case context_header:
            fl_push_clip(X, Y, W, H);
            {
                fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, color());
                fl_color(COLORS::BRIGHT_BLUE);
                fl_draw(s, X, Y, W, H, FL_ALIGN_CENTER);
            }
            fl_pop_clip();
            return;
 
        case context_cell:                  // Fl_Table telling us to draw cells
            fl_push_clip(X, Y, W, H);
            {
                // BG COLOR
                fl_color(COLORS::BRIGHT_BLUE);
                fl_rectf(X, Y, W, H);
 
                // TEXT
                fl_color(FL_BLACK);
                fl_draw(s, X, Y, W, H, FL_ALIGN_CENTER);
 
                // BORDER
                fl_color(FL_RED);
                fl_rect(X, Y, W, H);
            }
            fl_pop_clip();
            return;
 
        default:
            std::cout << C << " " << R << std::endl;
            return;
    }
}