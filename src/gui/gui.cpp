#include "gui.h"
#include <exception>
#include <iostream>
#include <vector>

#include <cmath>

#define GLib Graph_lib

// CALLBACKS
static void callback_predict(GLib::Address, GLib::Address addr)
{
  auto *pb = static_cast<GLib::Button *>(addr);
  auto &window = static_cast<GLib::Window &>(pb->window());
  window.redraw();
  std::cout << "Button 'Predict' pressed!" << std::endl;

  if (window.graph_is_shown)
    window.detach(*window.shapes.back()); // delete function shape. In fact always the last one in vector
  else
    window.graph_is_shown = true;

  fl_color(COLORS::BRIGHT_BLUE);

  GLib::Function *funkcia = new GLib::Function{
      sin, 0, 15, GLib::Point(canvas_origin_x, canvas_origin_y - 50)};

  window.attach(*funkcia);
}

static void callback_save(GLib::Address, GLib::Address addr)
{
  auto *pb = static_cast<GLib::Button *>(addr);
  static_cast<GLib::Window &>(pb->window()).redraw();
  std::cout << "Button 'Save' pressed!" << std::endl;
}

static void callback_history(GLib::Address, GLib::Address addr)
{
  auto *pb = static_cast<GLib::Button *>(addr);
  static_cast<GLib::Window &>(pb->window()).redraw();
  std::cout << "Button 'History' pressed!" << std::endl;
}

static void callback_clear(GLib::Address, GLib::Address addr)
{
  auto *pb = static_cast<GLib::Button *>(addr);
  auto &window = static_cast<GLib::Window &>(pb->window());

  if (window.graph_is_shown)
  {
    window.detach(*window.shapes.back()); // delete function shape. In fact always the last one in vector
    window.graph_is_shown = false;
  }
  std::vector<GLib::Widget *> widgets = window.widgets; // copy widgets

  for (size_t i = 0; i < widgets.size(); i++)
  {
    window.detach(*widgets[i]); // delete all widgets
  }
  for (size_t i = 0; i < widgets.size(); i++)
  {
    window.attach(*widgets[i]); // restore all widgets
  }

  window.redraw();
  std::cout << "Button 'Clear' pressed!" << std::endl;
}

int main()
try
{
  // CREATING MAIN WINDOW
  GLib::Window win{window_width, window_height, main_window_title, COLORS::LIGHT_GRAY};

  // TABLE OF PARAMETERS AND VALUES
  GLib::Table parameters_table{
      table_x, table_y,
      table_width, table_height,
      table_cols, table_rows,
      COLORS::LIGHT_GRAY, COLORS::LIGHT_GRAY};
  parameters_table.set_label(labels_list);
  win.attach(parameters_table);

  // PREDICT BUTTON
  GLib::Button predict_button{
      GLib::Point(predict_button_x, predict_button_y),
      button_w, button_h,
      predict_button_label,
      callback_predict};
  win.attach(predict_button);

  // SAVE BUTTON
  GLib::Button save_button{
      GLib::Point(save_button_x, save_button_y),
      button_w, button_h,
      save_button_label,
      callback_save};
  win.attach(save_button);

  // HISTORY BUTTON
  GLib::Button history_button{
      GLib::Point(history_button_x, history_button_y),
      button_w, button_h,
      history_button_label,
      callback_history};
  win.attach(history_button);

  // CLEAR BUTTON
  GLib::Button clear_button{
      GLib::Point(clear_button_x, clear_button_y),
      button_w, button_h,
      clear_button_label,
      callback_clear};
  win.attach(clear_button);

  // GRAPH AREA (WHITE BOX WITH BLACK AXISES AND LIGHT GRAY GRID)
  GLib::GraphCanvas graph_canvas{
      GLib::Point(graph_canvas_x, graph_canvas_y),
      graph_canvas_w, graph_canvas_h,
      GLib::Point(canvas_origin_x, canvas_origin_y)};

  win.attach(graph_canvas);

  std::vector<GLib::In_box *> inboxes;
  for (size_t i = 0; i < 13; ++i)
  {
    inboxes.push_back(new GLib::In_box{
        GLib::Point(inbox_x, inbox_y + inbox_h * i),
        inbox_w, inbox_h, ""});
    win.attach(*inboxes[i]);
  }

  return Fl::run();
}
catch (std::exception &e)
{
  std::cerr << e.what() << std::endl;
}
catch (...)
{
  std::cerr << "Unknown exception." << std::endl;
}