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
    //Bus, Car, Truck, Motorcycle
    if (vehicle == "Truck")
        return 1000;
    if (vehicle == "Bus")
        return 666;
    if (vehicle == "Car")
        return 333;
    return 0;
}

long double turn_fuel_type_to_double(std::string fuel)
{
    //Electric, Hybrid, Diesel, Petrol
    if (fuel == "Petrol")
        return 1000;
    if (fuel == "Diesel")
        return 666;
    if (fuel == "Hybrid")
        return 333;
    return 0;
}

long double turn_road_type_to_double(std::string road)
{
    //City, Highway, Rural
    if (road == "City")
        return 1000;
    if (road == "Highway")
        return 500;
    return 0;
}

long double turn_traffic_conditions_to_double(std::string traffic)
{
    //Moderate Heavy Free flow
    if (traffic == "Heavy")
        return 1000;
    if (traffic == "Moderate")
        return 500;
    return 0;
}

std::vector<long double> normalise_data(DatasetCell cell)
{
    std::vector<long double> vec(19);
    vec[0] = turn_vehicle_type_to_double(cell.vehicle_type);
    vec[1] = turn_fuel_type_to_double(cell.fuel_type);
    vec[2] = cell.engine_size;
    vec[3] = cell.age_of_vehicle;
    vec[4] = cell.mileage;
    vec[5] = cell.speed;
    vec[6] = cell.acceleration;
    vec[7] = turn_road_type_to_double(cell.road_type);
    vec[8] = turn_traffic_conditions_to_double(cell.traffic_conditions);
    vec[9] = cell.temperature;
    vec[10] = cell.humidity;
    vec[11] = cell.wind_speed;
    vec[12] = cell.air_pressure;
    vec[13] = 1e1 * cell.CO2_emissions;
    vec[14] = 1e2 * cell.NOx_emissions;
    vec[15] = 1e3 * cell.PM_emissions;
    vec[16] = 1e3 * cell.VOC_emissions;
    vec[17] = 1e3 * cell.SO2_emissions;

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

RowVector* turn_data_to_output_CO2_ptr(std::vector<long double> vector)
{
    RowVector* output = new RowVector(1);
    (*output)[0] = vector[13];
    return output;
}

RowVector* turn_data_to_output_NOX_ptr(std::vector<long double> vector)
{
    RowVector* output = new RowVector(1);
    (*output)[0] = vector[14];
    return output;
}

RowVector* turn_data_to_output_PM_ptr(std::vector<long double> vector)
{
    RowVector* output = new RowVector(1);
    (*output)[0] = vector[15];
    return output;
}

RowVector* turn_data_to_output_VOC_ptr(std::vector<long double> vector)
{
    RowVector* output = new RowVector(1);
    (*output)[0] = vector[16];
    return output;
}

RowVector* turn_data_to_output_SO2_ptr(std::vector<long double> vector)
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

    std::vector<RowVector*> input{size_t(cells.size()*0.2)};

    std::vector<RowVector*> output_CO2{size_t(cells.size()*0.2)};
    std::vector<RowVector*> output_NOX{size_t(cells.size()*0.2)};
    std::vector<RowVector*> output_PM{size_t(cells.size()*0.2)};
    std::vector<RowVector*> output_VOC{size_t(cells.size()*0.2)};
    std::vector<RowVector*> output_SO2{size_t(cells.size()*0.2)};

    std::cout << "reading data...\n";

    for (size_t i = 0; i < input.size(); ++i) {
        input[i] = turn_data_to_input_ptr(normalise_data(cells[cells.size() * 0.8 + i]));
        output_CO2[i] = turn_data_to_output_CO2_ptr(normalise_data(cells[cells.size() * 0.8 + i]));
        output_NOX[i] = turn_data_to_output_NOX_ptr(normalise_data(cells[cells.size() * 0.8 + i]));
        output_PM[i] = turn_data_to_output_PM_ptr(normalise_data(cells[cells.size() * 0.8 + i]));
        output_VOC[i] = turn_data_to_output_VOC_ptr(normalise_data(cells[cells.size() * 0.8 + i]));
        output_SO2[i] = turn_data_to_output_SO2_ptr(normalise_data(cells[cells.size() * 0.8 + i]));
    }

    std::cout << "done.\n";

    std::cout << "initializing...\n";

    std::vector<Number> structure{
        13, 21, 1
    };

    Scalar l{1e-15};

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

    std::cout << "initialized.\n";

    std::cout << CO2.test(input, output_CO2)[0] << std::endl;
    std::cout << NOX.test(input, output_NOX)[0] << std::endl;
    std::cout << PM.test(input, output_PM)[0] << std::endl;
    std::cout << VOC.test(input, output_VOC)[0] << std::endl;
    std::cout << SO2.test(input, output_SO2)[0] << std::endl;

    return 0;
}