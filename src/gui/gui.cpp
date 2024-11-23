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
  GLib::Window win{window_width, window_height, main_window_title, COLORS::LIGHT_GRAY};
  GLib::Table parameters_table
  {
    table_x, table_y,
    table_width, table_height,
    table_cols, table_rows,
    COLORS::LIGHT_GRAY, COLORS::LIGHT_GRAY
  };
  parameters_table.set_label(labels_list);
  win.attach(parameters_table);

  GLib::Button predict_button {GLib::Point(predict_button_x, predict_button_y), button_w, button_h, predict_button_label, cb_empty};
  win.attach(predict_button);

  GLib::Button save_button {GLib::Point(save_button_x, save_button_y), button_w, button_h, save_button_label, cb_empty};
  win.attach(save_button);

  GLib::Button history_button {GLib::Point(history_button_x, history_button_y), button_w, button_h, history_button_label, cb_empty};
  win.attach(history_button);

  GLib::Button clear_button {GLib::Point(clear_button_x, clear_button_y), button_w, button_h, clear_button_label, cb_empty};
  win.attach(clear_button);

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