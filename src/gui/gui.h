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


LabelsList labels_list
(
  std::initializer_list<Labels>
  {
    Labels(std::initializer_list<std::string>
    {
      "Parameters", "Vehicle Type", "Fuel Type",
      "Engine Size", "Age of Vehicle", "Mileage",
      "Acceleration", "Road Type",
      "Traffic Conditions", "Temperature", "Humidity",
      "Wind Speed", "Air Pressure", "Max speed",
    }, context_column, 0),
    Labels(std::initializer_list<std::string>
    {
      "Values"
    }, context_column, 1)
  }
);

#endif