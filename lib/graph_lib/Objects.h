#ifndef OBJECTS_H
#define OBJECTS_H 1

#include "FL/Fl_Scroll.H"

#define COLS 2
#define ROWS 6
class RateTable : public Fl_Scroll {
    void *w[ROWS][COLS];
public:
    RateTable(int x, int y, int w, int h, const char *l = 0);
    void draw();
};

int create_table();

#endif // OBJECTS_H