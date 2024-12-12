#ifndef WINDOW_WITH_NEURO_H
#define WINDOW_WITH_NEURO_H 1

#include "Network.h"
#include "graph_lib.h"
#include "dataset_reader.h"
#include "gui.h"
#include <vector>
#include <regex>
#include <math.h>

#define GLib Graph_lib

namespace Graph_lib
{
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
                error("bad graphing range");
            if (count <= 0)
                error("non-positive graphing count");
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

    class WindowWithNeuro : public GLib::Window
    {
    public:
        WindowWithNeuro(int ww, int hh, const std::string &title, Fl_Color background_color = FL_WHITE) : GLib::Window{ww,
                                                                                                                       hh,
                                                                                                                       title,
                                                                                                                       background_color} {};
        std::vector<NeuralNetwork::Number> neuro_structure{
            13,
            13,
            13,
            13,
            13,
            13,
            13,
            13,
            13,
            1};

        NeuralNetwork::Scalar l{1e-30};

        NeuralNetwork::NeuralNetwork CO2{neuro_structure, l};
        NeuralNetwork::NeuralNetwork NOX{neuro_structure, l};
        NeuralNetwork::NeuralNetwork PM{neuro_structure, l};
        NeuralNetwork::NeuralNetwork VOC{neuro_structure, l};
        NeuralNetwork::NeuralNetwork SO2{neuro_structure, l};

