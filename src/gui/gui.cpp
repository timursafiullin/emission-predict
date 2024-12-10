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

// CALLBACKS
void callback_predict(GLib::Address, GLib::Address addr)
{
  auto *pb = static_cast<GLib::Button *>(addr);
  auto &window = static_cast<GLib::WindowWithNeuro &>(pb->window());
  window.redraw();
  std::cout << "Button 'Predict' pressed!" << std::endl;

  std::vector<std::string> inbox_values = window.get_values_from_inboxes();
  bool validated = window.validate_inboxes();

  if (validated)
  {
    window.current_cell.vehicle_type = inbox_values[0];
    window.current_cell.fuel_type = inbox_values[1];
    window.current_cell.engine_size = std::stold(inbox_values[2]);
    window.current_cell.age_of_vehicle = std::stoul(inbox_values[3]);
    window.current_cell.mileage = std::stoull(inbox_values[4]);
    window.current_cell.speed = std::stold(inbox_values[5]);
    window.current_cell.acceleration = std::stold(inbox_values[6]);
    window.current_cell.road_type = inbox_values[7];
    window.current_cell.traffic_conditions = inbox_values[8];
    window.current_cell.temperature = std::stold(inbox_values[9]);
    window.current_cell.humidity = std::stold(inbox_values[10]);
    window.current_cell.wind_speed = std::stold(inbox_values[11]);
    window.current_cell.air_pressure = std::stold(inbox_values[12]);

    std::vector<double> evaluations = window.evaluate_network_CO2();

    if (window.graph_is_shown)
      window.detach(*window.shapes.back()); // delete function shape. In fact always the last one in vector
    else
      window.graph_is_shown = true;

  fl_color(graph_colors.get_current());

  GLib::Function *funkcia = new GLib::Function{
      sqrt, 0, 15, GLib::Point(canvas_origin_x, canvas_origin_y - 50)};

    window.attach(*funkcia);

    
  }
}

void callback_save(GLib::Address, GLib::Address addr)
{
  auto *pb = static_cast<GLib::Button *>(addr);
  auto &window = static_cast<GLib::WindowWithNeuro &>(pb->window());

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

  std::cout << "Button 'History' pressed!" << std::endl;
}

void callback_clear(GLib::Address, GLib::Address addr)
{
  auto *pb = static_cast<GLib::Button *>(addr);
  auto &window = static_cast<GLib::WindowWithNeuro &>(pb->window());

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

static void show_gas_label(GLib::Window& window, std::string gas_label);
void show_graph(EmissionState& state);

static void callback_next(GLib::Address, GLib::Address addr)
{
  auto *pb = static_cast<GLib::Button *>(addr);
  auto &window = static_cast<GLib::Window &>(pb->window());
  window.redraw();
  std::cout << "Button 'Next' pressed!" << std::endl;

  if (window.graph_is_shown)
    window.detach(*window.shapes.back()); // delete function shape. In fact always the last one in vector
  else
    window.graph_is_shown = true;

  EmissionState state = emissions.get_next();
  show_gas_label(window, state.gas_label);
  show_graph(state);

  // для наглядности и теста
  fl_color(graph_colors.get_next());
  GLib::Function *funkcia = new GLib::Function{
      sqrt, 0, 15, GLib::Point(canvas_origin_x, canvas_origin_y - 50)};
  window.attach(*funkcia);
}

static void callback_prev(GLib::Address, GLib::Address addr)
{
  auto *pb = static_cast<GLib::Button *>(addr);
  auto &window = static_cast<GLib::Window &>(pb->window());
  window.redraw();
  std::cout << "Button 'Prev' pressed!" << std::endl;

  if (window.graph_is_shown)
    window.detach(*window.shapes.back()); // delete function shape. In fact always the last one in vector
  else
    window.graph_is_shown = true;

  EmissionState state = emissions.get_previous();
  show_gas_label(window, state.gas_label);
  show_graph(state);

  // для наглядности и теста
  fl_color(graph_colors.get_previous());
  GLib::Function *funkcia = new GLib::Function{
      sqrt, 0, 15, GLib::Point(canvas_origin_x, canvas_origin_y - 50)};

  window.attach(*funkcia);
}

static void show_gas_label(GLib::Window& window, std::string gas_label)
{
  unsigned int gas_label_x{graph_canvas_x + graph_canvas_w / 2 - (gas_label.size() / 2) * 8}, gas_label_y{next_gas_y + 17};

  GLib::Text *gas = new GLib::Text{GLib::Point(gas_label_x, gas_label_y), gas_label};
  gas->set_color(COLORS::BLACK);
  gas->set_font(FL_HELVETICA);
  gas->set_font_size(16);

  window.attach(*gas);
}

void show_graph(EmissionState& state)
{
  // ... (rest of the code)
  switch(state.gas_tag)
  {
    case CO2:
      // co2 graph
      break;
    case NOX:
      // nox graph
      break;
    case SO2:
      // so2 graph
      break;
    case VOC:
      // voc graph
      break;
    case PM25:
      // pm2.5 graph
      break;
    default:
      // something
      break;
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
    COLORS::LIGHT_GRAY, COLORS::LIGHT_GRAY
  };
  parameters_table.set_label(labels_list);
  win.attach(parameters_table);

  // PREDICT BUTTON
  GLib::Button predict_button{
    GLib::Point(predict_button_x, predict_button_y),
    button_w, button_h,
    predict_button_label,
    callback_predict
  };
  win.attach(predict_button);

  // SAVE BUTTON
  GLib::Button save_button{
    GLib::Point(save_button_x, save_button_y),
    button_w, button_h,
    save_button_label,
    callback_save
  };
  win.attach(save_button);

  // HISTORY BUTTON
  GLib::Button history_button{
    GLib::Point(history_button_x, history_button_y),
    button_w, button_h,
    history_button_label,
    callback_history
  };
  win.attach(history_button);

  // CLEAR BUTTON
  GLib::Button clear_button{
    GLib::Point(clear_button_x, clear_button_y),
    button_w, button_h,
    clear_button_label,
    callback_clear
  };
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
    callback_next
  };
  next_button.set_box_type(FL_OVAL_BOX);
  win.attach(next_button);

  // PREV GAS BUTTON
  GLib::Button prev_button{
    GLib::Point(prev_gas_x, prev_gas_y),
    gas_button_w, gas_button_w,
    prev_gas_label,
    callback_prev
  };
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