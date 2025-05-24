Product Requirements Document: Eachine Tyro79 Support in INAV 6.1.0
1. Overview
The Eachine Tyro79 quadcopter uses an STM32F411-based flight controller with an ICM20602 gyro/accelerometer, which is not fully supported by the MATEKF411 target in INAV 6.1.0. Users experience issues such as no sensor detection (GYRO=UNAVAILABLE, ACC=NONE) and communication failures after flashing. This PRD outlines requirements to add proper support, either by updating the MATEKF411 target or creating a new TYRO79 target.
2. Problem Statement

Issue: The MATEKF411 target assumes an MPU6500 IMU, but the Tyro79 uses an ICM20602, leading to undetected sensors. Additionally, I2C2 pins (PB3, PB10) for a BMP280 barometer and PA8 for servo output are not configured.
Impact: Tyro79 users cannot use INAV effectively, limiting access to navigation features like altitude hold.
Evidence: Issue #4863, Discussion #6716.

3. Requirements
3.1 Functional Requirements

Sensor Support:
Enable ICM20602 gyro and accelerometer for MATEKF411 or a new TYRO79 target.
Support BMP280 barometer on I2C2 (PB3 for SDA, PB10 for SCL).
Support servo output on PA8 (LED pad).


Firmware Compatibility:
Target INAV 6.1.0 to match user hardware (commit d6ce6e70, GCC 10.2.1).
Ensure firmware fits within 512 KB flash (STM32F411).


Verification:
CLI commands (sensor, status) show GYRO=ICM20602, ACC=ICM20602, BARO=BMP280.
Configurator sensor graphs display gyro/accelerometer data.
Motors and servos function in test mode.



3.2 Non-Functional Requirements

Stability: No boot loops or communication failures post-flash.
Compatibility: Maintain compatibility with existing MATEKF411 boards (if not using a separate target).
Build Environment: Use Docker with GCC 10.2.1 for reproducibility.
Documentation: Update docs/Targets.md with Tyro79 details.

4. Proposed Solution
Option 1: Update MATEKF411 Target

Modify src/main/target/MATEKF411/target.h:#define USE_ACC
#define USE_GYRO
#define USE_ACC_ICM20602
#define USE_GYRO_ICM20602
#define GYRO_ICM20602_ALIGN CW270_DEG
#define ACC_ICM20602_ALIGN CW270_DEG
#define USE_BARO
#define USE_BARO_BMP280
#define BARO_I2C_INSTANCE (I2CDEV_2)
#define USE_I2C
#define I2C2_SCL PB10
#define I2C2_SDA PB3
#define USE_I2C_PULLUP
#define USE_SERVOS
#define SERVO1_PIN PA8


Ensure SPI pins for ICM20602 match the Tyro79 schematic.
Pros: Simplifies maintenance, leverages existing target.
Cons: May conflict with Matek F411 boards using MPU6500.


Step 2: Update the Dockerfile for GCC 10.2.1
The CLI output shows INAV 6.1.0 was built with GCC 10.2.1, but your previous build attempt used GCC 13.2.1, which caused issues. Modify the Dockerfile to use the correct toolchain version:

Open the Dockerfile:
bash

Copy
nano ~/inav/Dockerfile
Add the following after the apt-get install line to install GCC 10.2.1:
dockerfile

Copy
RUN wget https://developer.arm.com/-/media/Files/downloads/gnu-rm/10.2-2020.11/arm-none-eabi-gcc-10.2.1.tar.bz2 && \
    tar -xjf arm-none-eabi-gcc-10.2.1.tar.bz2 -C /opt && \
    ln -s /opt/arm-none-eabi-gcc-10.2.1/bin/* /usr/bin/ && \
    rm arm-none-eabi-gcc-10.2.1.tar.bz2
Save and exit.