#include "gui.h"
#include <exception>

#define GLib Graph_lib

static void cb_empty(GLib::Address, GLib::Address addr)
{
  auto* pb = static_cast<GLib::Button*>(addr);
  static_cast<GLib::Window&>(pb->window()).redraw();
}

int main()
try
{
  // CREATING MAIN WINDOW
  GLib::Window win{window_width, window_height, main_window_title, COLORS::LIGHT_GRAY};

  // TABLE OF PARAMETERS AND VALUES
  GLib::Table parameters_table
  {
    table_x, table_y,
    table_width, table_height,
    table_cols, table_rows,
    COLORS::LIGHT_GRAY, COLORS::LIGHT_GRAY
  };
  parameters_table.set_label(labels_list);
  win.attach(parameters_table);

  // PREDICT BUTTON
  GLib::Button predict_button
  {
    GLib::Point(predict_button_x, predict_button_y),
    button_w, button_h,
    predict_button_label,
    cb_empty
  };
  win.attach(predict_button);

  // SAVE BUTTON
  GLib::Button save_button
  {
    GLib::Point(save_button_x, save_button_y),
    button_w, button_h,
    save_button_label,
    cb_empty
  };
  win.attach(save_button);

  // HISTORY BUTTON
  GLib::Button history_button
  {
    GLib::Point(history_button_x, history_button_y),
    button_w, button_h,
    history_button_label,
    cb_empty
  };
  win.attach(history_button);

  // CLEAR BUTTON
  GLib::Button clear_button
  {
    GLib::Point(clear_button_x, clear_button_y),
    button_w, button_h,
    clear_button_label,
    cb_empty
  };
  win.attach(clear_button);

  GLib::GraphCanvas graph_canvas
  {
    GLib::Point(graph_canvas_x, graph_canvas_y),
    graph_canvas_w, graph_canvas_h,
    GLib::Point(canvas_origin_x, canvas_origin_y)
  };
  win.attach(graph_canvas);

  return Fl::run();
}
catch (std::exception& e)
{
  std::cerr << e.what() << std::endl;
}
catch (...)
{
  std::cerr << "Unknown exception." << std::endl;
}