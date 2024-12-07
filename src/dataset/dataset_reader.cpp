#include <vector>
#include "dataset_reader.h"

CsvReader csv_reader;
std::string filename = "../dataset/vehicle_emission_dataset.csv";

std::vector<long double> DatasetCell::normalise_data()
{
    std::vector<long double> vec(19);
    vec[0] = turn_vehicle_type_to_double(vehicle_type) / 1000 * 6;
    vec[1] = turn_fuel_type_to_double(fuel_type) / 1000 * 6;
    vec[2] = engine_size;
    vec[3] = age_of_vehicle / 29 * 6;
    vec[4] = mileage / 300000 * 6;
    vec[5] = speed / 120 * 6;
    vec[6] = acceleration / 5 * 6;
    vec[7] = turn_road_type_to_double(road_type) / 1000 * 6;
    vec[8] = turn_traffic_conditions_to_double(traffic_conditions) / 1000 * 6;
    vec[9] = temperature / 40 * 6;
    vec[10] = humidity / 100 * 6;
    vec[11] = wind_speed / 20 * 6;
    vec[12] = air_pressure / 1050 * 6;
    vec[13] = CO2_emissions / 500 * 6;
    vec[14] = NOx_emissions / 2 * 6;
    vec[15] = PM_emissions / 0.2 * 6;
    vec[16] = VOC_emissions / 0.1 * 6;
    vec[17] = SO2_emissions / 0.1 * 6;

    return vec;
}

long double DatasetCell::turn_vehicle_type_to_double(std::string vehicle)
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

long double DatasetCell::turn_fuel_type_to_double(std::string fuel)
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

long double DatasetCell::turn_road_type_to_double(std::string road)
{
    //City, Highway, Rural
    if (road == "City")
        return 1000;
    if (road == "Highway")
        return 500;
    return 0;
}

long double DatasetCell::turn_traffic_conditions_to_double(std::string traffic)
{
    //Moderate Heavy Free flow
    if (traffic == "Heavy")
        return 1000;
    if (traffic == "Moderate")
        return 500;
    return 0;
}

DatasetCell get_cell()
{
    if (!csv_reader.is_open())
    {
        csv_reader.open_file(filename);
        csv_reader.read_row(); // get rid of csv columns header
    }

    DatasetCell cell;
    std::vector<std::string> line = csv_reader.read_row();
    if (line == std::vector<std::string>{""})
        throw FileIsClosedError();
    for (size_t i = 0; i < 18; ++i)
    {
        switch (i)
        {
        case 0:
        {
            cell.vehicle_type = line[i];
            break;
        }
        case 1:
        {
            cell.fuel_type = line[i];
            break;
        }
        case 2:
        {
            cell.engine_size = std::stold(line[i]);
            break;
        }
        case 3:
        {
            cell.age_of_vehicle = std::stoul(line[i]);
            break;
        }
        case 4:
        {
            cell.mileage = std::stoull(line[i]);
            break;
        }
        case 5:
        {
            cell.speed = std::stold(line[i]);
            break;
        }
        case 6:
        {
            cell.acceleration = std::stold(line[i]);
            break;
        }
        case 7:
        {
            cell.road_type = line[i];
            break;
        }
        case 8:
        {
            cell.traffic_conditions = line[i];
            break;
        }
        case 9:
        {
            cell.temperature = std::stold(line[i]);
            break;
        }
        case 10:
        {
            cell.humidity = std::stold(line[i]);
            break;
        }
        case 11:
        {
            cell.wind_speed = std::stold(line[i]);
            break;
        }
        case 12:
        {
            cell.air_pressure = std::stold(line[i]);
            break;
        }
        case 13:
        {
            cell.CO2_emissions = std::stold(line[i]);
            break;
        }
        case 14:
        {
            cell.NOx_emissions = std::stold(line[i]);
            break;
        }
        case 15:
        {
            cell.PM_emissions = std::stold(line[i]);
            break;
        }
        case 16:
        {
            cell.VOC_emissions = std::stold(line[i]);
            break;
        }
        case 17:
        {
            cell.SO2_emissions = std::stold(line[i]);
            break;
        }
        }
    }

    return cell;
}

std::vector<DatasetCell> get_all_cells()
{
    std::vector<DatasetCell> cells;
    try
    {
        while (true)
        {
            DatasetCell cell = get_cell();
            cells.push_back(cell);
        }
    }
    catch (FileIsClosedError const &) {
        return cells;
    }
    return cells;
}