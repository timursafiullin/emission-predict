#include <iostream>
#include "../window_with_neuro.h"
#include "gui.h"
#include <exception>
#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <regex>
#include <iterator>

#if (defined(_WIN32) || defined(_WIN64))
#include <winuser.h>
#endif

#define GLib Graph_lib

std::string to_string_exp(double d)
{
  std::ostringstream os;
  os << std::scientific << std::setprecision(6) << d;
  return os.str();
}

LabelsList labels_list(
    std::initializer_list<Labels>{
        Labels(std::initializer_list<std::string>{
                   "Parameters", "Vehicle Type", "Fuel Type",
                   "Engine Size (liters)", "Age of Vehicle (y.)", "Mileage (km)",
                   "Acceleration (m/s²)", "Road Type",
                   "Traffic Conditions", "Temperature (°C)", "Humidity (%)",
                   "Wind Speed (m/s)", "Air Pressure (hPa)", "Max speed (km/h)"},
               context_column, 0),
        Labels(std::initializer_list<std::string>{"Values"}, context_column, 1)});

dlcList<EmissionState> emissions{};

static void show_gas_label(GLib::WindowWithNeuro &window, std::string gas_label);
void show_graph(GLib::WindowWithNeuro &window, EmissionState &state);
void show_error_message(GLib::WindowWithNeuro &window, std::string message);
void show_graph_labels(GLib::WindowWithNeuro &window, std::vector<double> evaluations, int max_speed);

// CALLBACKS
void callback_predict(GLib::Address, GLib::Address addr)
{
  auto *pb = static_cast<GLib::Button *>(addr);
  auto &window = static_cast<GLib::WindowWithNeuro &>(pb->window());

  std::vector<GLib::FunctionStepping *> functions = window.functions; // copy functions

  for (size_t i = 0; i < functions.size(); i++)
    window.detach(*functions[i]); // delete all functions

  EmissionState state = emissions.get_current();
  show_graph(window, state);

  bool validated = window.validate_inboxes() == "";
  if (validated)
  {
    window.add_record_to_journal();
    window.draw_journal_buttons();
  }

  window.redraw();
}

void callback_save(GLib::Address, GLib::Address addr)
{
  auto *pb = static_cast<GLib::Button *>(addr);
  auto &window = static_cast<GLib::WindowWithNeuro &>(pb->window());

  bool validated = window.validate_inboxes() == "";
  if (validated)
  {
    const char *path = fl_file_chooser("Save a file", "*.txt", NULL, 0);

    if (path == nullptr)
      return;

    std::ofstream save_file;
    save_file.open(path);
    if (save_file.is_open())
    {
      std::vector<std::string> inbox_values = window.get_values_from_inboxes();
      for (std::string &value : inbox_values)
        save_file << value << std::endl;
    }
    save_file.close();
    std::cout << "[ACTION] File has been written." << std::endl;
    show_error_message(window, "[ACTION] File has been written.");
  }
  else
  {
    std::cout << "[ERROR] Bad data given to save. Try again." << std::endl;
    show_error_message(window, "[ERROR] Bad data given to save. Try again.");
  }
}

void callback_load(GLib::Address, GLib::Address addr)
{
  auto *pb = static_cast<GLib::Button *>(addr);
  auto &window = static_cast<GLib::WindowWithNeuro &>(pb->window());

  const char *path = fl_file_chooser("Choose a file", "*.txt", NULL, 0);

  if (path == nullptr)
    return;

  std::ifstream save_file(path);
  if (!save_file.is_open())
  {
    std::cerr << "[ERROR] History file not found" << std::endl;
    show_error_message(window, "[ERROR] History file not found");
    return;
  }
  std::string file_contents(std::istreambuf_iterator<char>{save_file}, {});
  save_file.close();

  std::regex file_regex_wrapped(file_regex, std::regex_constants::icase);
  if (!std::regex_match(file_contents.begin(), file_contents.end(), file_regex_wrapped))
  {
    std::cerr << "[ERROR] History file is not in the correct format" << std::endl;
    show_error_message(window, "[ERROR] History file is not in the correct format");
    return;
  }

  std::vector<std::string> inbox_values;
  std::regex del("\n");
  std::sregex_token_iterator token_iterator(file_contents.begin(), file_contents.end(), del, -1);
  std::sregex_token_iterator end;

  for (; token_iterator != end; ++token_iterator)
    inbox_values.push_back(*token_iterator);

  std::vector<GLib::In_box *> inboxes = window.inboxes; // copy inboxes

  for (size_t i = 0; i < inboxes.size(); i++)
    inboxes[i]->set_string(inbox_values[i]);

  std::cout << "[ACTION] Data pasted successfully." << std::endl;
  show_error_message(window, "[ACTION] Data pasted successfully.");
}

