#include <iostream>
#include "gui.h"
#include <exception>
#include <iostream>
#include <vector>

#include <cmath>

#define GLib Graph_lib

LabelsList labels_list(
    std::initializer_list<Labels>{
        Labels(std::initializer_list<std::string>{
                   "Parameters", "Vehicle Type", "Fuel Type",
                   "Engine Size", "Age of Vehicle", "Mileage",
                   "Acceleration", "Road Type",
                   "Traffic Conditions", "Temperature", "Humidity",
                   "Wind Speed", "Air Pressure", "Max speed"},
               context_column, 0),
        Labels(std::initializer_list<std::string>{"Values"}, context_column, 1)});

dlcList<Fl_Color> graph_colors{};
dlcList<EmissionState> emissions{};

static void show_gas_label(GLib::WindowWithNeuro &window, std::string gas_label);
void show_graph(GLib::WindowWithNeuro &window, EmissionState &state);

// CALLBACKS
void callback_predict(GLib::Address, GLib::Address addr)
{
  auto *pb = static_cast<GLib::Button *>(addr);
  auto &window = static_cast<GLib::WindowWithNeuro &>(pb->window());
  std::cout << "Button 'Predict' pressed!" << std::endl;

  std::vector<GLib::FunctionStepping *> functions = window.functions; // copy functions

  for (size_t i = 0; i < functions.size(); i++)
  {
    window.detach(*functions[i]); // delete all functions
  }

  EmissionState state = emissions.get_current();
  fl_color(graph_colors.get_current());
  show_graph(window, state);

  window.redraw();
}

void callback_save(GLib::Address, GLib::Address addr)
{
  auto *pb = static_cast<GLib::Button *>(addr);
  auto &window = static_cast<GLib::WindowWithNeuro &>(pb->window());
  std::cout << "Button 'History' pressed!" << std::endl;

  bool validated = window.validate_inboxes();
  if (validated)
  {
    std::ofstream save_file;
    save_file.open("saved_inputs.txt");
    if (save_file.is_open())
    {
      std::vector<std::string> inbox_values = window.get_values_from_inboxes();
      for (std::string &value : inbox_values)
      {
        save_file << value << std::endl;
      }
    }
    save_file.close();
    std::cout << "File has been written" << std::endl;
  }

  std::cout << "Button 'Save' pressed!" << std::endl;
}

void callback_history(GLib::Address, GLib::Address addr)
{
  auto *pb = static_cast<GLib::Button *>(addr);
  auto &window = static_cast<GLib::WindowWithNeuro &>(pb->window());

  std::ifstream save_file("saved_inputs.txt");
  std::vector<std::string> inbox_values;
  if (save_file.is_open())
  {
    std::string value;
    while (save_file >> value)
    {
      inbox_values.push_back(value);
    }
  }
  else
  {
    std::cout << "History file not found" << std::endl;
    return;
  }
  save_file.close();

  std::vector<GLib::In_box *> inboxes = window.inboxes; // copy inboxes

  for (size_t i = 0; i < inboxes.size(); i++)
  {
    inboxes[i]->set_string(inbox_values[i]);
  }
}

