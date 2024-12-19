#ifndef GUI_H
#define GUI_H 1

#include <string>
#include "graph_lib.h"

#define GLib Graph_lib

// Window
const std::string main_window_title{"Emission predict"};
constexpr unsigned int window_width{852};
constexpr unsigned int window_height{550};

constexpr unsigned int window_width_journal{1152};

// Table
constexpr unsigned int table_x{561};
constexpr unsigned int table_y{21};
constexpr unsigned int table_width{260};
constexpr unsigned int table_height{408};
constexpr unsigned int table_cols{2};
constexpr unsigned int table_rows{14};

// Journal
constexpr unsigned int journal_x{854};
constexpr unsigned int journal_y{21};
constexpr unsigned int journal_width{266};
constexpr unsigned int journal_height{505};

// Buttons
constexpr unsigned int button_w{125};
constexpr unsigned int button_h{25};
constexpr unsigned int button_w_div_2{60};

const std::string predict_button_label{"Predict"};
constexpr unsigned int predict_button_x{table_x};
constexpr unsigned int predict_button_y{434};

const std::string help_button_label{"Help"};
constexpr unsigned int help_button_x{table_x + table_width - button_w};
constexpr unsigned int help_button_y{434};

const std::string save_button_label{"Save"};
constexpr unsigned int save_button_x{table_x};
constexpr unsigned int save_button_y{468};

const std::string load_button_label{"Load"};
constexpr unsigned int load_button_x{table_x + table_width - button_w};
constexpr unsigned int load_button_y{468};

const std::string journal_button_label{"Journal"};
constexpr unsigned int journal_button_x{table_x + table_width - button_w};
constexpr unsigned int journal_button_y{501};

const std::string clear_button_label{"Clear"};
constexpr unsigned int clear_button_x{table_x};
constexpr unsigned int clear_button_y{501};

const std::string prev_gas_label{"<"};
constexpr unsigned int prev_gas_x{441};
constexpr unsigned int prev_gas_y{32};

const std::string next_gas_label{">"};
constexpr unsigned int next_gas_x{485};
constexpr unsigned int next_gas_y{32};

constexpr unsigned int gas_button_w{30}, gas_button_h{30};

// Help window
constexpr unsigned int help_window_width{650};
constexpr unsigned int help_window_height{450};
extern std::string help_message;

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
const unsigned int max_num_of_graph_labels{8};
const std::vector<unsigned int> graph_labels_steps = {1, 2, 5, 10, 25, 50, 100, 500, 1000};

// FDFK
const unsigned int inbox_x{1 + table_x + table_width / table_cols};
const unsigned int inbox_y{1 + table_y + table_height / table_rows};
const unsigned int inbox_w{-1 + table_width / table_cols};
const unsigned int inbox_h{-1 + table_height / table_rows};

// SAVE FILE REGEX
const std::string file_regex = "^(Truck|Car|Motorcycle|Bus)\n(Petrol|Electric|Diesel|hybrid)\n([0-9]+[.])?[0-9]+\n[0-9]+\n[0-9]+\n([0-9]+[.])?[0-9]+\n(City|Highway|Rural)\n(Free flow|Heavy|Moderate)\n[+-]?([0-9]+[.])?[0-9]+\n([0-9]+[.])?[0-9]+\n([0-9]+[.])?[0-9]+\n([0-9]+[.])?[0-9]+\n[0-9]+\n$";

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
extern void callback_load(GLib::Address, GLib::Address addr);
extern void callback_clear(GLib::Address, GLib::Address addr);

extern int main_gui();
extern std::string to_string_exp(double d);
extern void show_error_message(std::string message);

#endif