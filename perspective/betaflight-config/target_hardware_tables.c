// Definitions for hardware tables for TCMMF411 target
#include "drivers/compass/compass.h"
#include "drivers/barometer/barometer.h"

// Ensure these symbols are visible to the linker and other translation units
const char * const table_mag_hardware[] __attribute__((used));
const char * const table_baro_hardware[] __attribute__((used));

// List of enabled magnetometers (order and names must match magSensor_e in compass.h)
const char * const table_mag_hardware[] = {
    "NONE",
    "AUTODETECT",
    "HMC5883",
    "AK8975",
    "MAG3110",
    "AK8963",
    "IST8310",
    "QMC5883",
    "MPU9250",
    "IST8308",
    "LIS3MDL",
    "MSP",
    "RM3100",
    "VCM5883",
    "MLX90393",
    "FAKE"
};

// List of enabled barometers (order and names must match baroSensor_e in barometer.h)
const char * const table_baro_hardware[] = {
    "NONE",
    "AUTODETECT",
    "BMP085",
    "MS5611",
    "BMP280",
    "MS5607",
    "LPS25H",
    "SPL06",
    "BMP388",
    "DPS310",
    "B2SMPB",
    "MSP",
    "FAKE"
};
