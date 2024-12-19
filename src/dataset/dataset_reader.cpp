#include <vector>
#include "dataset_reader.h"


CsvReader csv_reader;

std::vector<long double> DatasetCell::normalise_data() const
{
    std::vector<long double> v(amount_of_features);

    v[(unsigned long long)Index::vehicle_type      ] = turn_vehicle_type_to_double(vehicle_type) / type_normalization_constant * main_normalization_constant;
    v[(unsigned long long)Index::fuel_type         ] = turn_fuel_type_to_double(fuel_type) / type_normalization_constant * main_normalization_constant;
    v[(unsigned long long)Index::engine_size       ] = engine_size;
    v[(unsigned long long)Index::age               ] = age_of_vehicle / age_normalization_constant * main_normalization_constant;
    v[(unsigned long long)Index::mileage           ] = mileage / mileage_normalization_constant * main_normalization_constant;
    v[(unsigned long long)Index::speed             ] = speed / speed_normalization_constant* main_normalization_constant;
    v[(unsigned long long)Index::acceleration      ] = acceleration / acceleration_normalization_constant * main_normalization_constant;
    v[(unsigned long long)Index::road_type         ] = turn_road_type_to_double(road_type) / type_normalization_constant * main_normalization_constant;
    v[(unsigned long long)Index::traffic_conditions] = turn_traffic_conditions_to_double(traffic_conditions) / type_normalization_constant * main_normalization_constant;
    v[(unsigned long long)Index::temperature       ] = temperature / temperature_normalization_constant * main_normalization_constant;
    v[(unsigned long long)Index::humidity          ] = humidity / humidity_normalization_constant * main_normalization_constant;
    v[(unsigned long long)Index::wind_speed        ] = wind_speed / wind_speed_normalization_constant * main_normalization_constant;
    v[(unsigned long long)Index::air_pressure      ] = air_pressure / air_pressure_normalization_constant * main_normalization_constant;
    v[(unsigned long long)Index::CO2               ] = CO2_emissions / CO2_normalization_constant * main_normalization_constant;
    v[(unsigned long long)Index::NOx               ] = NOx_emissions / NOx_normalization_constant * main_normalization_constant;
    v[(unsigned long long)Index::PM                ] = PM_emissions / PM_normalization_constant * main_normalization_constant;
    v[(unsigned long long)Index::VOC               ] = VOC_emissions / VOC_normalization_constant * main_normalization_constant;
    v[(unsigned long long)Index::SO2               ] = SO2_emissions / SO2_normalization_constant * main_normalization_constant;

    return v;
}

long double DatasetCell::turn_vehicle_type_to_double(std::string vehicle)
{
    if (vehicle == truck_type)
        return (long double)(VehicleTypeValues::Truck);
    if (vehicle == bus_type)
        return (long double)(VehicleTypeValues::Bus);
    if (vehicle == car_type)
        return (long double)(VehicleTypeValues::Car);
    return (long double)(VehicleTypeValues::Motorcycle);
}

long double DatasetCell::turn_fuel_type_to_double(std::string fuel)
{
    if (fuel == petrol_type)
        return (long double)FuelTypeValues::Petrol;
    if (fuel == diesel_type)
        return (long double)FuelTypeValues::Diesel;
    if (fuel == hybrid_type)
        return (long double)FuelTypeValues::Hybrid;
    return (long double)FuelTypeValues::Electric;
}

long double DatasetCell::turn_road_type_to_double(std::string road)
{
    if (road == city_type)
        return (long double)RoadTypeValues::City;
    if (road == highway_type)
        return (long double)RoadTypeValues::Highway;
    return (long double)RoadTypeValues::Rural;
}

long double DatasetCell::turn_traffic_conditions_to_double(std::string traffic)
{
    if (traffic == heavy_type)
        return (long double)TrafficTypeValues::Heavy;
    if (traffic == moderate_type)
        return (long double)TrafficTypeValues::Moderate;
    return (long double)TrafficTypeValues::FreeFlow;
}

DatasetCell get_cell()
{
    if (!csv_reader.is_open())
    {
        csv_reader.open_file(dataset_filename);
        csv_reader.read_row(); // get rid of csv columns header
    }

    DatasetCell cell;
    std::vector<std::string> line = csv_reader.read_row();
    if (line == std::vector<std::string>{""})
        throw FileIsClosedError();
    for (size_t i = 0; i < amount_of_features; ++i)
    {
        switch (i)
        {
        case (unsigned long long)Index::vehicle_type:
        {
            cell.vehicle_type = line[i];
            break;
        }
        case (unsigned long long)Index::fuel_type:
        {
            cell.fuel_type = line[i];
            break;
        }
        case (unsigned long long)Index::engine_size:
        {
            cell.engine_size = std::stold(line[i]);
            break;
        }
        case (unsigned long long)Index::age:
        {
            cell.age_of_vehicle = std::stoul(line[i]);
            break;
        }
        case (unsigned long long)Index::mileage:
        {
            cell.mileage = std::stoull(line[i]);
            break;
        }
        case (unsigned long long)Index::speed:
        {
            cell.speed = std::stold(line[i]);
            break;
        }
        case (unsigned long long)Index::acceleration:
        {
            cell.acceleration = std::stold(line[i]);
            break;
        }
        case (unsigned long long)Index::road_type:
        {
            cell.road_type = line[i];
            break;
        }
        case (unsigned long long)Index::traffic_conditions:
        {
            cell.traffic_conditions = line[i];
            break;
        }
        case (unsigned long long)Index::temperature:
        {
            cell.temperature = std::stold(line[i]);
            break;
        }
        case (unsigned long long)Index::humidity:
        {
            cell.humidity = std::stold(line[i]);
            break;
        }
        case (unsigned long long)Index::wind_speed:
        {
            cell.wind_speed = std::stold(line[i]);
            break;
        }
        case (unsigned long long)Index::air_pressure:
        {
            cell.air_pressure = std::stold(line[i]);
            break;
        }
        case (unsigned long long)Index::CO2:
        {
            cell.CO2_emissions = std::stold(line[i]);
            break;
        }
        case (unsigned long long)Index::NOx:
        {
            cell.NOx_emissions = std::stold(line[i]);
            break;
        }
        case (unsigned long long)Index::PM:
        {
            cell.PM_emissions = std::stold(line[i]);
            break;
        }
        case (unsigned long long)Index::VOC:
        {
            cell.VOC_emissions = std::stold(line[i]);
            break;
        }
        case (unsigned long long)Index::SO2:
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