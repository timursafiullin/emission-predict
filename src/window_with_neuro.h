#ifndef WINDOW_WITH_NEURO_H
#define WINDOW_WITH_NEURO_H 1

#include "Network.h"
#include "graph_lib.h"
#include "dataset_reader.h"
#include "gui.h"
#include <vector>
#include <regex>
#include <math.h>
#include <functional>

#define GLib Graph_lib


namespace Graph_lib
{
    constexpr long double min_engine_size                               { 0.6     };
    constexpr long double max_engine_size                               { 6.0     };
    constexpr long double max_acceleration                              { 5.0     };
    constexpr long double min_acceleration                              { 0.0     };
    constexpr long double min_temperature                               { -10.0   };
    constexpr long double max_temperature                               { 40.0    };
    constexpr long double min_air_pressure                              { 950.0   };
    constexpr long double max_air_pressure                              { 1050.0  };
    constexpr long double max_mileage                                   { 3e5     };
    constexpr long double min_mileage                                   { 0.0     };
    constexpr long double max_age_of_vehicle                            { 30.0    };
    constexpr long double min_age_of_vehicle                            { 0.0     };
    constexpr long double max_wind_speed                                { 20.0    };
    constexpr long double min_wind_speed                                { 0.0     };
    constexpr long double max_humidity                                  { 100.0   };

    static const std::string bad_graphing_range_error_message           { "bad graphing range"          };
    static const std::string non_positive_graphing_count_error_message  { "non-positive graphing count" };

    static const std::string invalid_traffic_conditions_error_message {
        "[ERROR] Invalid value of 'Traffic conditions': must be Heavy, Moderate or Free flow.\n"
        };
    static const std::string invalid_road_type_error_message {
        "[ERROR] Invalid value of 'Road type': must be City, Highway or Rural.\n"
        };
    static const std::string invalid_fuel_type_error_message {
        "[ERROR] Invalid value of 'Fuel type': must be Petrol, Diesel, Electric or Hybrid.\n"
        };
    static const std::string invalid_vehicle_type_error_message {
        "[ERROR] Invalid value of 'Vehicle type': must be Car, Truck, Bus or Motorcycle\n"
        };
    static const std::string all_empty_inboxes_error_message {
        "[ERROR] All inboxes are empty.\n"
        };
    static const std::string input_box_error_message {
            "[ERROR] Input box '"
        };
    static const std::string is_empty_error_message_end {
            "' is empty.\n"
        };
    static const std::string invalid_value_error_message_end {
        "' has invalid value.\n"
        };
    static const std::string invalid_max_speed_error_message {
            "[ERROR] Invalid value of 'Max speed': must be greater than "
        };
    static const std::string invalid_engine_size_value_error_message {
            "[ERROR] Invalid value of 'Engine Size': must be positive and be real number in [" + std::to_string(double(min_engine_size)) + ", " + std::to_string(double(max_engine_size)) + "]\n"
        };
    static const std::string invalid_age_error_message {
            "[ERROR] Invalid value of 'Age of Vehicle': must be positive real number less than " + std::to_string(int(max_age_of_vehicle)) + "\n"
        };
    static const std::string invalid_mileage_error_message {
            "[ERROR] Invalid value of 'Mileage': must be positive real number less than " + std::to_string(int(max_mileage)) + "\n"
        };
    static const std::string invalid_acceleration_error_message {
            "[ERROR] Invalid value of 'Acceleration': must be positive and be real number in [0, " + std::to_string(int(max_acceleration)) + "]\n"
        };
    static const std::string invalid_temperature_error_message {
            "[ERROR] Invalid value of 'Temperature': must be real number in [" + std::to_string(double(min_temperature)) + ", " + std::to_string(double(max_temperature)) + "]\n"
        };
    static const std::string invalid_humidity_error_message {
            "[ERROR] Invalid value of 'Humidity': must be real number in [0, " + std::to_string(int(max_humidity)) + "]\n"
        };
    static const std::string invalid_wind_speed_error_message {
            "[ERROR] Invalid value of 'Wind Speed': must be real number in [0, " + std::to_string(int(max_wind_speed)) + "]\n"
        };
    static const std::string invalid_pressure_error_message {
            "[ERROR] Invalid value of 'Air Pressure': must be real number in [" + std::to_string(int(min_air_pressure)) + ", " + std::to_string(int(max_air_pressure)) + "]\n"
        };
    static const std::string line_separator {
            "----------------------------------------------\n"
        };

