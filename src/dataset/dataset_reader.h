#ifndef DATASET_READER_H
#define DATASET_READER_H 1

#include <string>
#include <vector>
#include "csv_reader.h"

struct DatasetCell
{
    std::string vehicle_type;
    std::string fuel_type;
    long double engine_size;
    unsigned long age_of_vehicle;
    unsigned long long mileage;
    long double speed;
    long double acceleration;
    std::string road_type;
    std::string traffic_conditions;
    long double temperature;
};

DatasetCell get_cell();
std::vector<DatasetCell> get_all_cells();

#endif