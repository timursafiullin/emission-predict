#ifndef DATASET_READER_H
#define DATASET_READER_H 1

#include <string>
#include <vector>
#include "csv.h"

/**
 * @brief Local address to dataset file
 * 
 */
static const std::string dataset_filename = "../dataset/vehicle_emission_dataset.csv";

// values of normalization constants are based on the dataset statistics

static constexpr double        main_normalization_constant          { 6.0         };
static constexpr double        type_normalization_constant          { 1000.0      };
static constexpr double        age_normalization_constant           { 29.0        };
static constexpr double        mileage_normalization_constant       { 3e5         };
static constexpr double        speed_normalization_constant         { 120.0       };
static constexpr double        acceleration_normalization_constant  { 5.0         };
static constexpr double        temperature_normalization_constant   { 40.0        };
static constexpr double        humidity_normalization_constant      { 100.0       };
static constexpr double        wind_speed_normalization_constant    { 20.0        };
static constexpr double        air_pressure_normalization_constant  { 1050.0      };
static constexpr double        CO2_normalization_constant           { 500.0       };
static constexpr double        NOx_normalization_constant           { 2.0         };
static constexpr double        PM_normalization_constant            { 0.2         };
static constexpr double        VOC_normalization_constant           { 0.1         };
static constexpr double        SO2_normalization_constant           { 0.1         };

// types of vehicles

static const std::string       car_type                             { "car"       };
static const std::string       truck_type                           { "truck"     };
static const std::string       bus_type                             { "bus"       };
static const std::string       motorcycle_type                      { "motorcycle"};

// types of fuel

static const std::string       petrol_type                          { "petrol"    };
static const std::string       diesel_type                          { "diesel"    };
static const std::string       hybrid_type                          { "hybrid"    };
static const std::string       electric_type                        { "electric"  };

// types of road

static const std::string       city_type                            { "city"      };
static const std::string       highway_type                         { "highway"   };
static const std::string       rural_type                           { "rural"     };

// types of traffic

static const std::string       heavy_type                           { "heavy"     };
static const std::string       moderate_type                        { "moderate"  };
static const std::string       free_flow_type                       { "free flow" };

// amount of features in dataset

static constexpr unsigned int  amount_of_features                   { 18          };
static constexpr unsigned int  amount_of_input_features              { 13          };

/**
 * @brief Indexes of features
 * 
 */
enum class Index : unsigned long long
{
    vehicle_type = 0,
    fuel_type,
    engine_size,
    age,
    mileage,
    speed,
    acceleration,
    road_type,
    traffic_conditions,
    temperature,
    humidity,
    wind_speed,
    air_pressure,
    CO2,
    NOx,
    PM,
    VOC,
    SO2
};

/**
 * @brief Enumerates the different types of vehicles and their corresponding values.
 * 
 */
enum class VehicleTypeValues
{
    Motorcycle  = 0,
    Car         = 333,
    Bus         = 666,
    Truck       = 1000,
};

/**
 * @brief Enumerates the different types of fuel and their corresponding values.
 * 
 */
enum class FuelTypeValues
{
    Electric    = 0,
    Hybrid      = 333,
    Diesel      = 666,
    Petrol      = 1000
};

/**
 * @brief Enumerates the different types of road and their corresponding values.
 * 
 */
enum class RoadTypeValues
{
    Rural       = 0,
    Highway     = 500,
    City        = 1000
};

/**
 * @brief Enumerates the different types of traffic conditions and their corresponding values.
 * 
 */
enum class TrafficTypeValues
{
    FreeFlow    = 0,
    Moderate    = 500,
    Heavy       = 1000
};


/**
 * @brief Represents a single cell in the dataset, encapsulating vehicle data and emissions.
 * 
 * This struct provides methods for normalizing the data and converting categorical values to numerical representations.
 */
struct DatasetCell
{
    /**
     * @brief normalizes data values to a common scale.
     * 
     * @return std::vector<long double> 
     */
    std::vector<long double> normalise_data() const;

    // values of features

    std::string             vehicle_type;
    std::string             fuel_type;
    long double             engine_size;
    unsigned long           age_of_vehicle;
    unsigned long long      mileage;
    long double             speed;
    long double             acceleration;
    std::string             road_type;
    std::string             traffic_conditions;
    long double             temperature;
    long double             humidity;
    long double             wind_speed;
    long double             air_pressure;
    long double             CO2_emissions;
    long double             NOx_emissions;
    long double             PM_emissions;
    long double             VOC_emissions;
    long double             SO2_emissions;

private:
    // Helper function to convert categorical values to numerical representation

    static long double turn_vehicle_type_to_double      ( std::string vehicle   );
    static long double turn_fuel_type_to_double         ( std::string fuel      );
    static long double turn_road_type_to_double         ( std::string road      );
    static long double turn_traffic_conditions_to_double( std::string traffic   );
};

/**
 * @brief Get the cell object
 * 
 * @return DatasetCell 
 */
DatasetCell get_cell();

/**
 * @brief Get the all cells object
 * 
 * @return std::vector<DatasetCell> 
 */
std::vector<DatasetCell> get_all_cells();

#endif