void callback_clear(GLib::Address, GLib::Address addr)
{
  auto *pb = static_cast<GLib::Button *>(addr);
  auto &window = static_cast<GLib::WindowWithNeuro &>(pb->window());

  window.graph_points = {}; // clear graph points

  window.end_label_y->set_label("Emissions, g/km"); // clear y axis label

  std::vector<GLib::Line *> axis_labels = window.axis_labels; // copy axis lines

  for (size_t i = 0; i < axis_labels.size(); i++)
    window.detach(*axis_labels[i]); // delete axis lines

  std::vector<GLib::FunctionStepping *> functions = window.functions; // copy functions

  for (size_t i = 0; i < functions.size(); i++)
    window.detach(*functions[i]); // delete all functions

  std::vector<GLib::GasText *> gas_texts = window.gas_texts; // copy gas_texts

  for (size_t i = 0; i < gas_texts.size(); i++)
    window.detach(*gas_texts[i]); // delete all gas_texts

  std::vector<GLib::Widget *> widgets = window.widgets; // copy widgets

  for (size_t i = 0; i < widgets.size(); i++)
    window.detach(*widgets[i]); // delete all widgets

  for (size_t i = 0; i < widgets.size(); i++)
    window.attach(*widgets[i]); // restore all widgets

  reinterpret_cast<GLib::Box *>(window.widgets.back())->set_color(COLORS::WHITE); // restore cursor_box state. In fact always the last widget
  reinterpret_cast<GLib::Box *>(window.widgets.back())->box(FL_BORDER_BOX);
  reinterpret_cast<GLib::Box *>(window.widgets.back())->hide();

  window.redraw();
  std::cout << "[ACTION] Shapes and input boxes have been cleared." << std::endl;
  show_error_message(window, "[ACTION] Shapes and input boxes have been cleared.");
}

std::string help_message 
{
    "The Emission Prediction Program is designed to predict vehicle " \
    "emissions (CO2, NOX, PM, VOC, SO2)\nbased on various input parameters " \
    "using a neural network. The program features a graphical user\ninterface (GUI) " \
    "that allows users to input vehicle data, visualize predictions, and manage historical data.\n\n" \
    "Input parameters: vehicle type (Car, Truck, Motorcycle, Bus), fuel type (Petrol, Diesel, Electric, Hybrid),\n" \
    "engine size (in liters), age of vehicle (in years), mileage (in kilometers)," \
    "acceleration (m/s²),\nroad type (City, Rural, Highway), traffic conditions " \
    "(Heavy, Moderate, Free flow),\ntemperature (in °C), humidity (relative in %), " \
    "wind speed (m/s) and air pressure (in hPa).\n\nThen you need to input the max speed value. " \
    "It is the limit up to which the graph will be drawn.\n\n" \
    "There is few buttons: under parameters table there are buttons to predict, save, load and clear.\n" \
    " - There is also a help button to show this message.\n" \
    " - Button 'predict' will show the prediction graph.\n" \
    " - Button 'save' will save the current parameters and the prediction in a file.\n" \
    " - Button 'load' will load the parameters and the prediction from a file.\n" \
    " - Button 'clear' will clear the parameters and the prediction.\n" \
    " - Button 'journal' will expend the window and show last configurations.\n" \
    "There is also two buttons to navigate through the historical data: " \
    "'<' and '>'.\nThey are located at the right top of the graph canvas.\n\n" \
    "This program provides a user-friendly interface for predicting vehicle " \
    "emissions using neural\nnetwork techniques. By following the " \
    "instructions above, users can effectively use the program to\n" \
    "analyze and visualize emissions data."
};

