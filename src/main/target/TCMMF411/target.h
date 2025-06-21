/*
 * This file is part of Cleanflight.
 *
 * Cleanflight is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Cleanflight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Cleanflight.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

// Enable required INAV features for TCMMF411
#define USE_GPS
#define USE_NAV
#define USE_BARO
#define USE_RANGEFINDER
#define USE_PITOT
#define USE_OPFLOW
#define USE_VTX
#define USE_OSD
#define USE_TELEMETRY
#define USE_CRSF

#define TARGET_BOARD_IDENTIFIER "TCMM"
#define USBD_PRODUCT_STRING     "Tyro79 Pro"

#define LED0                    PC13
#define LED1                    PC14

#define BEEPER                  PB2
#define BEEPER_INVERTED
#define BEEPER_OD              OFF

// *************** SPI1 Gyro & ACC **********************
#define USE_SPI
#define USE_SPI_DEVICE_1

#define SPI1_SCK_PIN            PA5
#define SPI1_MISO_PIN           PA6
#define SPI1_MOSI_PIN           PA7

#define USE_IMU_BMI270
#define IMU_BMI270_ALIGN        CW270_DEG
#define BMI270_CS_PIN           PA4
#define BMI270_SPI_BUS          BUS_SPI1

// *************** SPI2 OSD *****************************
#define USE_SPI_DEVICE_2
#define SPI2_SCK_PIN            PB13
#define SPI2_MISO_PIN           PB14
#define SPI2_MOSI_PIN           PB15

#define USE_MAX7456
#define MAX7456_SPI_BUS         BUS_SPI2
#define MAX7456_CS_PIN          PB12

// *************** UART *****************************
#define USE_VCP
#define VBUS_SENSING_PIN        PC15
#define VBUS_SENSING_ENABLED

#define USE_UART1
#define UART1_TX_PIN            PA9
#define UART1_RX_PIN            PA10

#define USE_UART2
#define UART2_TX_PIN            PA2
#define UART2_RX_PIN            PA3

#define USE_SOFTSERIAL1
#define SOFTSERIAL_1_TX_PIN     PA0
#define SOFTSERIAL_1_RX_PIN     PA0

#define SERIAL_PORT_COUNT       4

#define DEFAULT_RX_TYPE         RX_TYPE_SERIAL
#define SERIALRX_PROVIDER       SERIALRX_SBUS
#define SERIALRX_UART           SERIAL_PORT_USART1

// *************** I2C /Baro/Mag/Pitot ********************
#define USE_I2C
#define USE_I2C_DEVICE_1
#define I2C1_SCL                PB8
#define I2C1_SDA                PB9
#define USE_I2C_PULLUP

#define DEFAULT_I2C_BUS         BUS_I2C1

#define USE_BARO
#define BARO_I2C_BUS            BUS_I2C1
#define USE_BARO_BMP280

#define TEMPERATURE_I2C_BUS     BUS_I2C1
#define PITOT_I2C_BUS           BUS_I2C1

#define USE_MAG
#define MAG_I2C_BUS             BUS_I2C1
#define USE_MAG_HMC5883
#define USE_MAG_QMC5883
#define USE_MAG_IST8310
#define USE_MAG_MAG3110
#define USE_MAG_LIS3MDL

// *************** ADC *****************************
#define USE_ADC
#define ADC_INSTANCE            ADC1
#define ADC1_DMA_STREAM         DMA2_Stream0

#define ADC_CHANNEL_1_PIN       PB0
#define ADC_CHANNEL_2_PIN       PB1
#define VBAT_ADC_CHANNEL        ADC_CHN_1
#define CURRENT_METER_ADC_CHANNEL   ADC_CHN_2

// *************** LED2812 ************************
#define USE_LED_STRIP
#define WS2811_PIN              PA8

// ***************  OTHERS ************************
#define DEFAULT_FEATURES        (FEATURE_TX_PROF_SEL | FEATURE_OSD | FEATURE_CURRENT_METER | FEATURE_VBAT | FEATURE_TELEMETRY | FEATURE_SOFTSERIAL)

#define USE_SPEKTRUM_BIND
#define BIND_PIN                PA10

#define USE_DSHOT
#define USE_ESC_SENSOR

#define USE_SERIAL_4WAY_BLHELI_INTERFACE

#define TARGET_IO_PORTA         0xffff
#define TARGET_IO_PORTB         0xffff
#define TARGET_IO_PORTC         0xffff
#define TARGET_IO_PORTD         (BIT(2))

#define MAX_PWM_OUTPUT_PORTS    7

// Provide default values for PINIOBOX settings to fix build errors
#define SETTING_PINIO_BOX1_DEFAULT 0
#define SETTING_PINIO_BOX2_DEFAULT 0
#define SETTING_PINIO_BOX3_DEFAULT 0
#define SETTING_PINIO_BOX4_DEFAULT 0

// Default magnetometer settings for TCMMF411
#define SETTING_MAG_HARDWARE_DEFAULT         0 // AUTODETECT
#define SETTING_ALIGN_MAG_DEFAULT            ALIGN_DEFAULT
#define SETTING_MAG_DECLINATION_DEFAULT      0
#define SETTING_MAG_CALIBRATION_TIME_DEFAULT 30
#define SETTING_ALIGN_MAG_ROLL_DEFAULT       0
#define SETTING_ALIGN_MAG_PITCH_DEFAULT      0
#define SETTING_ALIGN_MAG_YAW_DEFAULT        0
#define SETTING_MAGGAIN_X_DEFAULT            1024
#define SETTING_MAGGAIN_Y_DEFAULT            1024
#define SETTING_MAGGAIN_Z_DEFAULT            1024

// For build system: force-include this file in all sources with -include "src/main/target/TCMMF411/target.h"
// Example for CMake: add_compile_options(-include "${CMAKE_SOURCE_DIR}/src/main/target/TCMMF411/target.h")

#ifdef __cplusplus
extern "C" {
#endif
extern const char * const table_mag_hardware[];
extern const char * const table_baro_hardware[];
#ifdef __cplusplus
}
#endif