void callback_clear(GLib::Address, GLib::Address addr)
{
  auto *pb = static_cast<GLib::Button *>(addr);
  auto &window = static_cast<GLib::WindowWithNeuro &>(pb->window());
  std::cout << "Button 'Clear' pressed!" << std::endl;

  std::vector<GLib::FunctionStepping *> functions = window.functions; // copy functions

  for (size_t i = 0; i < functions.size(); i++)
  {
    window.detach(*functions[i]); // delete all functions
  }

  std::vector<GLib::GasText *> gas_texts = window.gas_texts; // copy gas_texts

  for (size_t i = 0; i < gas_texts.size(); i++)
  {
    window.detach(*gas_texts[i]); // delete all gas_texts
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
}

static void callback_next(GLib::Address, GLib::Address addr)
{
  auto *pb = static_cast<GLib::Button *>(addr);
  auto &window = static_cast<GLib::WindowWithNeuro &>(pb->window());
  std::cout << "Button 'Next' pressed!" << std::endl;

  std::vector<GLib::FunctionStepping *> functions = window.functions; // copy functions

  for (size_t i = 0; i < functions.size(); i++)
  {
    window.detach(*functions[i]); // delete all functions
  }

  EmissionState state = emissions.get_next();
  fl_color(graph_colors.get_next());
  show_graph(window, state);

  window.redraw();
}

static void callback_prev(GLib::Address, GLib::Address addr)
{
  auto *pb = static_cast<GLib::Button *>(addr);
  auto &window = static_cast<GLib::WindowWithNeuro &>(pb->window());
  std::cout << "Button 'Prev' pressed!" << std::endl;

  std::vector<GLib::FunctionStepping *> functions = window.functions; // copy functions

  for (size_t i = 0; i < functions.size(); i++)
  {
    window.detach(*functions[i]); // delete all functions
  }

  EmissionState state = emissions.get_previous();
  fl_color(graph_colors.get_previous());
  show_graph(window, state);

  window.redraw();
}

static void show_gas_label(GLib::WindowWithNeuro &window, std::string gas_label)
{
  unsigned int gas_label_x{graph_canvas_x + graph_canvas_w / 2 - (gas_label.size() / 2) * 8}, gas_label_y{next_gas_y + 17};

  std::vector<GLib::GasText *> gas_texts = window.gas_texts; // copy gas_texts

  for (size_t i = 0; i < gas_texts.size(); i++)
  {
    window.detach(*gas_texts[i]); // delete all gas_texts
  }

  GLib::GasText *gas = new GLib::GasText{GLib::Point(gas_label_x, gas_label_y), gas_label};
  gas->set_color(COLORS::BLACK);
  gas->set_font(FL_HELVETICA);
  gas->set_font_size(16);

  window.attach(*gas);
}

void show_graph(GLib::WindowWithNeuro &window, EmissionState &state)
{
  bool validated = window.validate_inboxes();
  if (validated)
  {
    show_gas_label(window, state.gas_label);
    window.update_current_cell();
    std::vector<double> evaluations;

    switch (state.gas_tag)
    {
    case CO2:
    {
      evaluations = window.evaluate_network(CO2);
    }
    break;
    case NOX:
    {
      evaluations = window.evaluate_network(NOX);
    }
    break;
    case SO2:
    {
      evaluations = window.evaluate_network(SO2);
    }
    break;
    case VOC:
    {
      evaluations = window.evaluate_network(VOC);
    }
    break;
    case PM25:
    {
      evaluations = window.evaluate_network(PM25);
    }
    break;
    }

    GLib::FunctionStepping *func = new GLib::FunctionStepping{
        evaluations, 0, window.current_cell.speed, GLib::Point(canvas_origin_x, canvas_origin_y)};

    window.attach(*func);
  }
}

int main_gui()
try
{
  graph_colors.insert(COLORS::BARBIE_GRAY);
  graph_colors.insert(COLORS::BRIGHT_BLUE);
  graph_colors.insert(COLORS::BLACK);
  graph_colors.insert(FL_GREEN);
  graph_colors.insert(FL_YELLOW);
  graph_colors.insert(FL_RED);

  emissions.insert(EmissionState("CO2 Emissions", CO2));
  emissions.insert(EmissionState("NOx Emissions", NOX));
  emissions.insert(EmissionState("PM2.5 Emissions", PM25));
  emissions.insert(EmissionState("VOC Emissions", VOC));
  emissions.insert(EmissionState("SO2 Emissions", SO2));

  // CREATING MAIN WINDOW
  GLib::WindowWithNeuro win{
      window_width, window_height,
      main_window_title, COLORS::LIGHT_GRAY};

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
  for (size_t i = 0; i < table_rows - 1; ++i)
  {
    inboxes.push_back(new GLib::In_box{
        GLib::Point(inbox_x, inbox_y + (inbox_h + 1) * i),
        inbox_w, inbox_h, ""});
    win.attach(*inboxes[i]);
  }

  // NEXT GAS BUTTON
  GLib::Button next_button{
      GLib::Point(next_gas_x, next_gas_y),
      gas_button_w, gas_button_w,
      next_gas_label,
      callback_next};
  next_button.set_box_type(FL_OVAL_BOX);
  win.attach(next_button);

  // PREV GAS BUTTON
  GLib::Button prev_button{
      GLib::Point(prev_gas_x, prev_gas_y),
      gas_button_w, gas_button_w,
      prev_gas_label,
      callback_prev};
  prev_button.set_box_type(FL_OVAL_BOX);
  win.attach(prev_button);

  return Fl::run();
}
catch (std::exception &e)
{
  std::cerr << e.what() << std::endl;
  return 1;
}
catch (...)
{
  std::cerr << "Unknown exception." << std::endl;
  return 1;
}