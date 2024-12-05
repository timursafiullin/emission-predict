#include <iostream>
#include "gui.h"
#include "graph_lib.h"
#include "dataset_reader.h"
#include "csv.h"
#include "Neural_Network/Network.h"
#include <sstream>

using namespace NeuralNetwork;

RowVector* get_input_ptr(std::vector<long double> vector)
{
    RowVector* input = new RowVector(13);
    for (Number i{0}; i < 13; ++i)
    {
        (*input)[i] = vector[i];
    }
    return input;
}

RowVector* get_output_CO2_ptr(std::vector<long double> vector)
{
    RowVector* output = new RowVector(1);
    (*output)[0] = vector[13];
    return output;
}

RowVector* get_output_NOX_ptr(std::vector<long double> vector)
{
    RowVector* output = new RowVector(1);
    (*output)[0] = vector[14];
    return output;
}

RowVector* get_output_PM_ptr(std::vector<long double> vector)
{
    RowVector* output = new RowVector(1);
    (*output)[0] = vector[15];
    return output;
}

RowVector* get_output_VOC_ptr(std::vector<long double> vector)
{
    RowVector* output = new RowVector(1);
    (*output)[0] = vector[16];
    return output;
}

RowVector* get_output_SO2_ptr(std::vector<long double> vector)
{
    RowVector* output = new RowVector(1);
    (*output)[0] = vector[17];
    return output;
}

std::vector<Scalar> turn_СO2_output_to_standart_view(RowVector a)
{
    std::vector<Scalar> ans(1);
    ans[0] = a[0] * 1e-1;
    return ans;
}

std::vector<Scalar> turn_NOX_output_to_standart_view(RowVector a)
{
    std::vector<Scalar> ans(1);
    ans[0] = a[0] * 1e-2;
    return ans;
}

std::vector<Scalar> turn_PM_output_to_standart_view(RowVector a)
{
    std::vector<Scalar> ans(1);
    ans[0] = a[0] * 1e-3;
    return ans;
}

std::vector<Scalar> turn_VOC_output_to_standart_view(RowVector a)
{
    std::vector<Scalar> ans(1);
    ans[0] = a[0] * 1e-3;
    return ans;
}

std::vector<Scalar> turn_SO2_output_to_standart_view(RowVector a)
{
    std::vector<Scalar> ans(1);
    ans[0] = a[0] * 1e-3;
    return ans;
}

int main()
{
    std::vector<DatasetCell> cells = get_all_cells();

    std::vector<RowVector*> input{size_t(cells.size()*0.8)};

    std::vector<RowVector*> output_CO2{size_t(cells.size() * 0.8)};
    std::vector<RowVector*> output_NOX{size_t(cells.size() * 0.8)};
    std::vector<RowVector*> output_PM{size_t(cells.size() * 0.8)};
    std::vector<RowVector*> output_VOC{size_t(cells.size() * 0.8)};
    std::vector<RowVector*> output_SO2{size_t(cells.size() * 0.8)};

    // std::cout << "reading data...\n";

    for (size_t i = 0; i < input.size(); ++i) {
        input[i] = get_input_ptr(cells[i].normalise_data());
        output_CO2[i] = get_output_CO2_ptr(cells[i].normalise_data());
        output_NOX[i] = get_output_NOX_ptr(cells[i].normalise_data());
        output_PM[i] = get_output_PM_ptr(cells[i].normalise_data());
        output_VOC[i] = get_output_VOC_ptr(cells[i].normalise_data());
        output_SO2[i] = get_output_SO2_ptr(cells[i].normalise_data());
    }

    // std::cout << "done.\n";

    // std::cout << "initializing...\n";

    std::vector<Number> structure{
        13, 21, 1
    };

    Scalar l{1e-12};

    NeuralNetwork::NeuralNetwork CO2{structure, l};
    NeuralNetwork::NeuralNetwork NOX{structure, l};
    NeuralNetwork::NeuralNetwork PM {structure, l};
    NeuralNetwork::NeuralNetwork VOC{structure, l};
    NeuralNetwork::NeuralNetwork SO2{structure, l};

    CO2.load_weights_from_file("../weights/weights_CO2.csv");
    NOX.load_weights_from_file("../weights/weights_NOX.csv");
    PM.load_weights_from_file("../weights/weights_PM.csv");
    VOC.load_weights_from_file("../weights/weights_VOC.csv");
    SO2.load_weights_from_file("../weights/weights_SO2.csv");

    // std::cout << "initialized.\n";

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

    RowVector pr = NOX.predict(test_input);
    std::vector ans = turn_NOX_output_to_standart_view(pr);
    std::cout << ans[0] << std::endl;

    return 0;
}