        void load_networks()
        {
            CO2.load_weights_from_file("../weights/weights_CO2.csv");
            NOX.load_weights_from_file("../weights/weights_NOX.csv");
            PM.load_weights_from_file("../weights/weights_PM.csv");
            VOC.load_weights_from_file("../weights/weights_VOC.csv");
            SO2.load_weights_from_file("../weights/weights_SO2.csv");
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

        void reset_inboxes_colors()
        {
            for (size_t i = 0; i < inboxes.size(); ++i)
            {
                inboxes[i]->set_color(COLORS::WHITE);
            }
            std::cout << "a" << std::endl;
            redraw();
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
            NeuralNetwork::RowVector *input = new NeuralNetwork::RowVector(13);
            for (NeuralNetwork::Number i{0}; i < 13; ++i)
            {
                (*input)[i] = vector[i];
            }
            return input;
        }

        NeuralNetwork::RowVector *get_output_CO2_ptr(std::vector<long double> vector)
        {
            NeuralNetwork::RowVector *output = new NeuralNetwork::RowVector(1);
            (*output)[0] = vector[13];
            return output;
        }

        NeuralNetwork::RowVector *get_output_NOX_ptr(std::vector<long double> vector)
        {
            NeuralNetwork::RowVector *output = new NeuralNetwork::RowVector(1);
            (*output)[0] = vector[14];
            return output;
        }

        NeuralNetwork::RowVector *get_output_PM_ptr(std::vector<long double> vector)
        {
            NeuralNetwork::RowVector *output = new NeuralNetwork::RowVector(1);
            (*output)[0] = vector[15];
            return output;
        }

        NeuralNetwork::RowVector *get_output_VOC_ptr(std::vector<long double> vector)
        {
            NeuralNetwork::RowVector *output = new NeuralNetwork::RowVector(1);
            (*output)[0] = vector[16];
            return output;
        }

        NeuralNetwork::RowVector *get_output_SO2_ptr(std::vector<long double> vector)
        {
            NeuralNetwork::RowVector *output = new NeuralNetwork::RowVector(1);
            (*output)[0] = vector[17];
            return output;
        }

        std::vector<NeuralNetwork::Scalar> turn_CO2_output_to_standart_view(NeuralNetwork::RowVector a)
        {
            std::vector<NeuralNetwork::Scalar> ans(1);
            ans[0] = (std::exp(a[0] / 6 * 500) - std::exp(23.3)) / 2770000 - 100;
            return ans;
        }

        std::vector<NeuralNetwork::Scalar> turn_NOX_output_to_standart_view(NeuralNetwork::RowVector a)
        {
            std::vector<NeuralNetwork::Scalar> ans(1);
            ans[0] = (std::exp(a[0] / 6 * 2 - 44.7 / 1000));
            return ans;
        }

        std::vector<NeuralNetwork::Scalar> turn_PM_output_to_standart_view(NeuralNetwork::RowVector a)
        {
            std::vector<NeuralNetwork::Scalar> ans(1);
            ans[0] = a[0] / 6 * 0.2 * 4;
            return ans;
        }

        std::vector<NeuralNetwork::Scalar> turn_VOC_output_to_standart_view(NeuralNetwork::RowVector a)
        {
            std::vector<NeuralNetwork::Scalar> ans(1);
            ans[0] = a[0] / 6 * 0.1 * 30;
            return ans;
        }

        std::vector<NeuralNetwork::Scalar> turn_SO2_output_to_standart_view(NeuralNetwork::RowVector a)
        {
            std::vector<NeuralNetwork::Scalar> ans(1);
            ans[0] = a[0] / 6 * 0.1 * 20;
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
                case 'C':
                {
                    NeuralNetwork::RowVector pr = CO2.predict(network_input);
                    ans = turn_CO2_output_to_standart_view(pr);
                    break;
                }
                case 'N':
                {
                    NeuralNetwork::RowVector pr = NOX.predict(network_input);
                    ans = turn_NOX_output_to_standart_view(pr);
                    break;
                }
                case 'P':
                {
                    NeuralNetwork::RowVector pr = PM.predict(network_input);
                    ans = turn_PM_output_to_standart_view(pr);
                    break;
                }
                case 'V':
                {
                    NeuralNetwork::RowVector pr = VOC.predict(network_input);
                    ans = turn_VOC_output_to_standart_view(pr);
                    break;
                }
                case 'S':
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
            message += "\n----------------------------------------------\n";

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
                    {
                        inboxes[i]->set_color(COLORS::SOFT_PINK);
                        validated += "[ERROR] Input box '" + inbox_names[i] + "' is empty.\n";
                    }
                    if (i != 0 && i != 1 && i != 6 && i != 7)
                        if (!is_string_double(inbox_values[i]))
                            {
                                inboxes[i]->set_color(COLORS::SOFT_PINK);
                                validated += "[ERROR] Input box '" + inbox_names[i] + "' has invalid value.\n";
                            }
                }
                if (inbox_values[0] != "truck" && inbox_values[0] != "car" && inbox_values[0] != "motorcycle" && inbox_values[0] != "bus")
                {
                    inboxes[0]->set_color(COLORS::SOFT_PINK);
                    validated += "[ERROR] Invalid value of 'Vehicle type': must be Car, Truck, Motorcycle or Bus.\n";
                }
                if (inbox_values[1] != "petrol" && inbox_values[1] != "electric" && inbox_values[1] != "diesel" && inbox_values[1] != "hybrid")
                {
                    inboxes[1]->set_color(COLORS::SOFT_PINK);
                    validated += "[ERROR] Invalid value of 'Fuel type': must be Petrol, Diesel, Electric or Hybrid.\n";
                }
                if (inbox_values[6] != "city" && inbox_values[6] != "highway" && inbox_values[6] != "rural")
                {
                    inboxes[6]->set_color(COLORS::SOFT_PINK);
                    validated += "[ERROR] Invalid value of 'Road type': must be City, Highway or Rural.\n";
                }
                if (inbox_values[7] != "free flow" && inbox_values[7] != "heavy" && inbox_values[7] != "moderate")
                {
                    inboxes[7]->set_color(COLORS::SOFT_PINK);
                    validated += "[ERROR] Invalid value of 'Traffic conditions': must be Heavy, Moderate or Free flow.\n";
                }
                if (inbox_values[12] != "" && is_string_double(inbox_values[12]))
                {
                    if (int(std::stold(inbox_values[12])) < num_of_graph_labels_x)
                    {
                        inboxes[12]->set_color(COLORS::SOFT_PINK);
                        validated += "[ERROR] Invalid value of 'Max speed': must be greater than " + std::to_string(num_of_graph_labels_x) + ".\n";
                    }
                }
                else
                {
                    inboxes[12]->set_color(COLORS::SOFT_PINK);
                    validated += "[ERROR] Max speed is invalid: must be number greater than " + std::to_string(num_of_graph_labels_x) + ".\n";
                }
            }
            else
            {
                for (size_t i = 0; i < inboxes.size(); ++i)
                {
                    inboxes[i]->set_color(COLORS::SOFT_PINK);
                }
                validated += "[ERROR] All inboxes are empty.\n";
            }
            message += validated;
            message += "----------------------------------------------\n";
            if (validated != "")
                std::cout << message << std::endl;
            
            redraw();
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