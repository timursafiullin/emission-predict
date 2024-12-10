#ifndef WINDOW_WITH_NEURO_H
#define WINDOW_WITH_NEURO_H 1

#include "Network.h"
#include "graph_lib.h"
#include "dataset_reader.h"
#include <vector>

#define GLib Graph_lib

namespace Graph_lib
{

    class WindowWithNeuro : public GLib::Window
    {
    public:
        WindowWithNeuro(int ww, int hh, const std::string &title, Fl_Color background_color = FL_WHITE) : GLib::Window{ww,
                                                                                                                       hh,
                                                                                                                       title,
                                                                                                                       background_color} {};
        /*
        WindowWithNeuro(int w, int h, const std::string &title) : GLib::Window{w,
                                                                               h,
                                                                               title} {};
        WindowWithNeuro(Point xy, int w, int h, const std::string &title) : GLib::Window{xy,
                                                                                         w,
                                                                                         h,
                                                                                         title} {};
        */
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

        void attach(GLib::In_box &w)
        {
            inboxes.push_back(&w);
            begin();
            w.attach(*this);
            end();
            widgets.push_back(&w);
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

        std::vector<std::string> get_values_from_inboxes()
        {
            std::vector<std::string> inbox_values;
            for (size_t i = 0; i < inboxes.size(); ++i)
            {
                inbox_values.push_back(inboxes[i]->get_string());
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
            ans[0] = a[0] / 6 * 500;
            return ans;
        }

        std::vector<NeuralNetwork::Scalar> turn_NOX_output_to_standart_view(NeuralNetwork::RowVector a)
        {
            std::vector<NeuralNetwork::Scalar> ans(1);
            ans[0] = a[0] / 6 * 2;
            return ans;
        }

        std::vector<NeuralNetwork::Scalar> turn_PM_output_to_standart_view(NeuralNetwork::RowVector a)
        {
            std::vector<NeuralNetwork::Scalar> ans(1);
            ans[0] = a[0] / 6 * 0.2;
            return ans;
        }

        std::vector<NeuralNetwork::Scalar> turn_VOC_output_to_standart_view(NeuralNetwork::RowVector a)
        {
            std::vector<NeuralNetwork::Scalar> ans(1);
            ans[0] = a[0] / 6 * 0.1;
            return ans;
        }

        std::vector<NeuralNetwork::Scalar> turn_SO2_output_to_standart_view(NeuralNetwork::RowVector a)
        {
            std::vector<NeuralNetwork::Scalar> ans(1);
            ans[0] = a[0] / 6 * 0.1;
            return ans;
        }

        std::vector<double> evaluate_network_CO2()
        {
            int max_speed = current_cell.speed;
            std::vector<double> evaluations;

            for (size_t current_speed = 0; current_speed < max_speed; ++current_speed)
            {
                current_cell.speed = current_speed;

                NeuralNetwork::RowVector network_input{*get_input_ptr(current_cell.normalise_data())};
                NeuralNetwork::RowVector pr = CO2.predict(network_input);
                std::vector<NeuralNetwork::Scalar> ans = turn_CO2_output_to_standart_view(pr);
                evaluations.push_back(ans[0]);
            }
            return evaluations;
        }

        bool validate_inboxes()
        {
            std::vector<std::string> inbox_values = get_values_from_inboxes();
            bool validated = true;
            for (std::string &value : inbox_values)
            {
                if (value == "")
                {
                    validated = false;
                    std::cout << "one or more values are empty" << std::endl;
                    break;
                }
            }
            return validated;
        }
    };

    class FunctionStepping : public GLib::Shape
    {
    public:
        FunctionStepping(std::vector<double> points, double r1, double r2, Point xy, double xscale = 25, double yscale = 25)
        {
            int count = points.size();

            double max_graph = *std::max_element(points.begin(), points.end());
            double min_graph = *std::min_element(points.begin(), points.end());

            yscale = (double)(505 - 50) / (double)(max_graph - min_graph); // graph_canvas_h
            xscale = (double)(504 - 50) / (double)count;           // graph_canvas_w

            if (r2 - r1 <= 0)
                error("bad graphing range");
            if (count <= 0)
                error("non-positive graphing count");
            double dist = (r2 - r1) / count;
            double r = r1;
            for (int i = 0; i < count; ++i)
            {
                add(Point{xy.x + int(r * xscale), xy.y - int((points[i] - min_graph) * yscale)});
                std::cout << xy.x + int(r * xscale) << " " << xy.y - int((points[i] - min_graph) * yscale) << std::endl;
                r += dist;
            }
        };
    };

}
#endif