    constexpr unsigned long long amount_of_inputs {13};
    constexpr unsigned long long amount_of_hidden_neurons {13};
    constexpr unsigned long long amount_of_outputs {1};

    static const std::string CO2_filepath { "../weights/weights_CO2.csv" };
    static const std::string NOx_filepath { "../weights/weights_NOX.csv" };
    static const std::string PM_filepath  { "../weights/weights_PM.csv" };
    static const std::string VOC_filepath { "../weights/weights_VOC.csv" };
    static const std::string SO2_filepath { "../weights/weights_SO2.csv" };

    enum NetworkTags
    {
        CO2_tag = 'C',
        NOx_tag = 'N',
        PM_tag = 'P',
        VOC_tag = 'V',
        SO2_tag = 'S',
    };

    enum InboxIndexes
    {
        vehicle_type_index,
        fuel_type_index,
        engine_size_index,
        age_of_vehicle_index,
        mileage_index,
        acceleration_index,
        road_type_index,
        traffic_conditions_index,
        temperature_index,
        humidity_index,
        wind_speed_index,
        air_pressure_index,
        max_speed_index
    };

    class FunctionStepping : public GLib::Shape
    {
    public:
        FunctionStepping(std::vector<double> points, double r1, double r2, Point xy, double xscale = 25, double yscale = 25)
        {
            int count = points.size();

            double max_graph = *std::max_element(points.begin(), points.end());
            double min_graph = *std::min_element(points.begin(), points.end());

            xscale = (double)(graph_canvas_x + graph_canvas_w - canvas_origin_x - (graph_canvas_w * 0.1)) / (double)count;
            yscale = (double)(canvas_origin_y - graph_canvas_y - (graph_canvas_h * 0.1)) / (double)(max_graph - min_graph);

            if (r2 - r1 <= 0)
                error(bad_graphing_range_error_message);
            if (count <= 0)
                error(non_positive_graphing_count_error_message);
            double dist = (r2 - r1) / count;
            double r = r1;
            for (int i = 0; i < count; ++i)
            {
                add(Point{xy.x + int(r * xscale), xy.y - int((points[i] - min_graph) * yscale)});
                r += dist;
            }
        };
    };

    class GasText : public GLib::Text
    {
    public:
        GasText(Point x, const std::string &s) : GLib::Text{x, s} {};
    };

    class ErrorWindow : public GLib::Window
    {
    public:
        ErrorWindow(std::string error_message) : GLib::Window{GLib::Point((GLib::x_max() - 1) / 2, (GLib::y_max() - 1) / 2), 1, 1, "Error message"},
                                                 error_message{error_message}
        {
            std::vector<std::string> parsed_error_message;
            std::regex del("\n");
            std::sregex_token_iterator token_iterator(error_message.begin(), error_message.end(), del, -1);
            std::sregex_token_iterator i_end;

            for (; token_iterator != i_end; ++token_iterator)
                parsed_error_message.push_back(*token_iterator);

            hh = parsed_error_message.size() * 40; // each line is 40 pixels in height
            ww = (*std::max_element(parsed_error_message.begin(), parsed_error_message.end(),
                                        [](const auto &a, const auto &b)
                                        {
                                            return a.size() < b.size();
                                        })).size() * 7; // each symbol is 7 pixels wide

            resize(ww, hh);

            begin();
            color(FL_WHITE);
            Fl_Box *box = new Fl_Box(0, 0, ww, hh, error_message.c_str());
            box->labelsize(14);                            // Увеличим размер шрифта
            box->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE); // Выравнивание по центру и внутри бокса
            end();
            show();
            Fl::run();
        }

