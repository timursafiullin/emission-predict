#ifndef PROGRAM_WINDOW
#define PROGRAM_WINDOW 1

#include "GUI.h"

struct ProgramWindow : Graph_lib::Window
{
    ProgramWindow(int w, int h, const std::string& title, Fl_Color bgcolor) : Window{w, h, title, bgcolor} {}
};

#endif  // Program Window