void show_help_message()
{
    GLib::Window *win = new GLib::Window
    {
      GLib::Point(
        (GLib::x_max() - help_window_width) / 2,
        (GLib::y_max() - help_window_height) / 2
      ),
      help_window_width, help_window_height,
      "Instruction"
    };
    win->begin();

    win->color(FL_WHITE);

    Fl_Box *box = new Fl_Box(10, 10, help_window_width-20, help_window_height-20, help_message.c_str());
    box->labelsize(13);
    box->align(FL_ALIGN_TOP_LEFT | FL_ALIGN_INSIDE);
  
    win->end();
    win->show();
}

void callback_help(GLib::Address, GLib::Address addr)
{
  auto *pb = static_cast<GLib::Button *>(addr);
  auto &window = static_cast<GLib::WindowWithNeuro &>(pb->window());

  std::cout << "[ACTION] Help window has been called." << std::endl;
  show_help_message();
}

static void callback_next(GLib::Address, GLib::Address addr)
{
  auto *pb = static_cast<GLib::Button *>(addr);
  auto &window = static_cast<GLib::WindowWithNeuro &>(pb->window());

  std::vector<GLib::FunctionStepping *> functions = window.functions; // copy functions

  for (size_t i = 0; i < functions.size(); i++)
    window.detach(*functions[i]); // delete all functions

  EmissionState state = emissions.get_next();
  show_graph(window, state);

  window.redraw();
}

static void callback_prev(GLib::Address, GLib::Address addr)
{
  auto *pb = static_cast<GLib::Button *>(addr);
  auto &window = static_cast<GLib::WindowWithNeuro &>(pb->window());

  std::vector<GLib::FunctionStepping *> functions = window.functions; // copy functions

  for (size_t i = 0; i < functions.size(); i++)
    window.detach(*functions[i]); // delete all functions

  EmissionState state = emissions.get_previous();
  show_graph(window, state);

  window.redraw();
}

static void callback_journal(GLib::Address, GLib::Address addr)
{
  auto *pb = static_cast<GLib::Button *>(addr);
  auto &window = static_cast<GLib::WindowWithNeuro &>(pb->window());

  if (window.get_w() == window_width)
  {
    window.resize(window_width_journal, window_height);
  }
  else
    window.resize(window_width, window_height);

  window.redraw();
}

static void show_gas_label(GLib::WindowWithNeuro &window, std::string gas_label)
{
  unsigned int gas_label_x{graph_canvas_x + graph_canvas_w / 2 - gas_label.size() * 4 + 20}, gas_label_y{next_gas_y + 20};

  std::vector<GLib::GasText *> gas_texts = window.gas_texts; // copy gas_texts

  for (size_t i = 0; i < gas_texts.size(); i++)
    window.detach(*gas_texts[i]); // delete all gas_texts

  GLib::GasText *gas = new GLib::GasText{GLib::Point(gas_label_x, gas_label_y), gas_label};
  gas->set_color(COLORS::BLACK);
  gas->set_font(FL_HELVETICA);
  gas->set_font_size(16);

  window.attach(*gas);
}

void show_graph(GLib::WindowWithNeuro &window, EmissionState &state)
{
  std::string error_message = window.validate_inboxes();
  bool validated = error_message == "";
  if (validated)
  {
    show_gas_label(window, state.gas_label);
    window.update_current_cell();
    int max_speed = window.current_cell.speed;
    std::vector<double> evaluations;

    evaluations = window.evaluate_network(state.gas_tag);

    fl_color(state.graph_color);
    GLib::FunctionStepping *func = new GLib::FunctionStepping{
        evaluations, 0, window.current_cell.speed, GLib::Point(canvas_origin_x, canvas_origin_y)};

    window.graph_points = func->points;
    window.graph_evaluations = evaluations;

    show_graph_labels(window, evaluations, max_speed);

    window.attach(*func);
  }
  else
  {
    show_error_message(window, error_message);
  }
}

