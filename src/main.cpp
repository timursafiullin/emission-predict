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

    FFNN<std::function<long double(long double)>> CO2 {
        structure,
        l,
        ([] (Scalar x) { return 1 / (1 + std::pow(std::exp(1.0), -x)); } ),
        ([] (Scalar x) { return std::pow(std::exp(1.0), -x) / std::pow(1 + std::pow(std::exp(1.0), -x), 2); } )
    };

    FFNN<std::function<long double(long double)>> NOX{
        structure,
        l,
        ([] (Scalar x) { return 1 / (1 + std::pow(std::exp(1.0), -x)); } ),
        ([] (Scalar x) { return std::pow(std::exp(1.0), -x) / std::pow(1 + std::pow(std::exp(1.0), -x), 2); } )
    };

    FFNN<std::function<long double(long double)>> PM{
        structure,
        l,
        ([] (Scalar x) { return 1 / (1 + std::pow(std::exp(1.0), -x)); } ),
        ([] (Scalar x) { return std::pow(std::exp(1.0), -x) / std::pow(1 + std::pow(std::exp(1.0), -x), 2); } )
    };

    FFNN<std::function<long double(long double)>> VOC{
        structure,
        l,
        ([] (Scalar x) { return 1 / (1 + std::pow(std::exp(1.0), -x)); } ),
        ([] (Scalar x) { return std::pow(std::exp(1.0), -x) / std::pow(1 + std::pow(std::exp(1.0), -x), 2); } )
    };

    FFNN<std::function<long double(long double)>> SO2{
        structure,
        l,
        ([] (Scalar x) { return 1 / (1 + std::pow(std::exp(1.0), -x)); } ),
        ([] (Scalar x) { return std::pow(std::exp(1.0), -x) / std::pow(1 + std::pow(std::exp(1.0), -x), 2); } )
    };


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

int main()
{
    main_gui();
    return 0;
}