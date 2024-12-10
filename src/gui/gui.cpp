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

    fl_color(COLORS::BRIGHT_BLUE);

    GLib::FunctionStepping *funkcia = new GLib::FunctionStepping{
        evaluations, 0, window.current_cell.speed, GLib::Point(canvas_origin_x, canvas_origin_y)};

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