void show_graph_labels(GLib::WindowWithNeuro &window, std::vector<double> evaluations, int max_speed)
{

  std::vector<GLib::Line *> axis_labels = window.axis_labels; // copy axis lines

  for (size_t i = 0; i < axis_labels.size(); i++)
    window.detach(*axis_labels[i]); // delete axis lines

  double max_graph = *std::max_element(evaluations.begin(), evaluations.end());
  double min_graph = *std::min_element(evaluations.begin(), evaluations.end());

  // generate x and y label numbers
  std::vector<unsigned int> graph_labels_x;
  std::vector<double> graph_labels_y;

  for (size_t i = 0; i < graph_labels_steps.size(); ++i)
  {
    if ((double)max_speed / (double)graph_labels_steps[i] <= max_num_of_graph_labels) // find lowest beautiful step
    {
      for (size_t j = 0; j <= (double)max_speed / (double)graph_labels_steps[i]; ++j)
      {
        graph_labels_x.push_back(graph_labels_steps[i] * j);
      }
      break;
    }
  }
  for (size_t i = 0; i < graph_labels_x.size(); ++i)
  {
    graph_labels_y.push_back((double)min_graph + (double)(max_graph - min_graph) / (double)graph_labels_x.size() * (double)i);
  }

  // marks and lines
  fl_color(COLORS::BLACK);
  std::vector<GLib::GasText *> graph_labels;

  // x
  for (size_t i = 1; i < graph_labels_x.size(); ++i)
  {
    fl_color(COLORS::LIGHT_GRAY);
    GLib::Point origin_point_low_graph{canvas_origin_x + int((double)(graph_canvas_x + graph_canvas_w - canvas_origin_x - (graph_canvas_w * 0.1)) / (double)max_speed * (double)graph_labels_x[i]), canvas_origin_y};
    GLib::Point origin_point_high_graph{canvas_origin_x + int((double)(graph_canvas_x + graph_canvas_w - canvas_origin_x - (graph_canvas_w * 0.1)) / (double)max_speed * (double)graph_labels_x[i]), canvas_origin_y - int(canvas_origin_y - graph_canvas_y)};
    GLib::Line *axis_line = new GLib::Line{origin_point_low_graph, origin_point_high_graph};
    axis_line->set_style(1);
    window.attach(*axis_line);
    fl_color(COLORS::BLACK);

    GLib::Point origin_point_low{canvas_origin_x + int((double)(graph_canvas_x + graph_canvas_w - canvas_origin_x - (graph_canvas_w * 0.1)) / (double)max_speed * (double)graph_labels_x[i]), canvas_origin_y + 5};
    GLib::Point origin_point_high{canvas_origin_x + int((double)(graph_canvas_x + graph_canvas_w - canvas_origin_x - (graph_canvas_w * 0.1)) / (double)max_speed * (double)graph_labels_x[i]), canvas_origin_y - 5};
    window.attach(*(new GLib::Line(origin_point_low, origin_point_high)));
  }
  // y
  for (size_t i = 1; i < graph_labels_x.size(); ++i)
  {
    fl_color(COLORS::LIGHT_GRAY);
    GLib::Point origin_point_left_graph{canvas_origin_x, canvas_origin_y - int((double)(canvas_origin_y - graph_canvas_y - (graph_canvas_h * 0.1)) / (double)graph_labels_x.size() * (double)i)};
    GLib::Point origin_point_right_graph{canvas_origin_x + int(graph_canvas_x + graph_canvas_w - canvas_origin_x), canvas_origin_y - int((double)(canvas_origin_y - graph_canvas_y - (graph_canvas_h * 0.1)) / (double)graph_labels_x.size() * (double)i)};
    GLib::Line *axis_line = new GLib::Line{origin_point_left_graph, origin_point_right_graph};
    axis_line->set_style(1);
    window.attach(*axis_line);
    fl_color(COLORS::BLACK);

    GLib::Point origin_point_left{canvas_origin_x - 5, canvas_origin_y - int((double)(canvas_origin_y - graph_canvas_y - (graph_canvas_h * 0.1)) / (double)graph_labels_x.size() * (double)i)};
    GLib::Point origin_point_right{canvas_origin_x + 5, canvas_origin_y - int((double)(canvas_origin_y - graph_canvas_y - (graph_canvas_h * 0.1)) / (double)graph_labels_x.size() * (double)i)};
    window.attach(*(new GLib::Line(origin_point_left, origin_point_right)));
  }

  // numbers
  // x
  for (size_t i = 0; i < graph_labels_x.size(); ++i)
  {
    std::string value = std::to_string(graph_labels_x[i]);
    GLib::GasText *current_text = new GLib::GasText{GLib::Point{0, 0}, value};
    GLib::Point origin_point{((i != 0) ? (-int(current_text->get_size() * 1.5) / 2) : (0)) + canvas_origin_x + 5 + int((double)(graph_canvas_x + graph_canvas_w - canvas_origin_x - (graph_canvas_w * 0.1)) / (double)max_speed * (double)graph_labels_x[i]), canvas_origin_y + 21};
    current_text->move(origin_point.x, origin_point.y);
    graph_labels.push_back(current_text);
  }
  // y
  for (size_t i = 0; i < graph_labels_x.size(); ++i)
  {
    GLib::Point origin_point{canvas_origin_x - 67, ((i != 0) ? (10) : (0)) + canvas_origin_y - 5 - int((double)(canvas_origin_y - graph_canvas_y - (graph_canvas_h * 0.1)) / (double)graph_labels_x.size() * (double)i)};
    std::string value = to_string_exp(graph_labels_y[i]);
    if (i == graph_labels_x.size())
      window.end_label_y->set_label("Emissions, g/km (10^" + std::to_string(std::stoi(value.substr(value.find("e") + 1, value.size()))) + ")");
    graph_labels.push_back(new GLib::GasText{origin_point, value.substr(0, value.find("e"))});
  }

  for (size_t i = 0; i < graph_labels.size(); ++i)
    window.attach(*graph_labels[i]);
}

