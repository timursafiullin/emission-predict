#ifndef GUI_H
#define GUI_H 1

#include <string>
#include "graph_lib.h"

#define GLib Graph_lib

// Window
const std::string main_window_title{"Emission predict"};
constexpr unsigned int window_width{852};
constexpr unsigned int window_height{550};

// Table
constexpr unsigned int table_x{561};
constexpr unsigned int table_y{21};
constexpr unsigned int table_width{260};
constexpr unsigned int table_height{416};
constexpr unsigned int table_cols{2};
constexpr unsigned int table_rows{14};

// Buttons
constexpr unsigned int button_w{121};
constexpr unsigned int button_h{25};

const std::string predict_button_label{"Predict"};
constexpr unsigned int predict_button_x{table_x};
constexpr unsigned int predict_button_y{462};

const std::string history_button_label{"Load"};
constexpr unsigned int history_button_x{table_x + table_width - button_w};
constexpr unsigned int history_button_y{501};

const std::string clear_button_label{"Clear"};
constexpr unsigned int clear_button_x{table_x + table_width - button_w};
constexpr unsigned int clear_button_y{462};

const std::string save_button_label{"Save"};
constexpr unsigned int save_button_x{table_x};
constexpr unsigned int save_button_y{501};


const std::string prev_gas_label{"<"};
constexpr unsigned int prev_gas_x{441};
constexpr unsigned int prev_gas_y{32};

const std::string next_gas_label{">"};
constexpr unsigned int next_gas_x{485};
constexpr unsigned int next_gas_y{32};

constexpr unsigned int gas_button_w{30}, gas_button_h{30};

// Menu for table
const unsigned int vehicle_type_x{table_x + table_width / table_cols};
const unsigned int vehicle_type_y{table_y + table_height / table_rows};

// Graph Canvas
const unsigned int graph_canvas_x{23};
const unsigned int graph_canvas_y{21};
const unsigned int graph_canvas_w{504};
const unsigned int graph_canvas_h{505};
const unsigned int canvas_origin_x{96}; // was 62
const unsigned int canvas_origin_y{489};
const unsigned int num_of_graph_labels_x{4};
const unsigned int num_of_graph_labels_y{4};

// FDFK
const unsigned int inbox_x{1 + table_x + table_width / table_cols};
const unsigned int inbox_y{1 + table_y + table_height / table_rows};
const unsigned int inbox_w{-1 + table_width / table_cols};
const unsigned int inbox_h{-1 + table_height / table_rows};

extern LabelsList labels_list;

struct EmissionState
{
    std::string gas_label;
    char gas_tag; // Unique identifier for emission
    Fl_Color graph_color;

    EmissionState(std::string gas_label, char gas_tag, Fl_Color graph_color = COLORS::BRIGHT_BLUE) : gas_label(gas_label), gas_tag(gas_tag), graph_color(graph_color) {}
};

constexpr char CO2 {'C'};
constexpr char NOX {'N'};
constexpr char PM25 {'P'};
constexpr char VOC {'V'};
constexpr char SO2 {'S'};
extern void callback_predict(GLib::Address, GLib::Address addr);
extern void callback_save(GLib::Address, GLib::Address addr);
extern void callback_history(GLib::Address, GLib::Address addr);
extern void callback_clear(GLib::Address, GLib::Address addr);

extern int main_gui();
extern std::string to_string_exp(double d);

#endif