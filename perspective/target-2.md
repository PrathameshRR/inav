Thank you for sharing the Betaflight CLI `status` output for your Eachine Tyro79 quadcopter’s flight controller. This confirms that the Tyro79’s STM32F411-based flight controller correctly detects the **ICM20602** gyro and accelerometer when running Betaflight’s `MATEKF411` target, aligning with [Issue #4863](https://github.com/iNavFlight/inav/issues/4863). However, your INAV 6.1.0 build for the `MATEKF411` target (commit `b168dc62`, built May 30, 2025) shows `GYRO=UNAVAILABLE, ACC=NONE` in the CLI output, indicating that the INAV firmware is not properly configured for the ICM20602. The successful Betaflight output provides a reference to fix the INAV configuration.

Given your goal to modify INAV 6.1.0 for the Tyro79, ensuring proper sensor detection (ICM20602) and potentially supporting a BMP280 barometer and PA8 servo output (per [Discussion #6716](https://github.com/iNavFlight/inav/discussions/6716)), I’ll provide steps to align the INAV `MATEKF411` target with Betaflight’s configuration, rebuild the firmware, and verify sensor detection. I’ll also ensure the `Dockerfile` and `cmake/docker.sh` are optimized to avoid build errors (like the previous `No rule to make target 'MATEKF411'`).

### Analysis of Betaflight CLI Output
- **Key Details**:
  - **MCU**: STM32F411, clocked at 96 MHz, matching INAV’s output.
  - **Sensors**: `GYRO=ICM20602, ACC=ICM20602`, confirming the Tyro79’s IMU is functional and detected via SPI (likely SPI1, as per Betaflight’s `MATEKF411`).
  - **I2C Errors**: `I2C Errors: 3`, suggesting I2C2 (used for BMP280 in [Discussion #6716](https://github.com/iNavFlight/inav/discussions/6716)) may have issues or no device is connected.
  - **Config**: Stack size 2048, config size 3000, max 16384, similar to INAV’s 8999/16384.
  - **Arming Flags**: `RXLOSS ANGLE CLI MSP`, indicating no receiver or calibration, which is normal for a CLI session.
- **Comparison with INAV**:
  - INAV’s `GYRO=UNAVAILABLE, ACC=NONE` indicates a misconfiguration in `src/main/target/MATEKF411/target.h` or `target.c`, likely missing `USE_ACC_ICM20602`, `USE_GYRO_ICM20602`, or incorrect SPI pins.
  - Betaflight’s success suggests INAV needs identical ICM20602 and SPI settings.
  - INAV’s `BARO=NONE` may be correct if no BMP280 is connected, but I2C2 (PB3, PB10) settings need verification.

### Steps to Fix INAV Sensor Detection

#### Step 1: Update `MATEKF411` Target Configuration
Align INAV’s `MATEKF411` target with Betaflight’s, focusing on ICM20602 and SPI1 settings.

1. **Edit `target.h`**:
   ```bash
   cd ~/inav
   nano src/main/target/MATEKF411/target.h
   ```
2. **Update for ICM20602**:
   Use the following configuration, incorporating ICM20602 support ([Issue #4863](https://github.com/iNavFlight/inav/issues/4863)), optional BMP280, and PA8 servo ([Discussion #6716](https://github.com/iNavFlight/inav/discussions/6716)):
   ```c
   #define TARGET_BOARD_IDENTIFIER "MK41"
   #define USBD_PRODUCT_STRING     "MATEKF411"

   #define USE_ACC
   #define USE_GYRO
   #define USE_ACC_ICM20602
   #define USE_GYRO_ICM20602
   #define GYRO_ICM20602_ALIGN CW270_DEG
   #define ACC_ICM20602_ALIGN CW270_DEG
   #define GYRO_1_CS_PIN PB2
   #define GYRO_1_SPI_INSTANCE SPI1

   #define USE_BARO
   #define USE_BARO_BMP280
   #define BARO_I2C_INSTANCE (I2CDEV_2)
   #define USE_I2C
   #define I2C2_SCL PB10
   #define I2C2_SDA PB3
   #define USE_I2C_PULLUP

   #define USE_SERVOS
   #define SERVO1_PIN PA8

   #define LED0_PIN PB9
   #define USE_VCP
   #define USE_UART1
   #define UART1_TX_PIN PA9
   #define UART1_RX_PIN PA10
   #define USE_UART2
   #define UART2_TX_PIN PA2
   #define UART2_RX_PIN PA3
   #define USE_UART3
   #define UART3_TX_PIN PB10
   #define UART3_RX_PIN PB11

   #define USE_SPI
   #define USE_SPI_DEVICE_1
   #define SPI1_SCK_PIN PA5
   #define SPI1_MISO_PIN PA6
   #define SPI1_MOSI_PIN PA7

   #define USE_I2C_DEVICE_2
   #define FLASH_CS_PIN PB0
   #define FLASH_SPI_INSTANCE SPI1

   #define USE_MAX7456
   #define MAX7456_SPI_CS_PIN PA4
   #define MAX7456_SPI_INSTANCE SPI1

   #define USE_ADC
   #define ADC1_DMA_STREAM DMA2_Stream0
   #define VBAT_ADC_PIN PC0
   #define CURRENT_METER_ADC_PIN PC1
   ```
   - **Changes**:
     - Added `USE_ACC_ICM20602`, `USE_GYRO_ICM20602` for Tyro79’s IMU.
     - Set `GYRO_1_CS_PIN PB2` (based on Betaflight’s `MATEKF411` for ICM20602).
     - Kept BMP280 on I2C2 (PB3, PB10) and PA8 servo.
     - Ensured SPI1 pins (`PA5`, `PA6`, `PA7`) match Betaflight’s configuration.

3. **Edit `target.c`**:
   ```bash
   nano ~/inav/src/main/target/MATEKF411/target.c
   ```
   Ensure:
   ```c
   #include <stdint.h>
   #include <platform.h>
   #include "drivers/io.h"
   #include "drivers/pwm_output.h"
   #include "drivers/max7456.h"

   #define SERVO1_PIN PA8
   ```
   - No changes needed unless custom pin mappings are required.

#### Step 2: Verify Build System
The previous build error (`No rule to make target 'MATEKF411'`) was likely due to `cmake/docker.sh` using `make` instead of `ninja`. Your `Dockerfile` is correct, but let’s ensure `cmake/docker.sh` uses `ninja`.

1. **Edit `cmake/docker.sh`**:
   ```bash
   nano ~/inav/cmake/docker.sh
   ```
2. **Update to Use Ninja**:
   Replace with:
   ```bash
   #!/bin/bash
   mkdir -p /src/build
   cd /src/build
   cmake .. -G Ninja
   ninja $1
   ```
   Save and exit.

3. **Check `CMakeLists.txt`**:
   ```bash
   nano ~/inav/src/main/CMakeLists.txt
   ```
   - Confirm `MATEKF411` is included:
     ```cmake
     add_firmware_target(MATEKF411)
     ```
   - If missing, add it alphabetically.

#### Step 3: Rebuild the Docker Image
Rebuild to apply any changes:
```bash
cd ~/inav
docker build -t inav-build --build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g) .
```

#### Step 4: Build the Firmware
Run the build:
```bash
docker run -it --rm -v $(pwd):/src inav-build MATEKF411
```
If it fails, run manually:
```bash
docker run -it --rm -v $(pwd):/src inav-build bash
cd /src/build
cmake .. -G Ninja
ninja -t targets  # Verify MATEKF411
ninja MATEKF411
```

#### Step 5: Flash and Verify
1. **Locate the Firmware**:
   ```bash
   ls ~/inav/build
   ```
   Look for `inav_6.1.0_MATEKF411.hex`.
2. **Flash with INAV Configurator 6.1.0**:
   - Open INAV Configurator 6.1.0 (Windows, COM5).
   - Go to “Firmware Flasher,” select `inav_6.1.0_MATEKF411.hex`, enable “Full Chip Erase,” and flash.
   - If DFU mode is needed, hold the boot button.
3. **Verify Sensors**:
   - In the CLI, run:
     ```
     sensor
     status
     ```
     Expect `GYRO=ICM20602, ACC=ICM20602` (and `BARO=BMP280` if connected).
   - Check Configurator’s sensor graphs.
4. **Configure**:
   - Enable PWM outputs in the “Outputs” tab.
   - Set up the mixer for a quadcopter in the “Mixer” tab.
   - Calibrate sensors in the “Calibration” tab.

#### Step 6: Troubleshoot Sensor Issues
If `GYRO=UNAVAILABLE, ACC=NONE` persists:
- **Verify SPI Pins**:
  - Betaflight confirms ICM20602 on SPI1. If `PB2` as `GYRO_1_CS_PIN` is incorrect, try `PB0` (common for `MATEKF411`):
    ```c
    #define GYRO_1_CS_PIN PB0
    ```
  - Check the Tyro79 schematic or Betaflight’s `MATEKF411` target.h (available in [Betaflight’s GitHub](https://github.com/betaflight/betaflight/blob/master/src/main/target/MATEKF411/target.h)).
- **I2C Errors**:
  - Betaflight shows `I2C Errors: 3`. If using BMP280, verify I2C2 wiring (PB3, PB10) and run:
    ```
    i2c
    ```
    If no BMP280 is connected, remove:
    ```c
    #define USE_BARO
    #define USE_BARO_BMP280
    #define BARO_I2C_INSTANCE (I2CDEV_2)
    ```
- **Alignment**:
  - If sensors detect but behave oddly, try alignments like `CW0_DEG` or `CW90_DEG`.
- **Test Betaflight Again**:
  - If INAV fails, reflash Betaflight’s `MATEKF411` to confirm hardware functionality.

### Updated `target.h`

```x-csrc
#define TARGET_BOARD_IDENTIFIER "MK41"
#define USBD_PRODUCT_STRING     "MATEKF411"

#define USE_ACC
#define USE_GYRO
#define USE_ACC_ICM20602
#define USE_GYRO_ICM20602
#define GYRO_ICM20602_ALIGN CW270_DEG
#define ACC_ICM20602_ALIGN CW270_DEG
#define GYRO_1_CS_PIN PB2
#define GYRO_1_SPI_INSTANCE SPI1

#define USE_BARO
#define USE_BARO_BMP280
#define BARO_I2C_INSTANCE (I2CDEV_2)
#define USE_I2C
#define I2C2_SCL PB10
#define I2C2_SDA PB3
#define USE_I2C_PULLUP

#define USE_SERVOS
#define SERVO1_PIN PA8

#define LED0_PIN PB9
#define USE_VCP
#define USE_UART1
#define UART1_TX_PIN PA9
#define UART1_RX_PIN PA10
#define USE_UART2
#define UART2_TX_PIN PA2
#define UART2_RX_PIN PA3
#define USE_UART3
#define UART3_TX_PIN PB10
#define UART3_RX_PIN PB11

#define USE_SPI
#define USE_SPI_DEVICE_1
#define SPI1_SCK_PIN PA5
#define SPI1_MISO_PIN PA6
#define SPI1_MOSI_PIN PA7

#define USE_I2C_DEVICE_2
#define FLASH_CS_PIN PB0
#define FLASH_SPI_INSTANCE SPI1

#define USE_MAX7456
#define MAX7456_SPI_CS_PIN PA4
#define MAX7456_SPI_INSTANCE SPI1

#define USE_ADC
#define ADC1_DMA_STREAM DMA2_Stream0
#define VBAT_ADC_PIN PC0
#define CURRENT_METER_ADC_PIN PC1
```

### Troubleshooting
- **Sensor Detection**:
  - If `GYRO=ICM20602` isn’t detected, compare with Betaflight’s `MATEKF411` target.h and target.c.
  - Run:
    ```bash
    grep -r "ICM20602" src/main/target/MATEKF411
    ```
    Ensure no conflicting IMU settings.
- **I2C Errors**:
  - If BMP280 isn’t detected, test I2C2 with a multimeter or remove barometer settings.
- **Flash Size**:
  - If the firmware exceeds 512 KB, disable features:
    ```c
    #undef USE_TELEMETRY
    #undef USE_GPS
    ```
- **Build Errors**:
  - If `No rule to make target 'MATEKF411'` reappears, verify:
    ```bash
    ninja -t targets
    ```

### Next Steps
- Flash the updated firmware and share the new `sensor` and `status` CLI output.
- If you need specific firmware changes (e.g., additional features), let me know.
- Consider submitting a pull request to [iNavFlight/inav](https://github.com/iNavFlight/inav) for 6.1.0, referencing [Issue #4863](https://github.com/iNavFlight/inav/issues/4863).

Would you like me to draft a pull request, compare Betaflight’s `MATEKF411` target files, or assist with I2C debugging for the BMP280?