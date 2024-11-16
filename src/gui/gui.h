#ifndef GUI_H
#define GUI_H 1

#include <string>
#include "graph_lib.h"


constexpr unsigned int window_width     { 852 };
constexpr unsigned int window_height    { 550 };
const std::string main_window_title     { "Emission predict" };

constexpr unsigned int table_x          { 561 };
constexpr unsigned int table_y          {  21 };
constexpr unsigned int table_width      { 260 };
constexpr unsigned int table_height     { 406 };
constexpr unsigned int table_cols       {   2 };
constexpr unsigned int table_rows       {  14 };


class ProgramWindow : public Fl_Window
{
public:
  ProgramWindow(int w, int h, const std::string& title) : Fl_Window(w, h) { label(title.c_str()); };
  virtual ~ProgramWindow() {}

  void draw() override;
  void display() { show(); Fl::run(); };
};

ProgramWindow* create_window(unsigned int width, unsigned int height, std::string title, Fl_Color background_color);

#endif