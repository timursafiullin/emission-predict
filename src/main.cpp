#include <iostream>
#include "gui.h"
#include "graph_lib.h"
#include "dataset_reader.h"
#include "csv.h"
#include "Neural_Network/Network.h"
#include <sstream>

using namespace NeuralNetwork;

long double turn_vehicle_type_to_double(std::string vehicle)
{
    //Bus,Car,Truck,Motorcycle
    if (vehicle == "Truck")
        return 3;
    if (vehicle == "Bus")
        return 1.5;
    if (vehicle == "Car")
        return -1.5;
    return -3;
}

long double turn_fuel_type_to_double(std::string fuel)
{
    //Electric, Hybrid, Diesel, Petrol
    if (fuel == "Petrol")
        return 3;
    if (fuel == "Diesel")
        return 1.5;
    if (fuel == "Hybrid")
        return -1.5;
    return -3;
}

long double turn_road_type_to_double(std::string road)
{
    //City, Highway, Rural
    if (road == "City")
        return 3;
    if (road == "Highway")
        return 0;
    return -3;
}

long double turn_traffic_conditions_to_double(std::string traffic)
{
    //Moderate Heavy Free flow
    if (traffic == "Heavy")
        return 3;
    if (traffic == "Moderate")
        return 0;
    return -3;
}

std::vector<long double> normalise_data(DatasetCell cell)
{
    std::vector<long double> vec(19);
    vec[0] = turn_vehicle_type_to_double(cell.vehicle_type) * 1e-15;
    vec[1] = turn_fuel_type_to_double(cell.fuel_type) * 1e-15;
    vec[2] = (cell.engine_size - 3.4) * 3 / 2.6 * 1e-15;
    vec[3] = (cell.age_of_vehicle - 14.5) * 3 / 14.5 * 1e-15;
    vec[4] = (cell.mileage - 150028) * 3 / 149972 * 1e-15;
    vec[5] = (cell.speed - 60) * 3 / 60 * 1e-15;
    vec[6] = (cell.acceleration - 2.5) * 3 / 2.5 * 1e-15;
    vec[7] = turn_road_type_to_double(cell.road_type) * 1e-15;
    vec[8] = turn_traffic_conditions_to_double(cell.traffic_conditions) * 1e-15;
    vec[9] = (cell.temperature - 15) * 3 / 25 * 1e-15;
    vec[10] = (cell.humidity - 50) * 3 / 50 * 1e-15;
    vec[11] = (cell.wind_speed - 10) * 3 / 10 * 1e-15;
    vec[12] = (cell.air_pressure - 1000) * 3 / 50 * 1e-15;
    vec[13] = (cell.CO2_emissions - 275) * 3000000 / 225;
    vec[14] = (cell.NOx_emissions - 1) * 3000000;
    vec[15] = (cell.PM_emissions - 0.1) * 3000000 / 0.1;
    vec[16] = (cell.VOC_emissions - 0.05) * 3000000 / 0.05;
    vec[17] = (cell.SO2_Emissions - 0.05) * 3000000 / 0.05;

    return vec;
}

RowVector* turn_data_to_input_ptr(std::vector<long double> vector)
{
    RowVector* input = new RowVector(13);
    for (Number i{0}; i < 13; ++i)
    {
        (*input)[i] = vector[i];
    }
    return input;
}

RowVector* turn_data_to_output_ptr(std::vector<long double> vector)
{
    RowVector* output = new RowVector(5);
    for (Number i{13}; i < 18; ++i)
    {
        (*output)[i - 13] = vector[i - 13];
    }
    return output;
}

std::vector<Scalar> turn_output_to_standart_view(RowVector a)
{
    std::vector<Scalar> ans(5);
    ans[0] = a[0] / 3000000 * 225 + 275;
    ans[1] = a[1] / 3000000 + 1;
    ans[2] = a[2] / 3000000 * 0.1;
    ans[3] = a[3] / 3000000 * 0.05 + 0.05;
    ans[4] = a[4] / 3000000 * 0.05 + 0.05;
    return ans;
}

int main()
{
    std::vector<DatasetCell> cells = get_all_cells();

    std::vector<RowVector*> input{size_t(cells.size()*0.8)};
    std::vector<RowVector*> output{size_t(cells.size()*0.8)};

    std::vector<RowVector*> test_input{size_t(cells.size()*0.2)};
    std::vector<RowVector*> test_output{size_t(cells.size()*0.2)};

    std::cout << "reading data...\n";

    for (size_t i = 0; i < input.size(); ++i) {
        input[i] = turn_data_to_input_ptr(normalise_data(cells[i]));
        output[i] = turn_data_to_output_ptr(normalise_data(cells[i]));
    }

    std::cout << "done.\n";

    std::cout << "reading data...\n";

    for (size_t i{size_t(cells.size()*0.8)}; i < size_t(cells.size()*0.9); ++i) {
        test_input[i - size_t(cells.size()*0.8)] = turn_data_to_input_ptr(normalise_data(cells[i]));
        test_output[i - size_t(cells.size()*0.8)] = turn_data_to_output_ptr(normalise_data(cells[i]));
    }

    std::cout << "done.\n";

    std::vector<Number> structure{
        13, 14, 14, 14, 14,
        14, 14, 14, 14, 14,
        14, 14, 14, 14, 14,
        14, 14, 14, 14, 14,
        14, 14, 14, 5};

    Scalar l{1e-21};

    std::cout << "initializing...\n";
    NeuralNetwork::NeuralNetwork a{structure, 1e-12};
    std::cout << "initialized.\n";
    /*
    std::cout << "training...\n";
    for (Number i{0}; i < 4; ++i)
    {
        std::cout << i + 1 << "\n";

        for (Number j{0}; j < 3; j++)
        //for (Number j{0}; j < (i + 1) * (i + 1) * 10; j++)
            a.train(input, output, l);

        l *= 10;

        if (i < 3)
            continue;

        a.save_weights_to_file("weights.csv");
    }
    
    std::cout << "trained.\n";
    */
    std::cout << "loading weights...\n";
    a.load_weights_from_file("weights.csv");
    std::cout << "loaded.\n";

    std::cout << "testing...\n";
    std::vector<Scalar> error{a.test(test_input, test_output)};
    for (Scalar e : error)
        std::cout << e << " ";
    std::cout << "\n";
    RowVector pr = a.predict(*(input[999]));
    std::vector ans = turn_output_to_standart_view(pr);
    std::cout << "answer on 1000 elem:\n"
        << ans[0] << " "
        << ans[1] << " "
        << ans[2] << " "
        << ans[3] << " "
        << ans[4] << "\n";
    std::cout << "tested.\n\n\n\n";

    return 0;
}