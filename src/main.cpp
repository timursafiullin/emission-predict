#include <iostream>
#include "gui.h"
#include "graph_lib.h"
#include "dataset_reader.h"
#include "Neural_Network/Network.h"
#include <fstream>

using namespace NeuralNetwork;

long double turn_vehicle_type_to_double(std::string vehicle)
{
    //Bus,Car,Truck,Motorcycle
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
    vec[13] = 1e6 * cell.CO2_emissions;
    vec[14] = 1e7 * cell.NOx_emissions;
    vec[15] = 1e8 * cell.PM_emissions;
    vec[16] = 1e8 * cell.VOC_emissions;
    vec[17] = 1e8 * cell.SO2_emissions;

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

std::vector<Scalar> turn_output_to_standart_view(RowVector a)
{
    std::vector<Scalar> ans(1);
    ans[0] = a[0];
    return ans;
}

int main()
{
    std::vector<DatasetCell> cells = get_all_cells();

    std::vector<RowVector*> input{size_t(cells.size()*0.8)};

    std::vector<RowVector*> output_CO2{size_t(cells.size()*0.8)};
    std::vector<RowVector*> output_NOX{size_t(cells.size()*0.8)};
    std::vector<RowVector*> output_PM{size_t(cells.size()*0.8)};
    std::vector<RowVector*> output_VOC{size_t(cells.size()*0.8)};
    std::vector<RowVector*> output_SO2{size_t(cells.size()*0.8)};

    std::cout << "reading data...\n";

    for (size_t i = 0; i < input.size(); ++i) {
        input[i] = turn_data_to_input_ptr(normalise_data(cells[i]));
        output_CO2[i] = turn_data_to_output_CO2_ptr(normalise_data(cells[i]));
        output_NOX[i] = turn_data_to_output_NOX_ptr(normalise_data(cells[i]));
        output_PM[i] = turn_data_to_output_PM_ptr(normalise_data(cells[i]));
        output_VOC[i] = turn_data_to_output_VOC_ptr(normalise_data(cells[i]));
        output_SO2[i] = turn_data_to_output_SO2_ptr(normalise_data(cells[i]));
    }

    std::cout << "done.\n";

    // std::cout << "reading data...\n";

    // for (size_t i{size_t(cells.size()*0.8)}; i < size_t(cells.size()*0.9); ++i) {
    //     test_input[i - size_t(cells.size()*0.8)] = turn_data_to_input_ptr(normalise_data(cells[i]));
    //     test_output[i - size_t(cells.size()*0.8)] = turn_data_to_output_ptr(normalise_data(cells[i]));
    // }

    // std::cout << "done.\n";

    std::cout << "initializing...\n";

    std::vector<Number> structure
    { 13, 20, 20, 20, 20, 20, 1};
    Scalar l{1e-17};

    NeuralNetwork::NeuralNetwork CO2{structure};
    NeuralNetwork::NeuralNetwork NOX{structure};
    NeuralNetwork::NeuralNetwork PM{structure};
    NeuralNetwork::NeuralNetwork VOC{structure};
    NeuralNetwork::NeuralNetwork SO2{structure};


    std::cout << "initialized.\n";

    std::cout << "training...\n";

    for (Number i{0}; i < 1000; ++i)
    {
        for (Number j{0}; j < 100; ++j)
        {
            CO2.train(input, output_CO2, l);
            NOX.train(input, output_NOX, l);
            PM.train(input, output_PM, l);
            VOC.train(input, output_VOC, l);
            SO2.train(input, output_SO2, l);
        }

        std::cout << "ans CO2: " << CO2.predict(*input[0]) << " " << *output_CO2[0] << "\n";
        std::cout << "ans CO2: " << CO2.predict(*input[1]) << " " << *output_CO2[1] << "\n";
        std::cout << "ans CO2: " << CO2.predict(*input[2]) << " " << *output_CO2[2] << "\n";
        std::cout << "ans CO2: " << CO2.predict(*input[3]) << " " << *output_CO2[3] << "\n\n";
        
        std::cout << "ans NOX: " << NOX.predict(*input[0]) << " " << *output_NOX[0] << "\n";
        std::cout << "ans NOX: " << NOX.predict(*input[1]) << " " << *output_NOX[1] << "\n";
        std::cout << "ans NOX: " << NOX.predict(*input[2]) << " " << *output_NOX[2] << "\n";
        std::cout << "ans NOX: " << NOX.predict(*input[3]) << " " << *output_NOX[3] << "\n\n";

        std::cout << "ans PM: " << PM.predict(*input[0]) << " " << *output_PM[0] << "\n";
        std::cout << "ans PM: " << PM.predict(*input[1]) << " " << *output_PM[1] << "\n";
        std::cout << "ans PM: " << PM.predict(*input[2]) << " " << *output_PM[2] << "\n";
        std::cout << "ans PM: " << PM.predict(*input[3]) << " " << *output_PM[3] << "\n\n";

        std::cout << "ans VOC: " << VOC.predict(*input[0]) << " " << *output_VOC[0] << "\n";
        std::cout << "ans VOC: " << VOC.predict(*input[1]) << " " << *output_VOC[1] << "\n";
        std::cout << "ans VOC: " << VOC.predict(*input[2]) << " " << *output_VOC[2] << "\n";
        std::cout << "ans VOC: " << VOC.predict(*input[3]) << " " << *output_VOC[3] << "\n\n";

        std::cout << "ans SO2: " << SO2.predict(*input[0]) << " " << *output_SO2[0] << "\n";
        std::cout << "ans SO2: " << SO2.predict(*input[1]) << " " << *output_SO2[1] << "\n";
        std::cout << "ans SO2: " << SO2.predict(*input[2]) << " " << *output_SO2[2] << "\n";
        std::cout << "ans SO2: " << SO2.predict(*input[3]) << " " << *output_SO2[3] << "\n\n\n\n";

        std::ofstream co2{"weightsCO2.txt"};

        std::vector<Matrix*> weights1{CO2.get_weights()};
        for (Matrix* l : weights1)
        {
            co2 << *l << "\n";
            co2 << "\n\n\n";
        }

        std::ofstream nox{"weightsNOX.txt"};

        std::vector<Matrix*> weights2{NOX.get_weights()};
        for (Matrix* l : weights2)
        {
            nox << *l << "\n";
            nox << "\n\n\n";
        }

        std::ofstream pm{"weightsPM.txt"};

        std::vector<Matrix*> weights3{PM.get_weights()};
        for (Matrix* l : weights3)
        {
            pm << *l << "\n";
            pm << "\n\n\n";
        }

        std::ofstream voc{"weightsVOC.txt"};

        std::vector<Matrix*> weights4{VOC.get_weights()};
        for (Matrix* l : weights4)
        {
            voc << *l << "\n";
            voc << "\n\n\n";
        }

        std::ofstream so2{"weightsSO2.txt"};

        std::vector<Matrix*> weights5{SO2.get_weights()};
        for (Matrix* l : weights5)
        {
            so2 << *l << "\n";
            so2 << "\n\n\n";
        }
    }
    std::cout << "trained.\n";


    return 0;
}