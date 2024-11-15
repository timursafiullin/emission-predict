#include <vector>
#include "dataset_reader.h"

CsvReader csv_reader;
std::string filename = "../dataset/vehicle_emission_dataset.csv";

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
    for (size_t i = 0; i < 10; ++i)
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
            cell.CO2_emissions = std::stold(line[i]);
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