    private:
        std::string error_message;
        int hh;
        int ww;
    };

    class WindowWithNeuro : public GLib::Window
    {
    public:
        WindowWithNeuro(int ww, int hh, const std::string &title, Fl_Color background_color = FL_WHITE) : GLib::Window{ww,
                                                                                                                       hh,
                                                                                                                       title,
                                                                                                                       background_color} {};
        std::vector<NeuralNetwork::Number> neuro_structure{
            amount_of_inputs,
            amount_of_hidden_neurons,
            amount_of_hidden_neurons,
            amount_of_hidden_neurons,
            amount_of_hidden_neurons,
            amount_of_hidden_neurons,
            amount_of_hidden_neurons,
            amount_of_hidden_neurons,
            amount_of_hidden_neurons,
            amount_of_outputs};

        NeuralNetwork::FFNN<std::function<long double(long double)>> CO2 {neuro_structure};
        NeuralNetwork::FFNN<std::function<long double(long double)>> PM {neuro_structure};
        NeuralNetwork::FFNN<std::function<long double(long double)>> NOX {neuro_structure};
        NeuralNetwork::FFNN<std::function<long double(long double)>> VOC {neuro_structure};
        NeuralNetwork::FFNN<std::function<long double(long double)>> SO2 {neuro_structure};

        void load_networks()
        {
            CO2.load_weights_from_file(CO2_filepath);
            NOX.load_weights_from_file(NOx_filepath);
            PM.load_weights_from_file(PM_filepath);
            VOC.load_weights_from_file(VOC_filepath);
            SO2.load_weights_from_file(SO2_filepath);
        }

        using Window::attach;
        using Window::detach;

        std::vector<GLib::In_box *> inboxes;
        std::vector<GLib::GasText *> gas_texts;
        std::vector<GLib::FunctionStepping *> functions;
        GLib::Text *end_label_y;

        void attach(GLib::In_box &w)
        {
            inboxes.push_back(&w);
            begin();
            w.attach(*this);
            end();
            widgets.push_back(&w);
        }

        void attach(GLib::GasText &s)
        {
            gas_texts.push_back(&s);
            shapes.push_back(&s);
        }

        void attach(GLib::FunctionStepping &s)
        {
            functions.push_back(&s);
            shapes.push_back(&s);
        }

        void detach(GLib::In_box &w)
        {
            for (unsigned int i = inboxes.size(); 0 < i; --i)
                if (inboxes[i - 1] == &w)
                    inboxes.erase(inboxes.begin() + (i - 1));

            w.hide();
            for (unsigned int i = widgets.size(); 0 < i; --i)
                if (widgets[i - 1] == &w)
                    widgets.erase(widgets.begin() + (i - 1));
        }

        void detach(GLib::GasText &s)
        {
            for (unsigned int i = gas_texts.size(); 0 < i; --i)
                if (gas_texts[i - 1] == &s)
                    gas_texts.erase(gas_texts.begin() + (i - 1));

            for (unsigned int i = shapes.size(); 0 < i; --i)
                if (shapes[i - 1] == &s)
                    shapes.erase(shapes.begin() + (i - 1));
        }

        void detach(GLib::FunctionStepping &s)
        {
            for (unsigned int i = functions.size(); 0 < i; --i)
                if (functions[i - 1] == &s)
                    functions.erase(functions.begin() + (i - 1));

            for (unsigned int i = shapes.size(); 0 < i; --i)
                if (shapes[i - 1] == &s)
                    shapes.erase(shapes.begin() + (i - 1));
        }

        std::vector<std::string> get_values_from_inboxes()
        {
            std::vector<std::string> inbox_values;
            for (size_t i = 0; i < inboxes.size(); ++i)
            {
                std::string inbox_value = inboxes[i]->get_string();
                std::transform(inbox_value.begin(), inbox_value.end(), inbox_value.begin(), ::tolower);
                inbox_values.push_back(inbox_value);
            }
            return inbox_values;
        }

        DatasetCell current_cell;

        NeuralNetwork::RowVector *get_input_ptr(std::vector<long double> vector)
        {
            NeuralNetwork::RowVector *input = new NeuralNetwork::RowVector(amount_of_input_features);
            for (NeuralNetwork::Number i{0}; i < amount_of_input_features; ++i)
            {
                (*input)[i] = vector[i];
            }
            return input;
        }

        NeuralNetwork::RowVector *get_output_CO2_ptr(std::vector<long double> vector)
        {
            NeuralNetwork::RowVector *output = new NeuralNetwork::RowVector(1);
            (*output)[0] = vector[(unsigned long long)Index::CO2];
            return output;
        }

        NeuralNetwork::RowVector *get_output_NOX_ptr(std::vector<long double> vector)
        {
            NeuralNetwork::RowVector *output = new NeuralNetwork::RowVector(1);
            (*output)[0] = vector[(unsigned long long)Index::NOx];
            return output;
        }

        NeuralNetwork::RowVector *get_output_PM_ptr(std::vector<long double> vector)
        {
            NeuralNetwork::RowVector *output = new NeuralNetwork::RowVector(1);
            (*output)[0] = vector[(unsigned long long)Index::PM];
            return output;
        }

        NeuralNetwork::RowVector *get_output_VOC_ptr(std::vector<long double> vector)
        {
            NeuralNetwork::RowVector *output = new NeuralNetwork::RowVector(1);
            (*output)[0] = vector[(unsigned long long)Index::VOC];
            return output;
        }

        NeuralNetwork::RowVector *get_output_SO2_ptr(std::vector<long double> vector)
        {
            NeuralNetwork::RowVector *output = new NeuralNetwork::RowVector(1);
            (*output)[0] = vector[(unsigned long long)Index::SO2];
            return output;
        }

        std::vector<NeuralNetwork::Scalar> turn_CO2_output_to_standart_view(NeuralNetwork::RowVector a)
        {
            std::vector<NeuralNetwork::Scalar> ans(1);
            ans[0] = (std::exp(a[0] / main_normalization_constant * CO2_normalization_constant) - std::exp(23.3)) / 2770000.0 - 100.0;
            return ans;
        }

        std::vector<NeuralNetwork::Scalar> turn_NOX_output_to_standart_view(NeuralNetwork::RowVector a)
        {
            std::vector<NeuralNetwork::Scalar> ans(1);
            ans[0] = (std::exp(a[0] / main_normalization_constant * NOx_normalization_constant - 44.7 / 1000.0));
            return ans;
        }

        std::vector<NeuralNetwork::Scalar> turn_PM_output_to_standart_view(NeuralNetwork::RowVector a)
        {
            std::vector<NeuralNetwork::Scalar> ans(1);
            ans[0] = a[0] / main_normalization_constant * PM_normalization_constant * 4.0;
            return ans;
        }

        std::vector<NeuralNetwork::Scalar> turn_VOC_output_to_standart_view(NeuralNetwork::RowVector a)
        {
            std::vector<NeuralNetwork::Scalar> ans(1);
            ans[0] = a[0] / main_normalization_constant * VOC_normalization_constant * 30.0;
            return ans;
        }

        std::vector<NeuralNetwork::Scalar> turn_SO2_output_to_standart_view(NeuralNetwork::RowVector a)
        {
            std::vector<NeuralNetwork::Scalar> ans(1);
            ans[0] = a[0] / main_normalization_constant * SO2_normalization_constant * 20.0;
            return ans;
        }

        bool is_string_double(const std::string &str)
        {
            for (char const &c : str)
            {
                if (std::isdigit(c) == 0 && c != '.' && c != '-')
                    return false;
            }
            return true;
        }

        bool is_string_int(const std::string &str)
        {
            for (char const &c : str)
            {
                if (std::isdigit(c) == 0)
                    return false;
            }
            return true;
        }

        std::vector<double> evaluate_network(char network)
        {
            int max_speed = current_cell.speed;
            std::vector<double> evaluations;

            for (size_t current_speed = 0; current_speed < max_speed; ++current_speed)
            {
                current_cell.speed = current_speed;
                std::vector<NeuralNetwork::Scalar> ans;
                ans.push_back(0);

                NeuralNetwork::RowVector network_input{*get_input_ptr(current_cell.normalise_data())};
                switch (network)
                {
                case CO2_tag:
                {
                    NeuralNetwork::RowVector pr = CO2.predict(network_input);
                    ans = turn_CO2_output_to_standart_view(pr);
                    break;
                }
                case NOx_tag:
                {
                    NeuralNetwork::RowVector pr = NOX.predict(network_input);
                    ans = turn_NOX_output_to_standart_view(pr);
                    break;
                }
                case PM_tag:
                {
                    NeuralNetwork::RowVector pr = PM.predict(network_input);
                    ans = turn_PM_output_to_standart_view(pr);
                    break;
                }
                case VOC_tag:
                {
                    NeuralNetwork::RowVector pr = VOC.predict(network_input);
                    ans = turn_VOC_output_to_standart_view(pr);
                    break;
                }
                case SO2_tag:
                {
                    NeuralNetwork::RowVector pr = SO2.predict(network_input);
                    ans = turn_SO2_output_to_standart_view(pr);
                    break;
                }
                }
                evaluations.push_back(ans[0]);
            }
            return evaluations;
        }

        std::string validate_inboxes()
        {
            std::vector<std::string> inbox_values = get_values_from_inboxes();
            std::string validated = "";
            std::string message = "";
            message += "\n" + line_separator;

            Labels inbox_names = Labels{std::initializer_list<std::string>
            {
                   "Vehicle Type", "Fuel Type",
                   "Engine Size", "Age of Vehicle", "Mileage",
                   "Acceleration", "Road Type",
                   "Traffic Conditions", "Temperature", "Humidity",
                   "Wind Speed", "Air Pressure", "Max speed"}, context_column, 1
            };

            bool is_all_empty = true;
            for (std::string &name : inbox_values)
            {
                if (name != "") 
                {
                    is_all_empty = false;
                    break;
                }
            }

            if (!is_all_empty)
            {
                for (size_t i{}; i < inbox_values.size()-1; ++i)
                {
                    if (inbox_values[i] == "")
                        validated += input_box_error_message + inbox_names[i] + is_empty_error_message_end;
                    if (i != 0 && i != 1 && i != 6 && i != 7)
                        if (!is_string_double(inbox_values[i]))
                            validated += input_box_error_message + inbox_names[i] + invalid_value_error_message_end;
                }
                if (inbox_values[vehicle_type_index] != truck_type &&
                    inbox_values[vehicle_type_index] != car_type &&
                    inbox_values[vehicle_type_index] != motorcycle_type &&
                    inbox_values[vehicle_type_index] != bus_type
                    )
                    validated += invalid_vehicle_type_error_message;

                if (inbox_values[fuel_type_index] != petrol_type &&
                    inbox_values[fuel_type_index] != electric_type &&
                    inbox_values[fuel_type_index] != diesel_type &&
                    inbox_values[fuel_type_index] != hybrid_type
                    )
                    validated += invalid_fuel_type_error_message;

                if (std::stold(inbox_values[engine_size_index]) < min_engine_size || 
                    std::stold(inbox_values[engine_size_index]) > max_engine_size 
                    )
                    validated += invalid_engine_size_value_error_message;

                if (std::stold(inbox_values[age_of_vehicle_index]) < min_age_of_vehicle ||
                    std::stold(inbox_values[age_of_vehicle_index]) > max_age_of_vehicle
                    )
                    validated += invalid_age_error_message;

                if (std::stold(inbox_values[mileage_index]) < min_mileage ||
                    std::stold(inbox_values[mileage_index]) > max_mileage
                    )
                    validated += invalid_mileage_error_message;

                if (std::stold(inbox_values[acceleration_index]) < min_acceleration ||
                    std::stold(inbox_values[acceleration_index]) > max_acceleration
                    )
                    validated += invalid_acceleration_error_message;

                if (
                    inbox_values[road_type_index] != city_type &&
                    inbox_values[road_type_index] != highway_type &&
                    inbox_values[road_type_index] != rural_type
                    )
                    validated += invalid_road_type_error_message;

                if (
                    inbox_values[traffic_conditions_index] != free_flow_type &&
                    inbox_values[traffic_conditions_index] != heavy_type &&
                    inbox_values[traffic_conditions_index] != moderate_type
                    )
                    validated += invalid_traffic_conditions_error_message;

                if (std::stold(inbox_values[temperature_index]) < min_temperature ||
                    std::stold(inbox_values[temperature_index]) > max_temperature
                    )
                    validated += invalid_temperature_error_message;
                
                if (std::stold(inbox_values[humidity_index]) < 0 ||
                    std::stold(inbox_values[humidity_index]) > max_humidity
                    )
                    validated += invalid_humidity_error_message;
                
                if (std::stold(inbox_values[wind_speed_index]) < min_wind_speed ||
                    std::stold(inbox_values[wind_speed_index]) > max_wind_speed
                    )
                    validated += invalid_wind_speed_error_message;
                
                if (std::stold(inbox_values[air_pressure_index]) < min_air_pressure ||
                    std::stold(inbox_values[air_pressure_index]) > max_air_pressure
                    )
                    validated += invalid_pressure_error_message;

                if (inbox_values[12] != "" && is_string_double(inbox_values[12]))
                {
                    if (int(std::stold(inbox_values[12])) < num_of_graph_labels_x)
                        validated += invalid_max_speed_error_message + std::to_string(num_of_graph_labels_x) + ".\n";
                }
                else
                    validated += invalid_max_speed_error_message + std::to_string(num_of_graph_labels_x) + ".\n";
            }
            else
                validated += all_empty_inboxes_error_message;
            message += validated;
            message += line_separator;
            if (validated != "")
                std::cout << message << std::endl;
            return validated;
        }

        void update_current_cell()
        {
            std::vector<std::string> inbox_values = get_values_from_inboxes();

            current_cell.vehicle_type = inbox_values[0];
            current_cell.fuel_type = inbox_values[1];
            current_cell.engine_size = (is_string_double(inbox_values[2])) ? (std::stold(inbox_values[2])) : (NAN);
            current_cell.age_of_vehicle = (is_string_int(inbox_values[3])) ? (std::stoul(inbox_values[3])) : (NAN);
            current_cell.mileage = (is_string_int(inbox_values[4])) ? (std::stoull(inbox_values[4])) : (NAN);
            current_cell.acceleration = (is_string_double(inbox_values[5])) ? (std::stold(inbox_values[5])) : (NAN);
            current_cell.road_type = inbox_values[6];
            current_cell.traffic_conditions = inbox_values[7];
            current_cell.temperature = (is_string_double(inbox_values[8])) ? (std::stold(inbox_values[8])) : (NAN);
            current_cell.humidity = (is_string_double(inbox_values[9])) ? (std::stold(inbox_values[9])) : (NAN);
            current_cell.wind_speed = (is_string_double(inbox_values[10])) ? (std::stold(inbox_values[10])) : (NAN);
            current_cell.air_pressure = (is_string_double(inbox_values[11])) ? (std::stold(inbox_values[11])) : (NAN);
            current_cell.speed = (is_string_double(inbox_values[12])) ? (std::stold(inbox_values[12])) : (NAN);
        }
    };
}
#endif