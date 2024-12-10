#include <iostream>
#include "dataset_reader.h"
#include "gui.h"
#include "csv.h"
#include "Network.h"
#include "normalization_funcs.h"
#include <sstream>

#define GLib Graph_lib

using namespace NeuralNetwork;

int main_neuro()
{
#if TRAIN == 1
    std::vector<DatasetCell> cells = get_all_cells();

    std::vector<RowVector *> input{size_t(cells.size() * 0.8)};

    std::vector<RowVector *> output_CO2{size_t(cells.size() * 0.8)};
    std::vector<RowVector *> output_NOX{size_t(cells.size() * 0.8)};
    std::vector<RowVector *> output_PM{size_t(cells.size() * 0.8)};
    std::vector<RowVector *> output_VOC{size_t(cells.size() * 0.8)};
    std::vector<RowVector *> output_SO2{size_t(cells.size() * 0.8)};

    for (size_t i = 0; i < input.size(); ++i)
    {
        input[i] = get_input_ptr(cells[i].normalise_data());
        output_CO2[i] = get_output_CO2_ptr(cells[i].normalise_data());
        output_NOX[i] = get_output_NOX_ptr(cells[i].normalise_data());
        output_PM[i] = get_output_PM_ptr(cells[i].normalise_data());
        output_VOC[i] = get_output_VOC_ptr(cells[i].normalise_data());
        output_SO2[i] = get_output_SO2_ptr(cells[i].normalise_data());
    }
#endif

    std::vector<Number> structure{
        13, 13, 13, 13, 13, 13, 13, 13, 13, 1};

    Scalar l{1e-30};

    NeuralNetwork::NeuralNetwork CO2{structure, l};
    NeuralNetwork::NeuralNetwork NOX{structure, l};
    NeuralNetwork::NeuralNetwork PM{structure, l};
    NeuralNetwork::NeuralNetwork VOC{structure, l};
    NeuralNetwork::NeuralNetwork SO2{structure, l};

#if TRAIN == 1
    for (Number i{0}; i < 10; i++)
    {
        std::cout << i << std::endl;
#pragma omp parallel sections
        {
            {
                CO2.train(input, output_CO2);
            }
#pragma omp section
            {
                NOX.train(input, output_NOX);
            }
#pragma omp section
            {
                PM.train(input, output_PM);
            }
#pragma omp section
            {
                VOC.train(input, output_VOC);
            }
#pragma omp section
            {
                SO2.train(input, output_SO2);
            }
        }

        std::cout << CO2.test(input, output_CO2)[0] << "\n";
        std::cout << NOX.test(input, output_NOX)[0] << "\n";
        std::cout << PM.test(input, output_PM)[0] << "\n";
        std::cout << VOC.test(input, output_VOC)[0] << "\n";
        std::cout << SO2.test(input, output_SO2)[0] << "\n";

        CO2.save_weights_to_file("../weights/weights_CO2.csv");
        NOX.save_weights_to_file("../weights/weights_NOX.csv");
        PM.save_weights_to_file("../weights/weights_PM.csv");
        VOC.save_weights_to_file("../weights/weights_VOC.csv");
        SO2.save_weights_to_file("../weights/weights_SO2.csv");
    }

#else
    CO2.load_weights_from_file("../weights/weights_CO2.csv");
    NOX.load_weights_from_file("../weights/weights_NOX.csv");
    PM.load_weights_from_file("../weights/weights_PM.csv");
    VOC.load_weights_from_file("../weights/weights_VOC.csv");
    SO2.load_weights_from_file("../weights/weights_SO2.csv");

    DatasetCell test_cell;
    std::cin >> test_cell.vehicle_type;
    std::cin >> test_cell.fuel_type;
    std::cin >> test_cell.engine_size;
    std::cin >> test_cell.age_of_vehicle;
    std::cin >> test_cell.mileage;
    std::cin >> test_cell.speed;
    std::cin >> test_cell.acceleration;
    std::cin >> test_cell.road_type;
    std::cin >> test_cell.traffic_conditions;
    std::cin >> test_cell.temperature;
    std::cin >> test_cell.humidity;
    std::cin >> test_cell.wind_speed;
    std::cin >> test_cell.air_pressure;

    RowVector test_input{*get_input_ptr(test_cell.normalise_data())};
    RowVector pr = CO2.predict(test_input);
    std::vector ans = turn_CO2_output_to_standart_view(pr);
    std::cout << ans[0] << std::endl;

#endif

    return 0;
}
/*
int main_gui()
try
{
    // CREATING MAIN WINDOW
    GLib::WindowWithNeuro win{window_width, window_height, main_window_title, COLORS::LIGHT_GRAY};

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
    for (size_t i = 0; i < 13; ++i)
    {
        inboxes.push_back(new GLib::In_box{
            GLib::Point(inbox_x, inbox_y + inbox_h * i),
            inbox_w, inbox_h, ""});
        win.attach(*inboxes[i]);
    }

    win.load_networks();

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
*/
int main()
{
    main_gui();
    return 0;
}