void show_error_message(GLib::WindowWithNeuro &window, std::string message)
{
  fl_alert(message.c_str());
  window.reset_inboxes_colors();
}

int main_gui()
try
{
  emissions.insert(EmissionState("CO2 Emissions", CO2, COLORS::BLACK));
  emissions.insert(EmissionState("NOx Emissions", NOX, COLORS::BRIGHT_BLUE));
  emissions.insert(EmissionState("PM2.5 Emissions", PM25, COLORS::RED_DARK_BERRY));
  emissions.insert(EmissionState("VOC Emissions", VOC, COLORS::GREEN_DARK_LEMON));
  emissions.insert(EmissionState("SO2 Emissions", SO2, COLORS::ROYAL_GOLD));

  // CREATING MAIN WINDOW
  GLib::WindowWithNeuro win{
      window_width, window_height,
      main_window_title, COLORS::LIGHT_GRAY};

  win.update_journal(
    journal_x, journal_y,
    journal_width, journal_height
  );

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
  GLib::Button load_button{
      GLib::Point(load_button_x, load_button_y),
      button_w, button_h,
      load_button_label,
      callback_load};
  win.attach(load_button);

  // CLEAR BUTTON
  GLib::Button clear_button{
      GLib::Point(clear_button_x, clear_button_y),
      button_w, button_h,
      clear_button_label,
      callback_clear};
  win.attach(clear_button);

  GLib::Button help_button{
      GLib::Point(help_button_x, help_button_y),
      button_w, button_h,
      help_button_label,
      callback_help};
  win.attach(help_button);

  // GRAPH AREA (WHITE BOX WITH BLACK AXISES AND LIGHT GRAY GRID)
  GLib::GraphCanvas graph_canvas{
      GLib::Point(graph_canvas_x, graph_canvas_y),
      graph_canvas_w, graph_canvas_h,
      GLib::Point(canvas_origin_x, canvas_origin_y)};
  win.attach(graph_canvas);

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

  GLib::Button journal_button{
      GLib::Point(journal_button_x, journal_button_y),
      button_w, button_h,
      journal_button_label,
      callback_journal};
  win.attach(journal_button);

  //INBOXES
  for (size_t i = 0; i < table_rows - 1; ++i)
  {
    if (i == 0)
    {
      GLib::Choose_In_box *in_box = new GLib::Choose_In_box{
          GLib::Point(inbox_x, inbox_y + (inbox_h + 1) * i),
          inbox_w, inbox_h, ""};
      in_box->add("Truck");
      in_box->add("Car");
      in_box->add("Motorcycle");
      in_box->add("Bus");
      win.attach(*in_box);
    }
    else if (i == 1)
    {
      GLib::Choose_In_box *in_box = new GLib::Choose_In_box{
          GLib::Point(inbox_x, inbox_y + (inbox_h + 1) * i),
          inbox_w, inbox_h, ""};
      in_box->add("Petrol");
      in_box->add("Electric");
      in_box->add("Diesel");
      in_box->add("Hybrid");
      win.attach(*in_box);
    }
    else if (i == 6)
    {
      GLib::Choose_In_box *in_box = new GLib::Choose_In_box{
          GLib::Point(inbox_x, inbox_y + (inbox_h + 1) * i),
          inbox_w, inbox_h, ""};
      in_box->add("City");
      in_box->add("Highway");
      in_box->add("Rural");
      win.attach(*in_box);
    }
    else if (i == 7)
    {
      GLib::Choose_In_box *in_box = new GLib::Choose_In_box{
          GLib::Point(inbox_x, inbox_y + (inbox_h + 1) * i),
          inbox_w, inbox_h, ""};
      in_box->add("Free flow");
      in_box->add("Heavy");
      in_box->add("Moderate");
      win.attach(*in_box);
    }
    else
    {
      GLib::In_box *in_box =  new GLib::In_box{
          GLib::Point(inbox_x, inbox_y + (inbox_h + 1) * i),
          inbox_w, inbox_h, ""};
      win.attach(*in_box);
    }
  }

  GLib::Text *end_label_y = new GLib::Text{GLib::Point(graph_canvas_x + 22, graph_canvas_y + 20), "Emissions, g/km"};
  win.end_label_y = end_label_y;
  win.attach(*end_label_y);

  GLib::Text end_label_x{GLib::Point(graph_canvas_x + graph_canvas_w - 85, graph_canvas_y + graph_canvas_h - 55), "Speed, km/h"};
  win.attach(end_label_x);

  win.load_networks();

  GLib::Box *cursor_box = new GLib::Box(GLib::Point(0, 0), 170, 35, "");
  win.attach(*cursor_box);
  cursor_box->set_color(COLORS::WHITE);
  cursor_box->box(FL_BORDER_BOX);
  cursor_box->hide();

#if (defined(_WIN32) || defined(_WIN64))
  while (true)
  {
    POINT p;
    if (GetCursorPos(&p))
    {
      int window_x = win.x_root();
      int window_y = win.y_root();
      int x = p.x - window_x;
      int y = p.y - window_y;

      for (size_t i = 0; i < win.graph_points.size(); ++i)
      {
        GLib::Point current_point = win.graph_points[i];

        if (x - 6 <= current_point.x && x + 6 >= current_point.x && y - 6 <= current_point.y && y + 6 >= current_point.y)
        {
          double graph_evaluation = win.graph_evaluations[i];
          std::string label_string = "Speed: " + std::to_string(i) + "\nEmissions: " + to_string_exp(graph_evaluation);
          cursor_box->position(GLib::Point(x, y - 35));
          if (label_string != cursor_box->get_label())
            cursor_box->set_label(label_string);
          cursor_box->show();
          break;
        }
        else
        {
          cursor_box->hide();
        }
      }
    }
    else
      throw std::runtime_error("can't fetch cursor");
    if (!win.shown())
    {
      return 0;
    }
    win.redraw();
    Fl::wait();
  }
#else
  return Fl::run();
#endif
}
catch (std::exception &e)
{
  std::cerr << e.what() << std::endl;
  return 1;
}
catch (...)
{
  std::cerr << "Unknown exception." << std::endl;
  return 2;
}