#### 2. Create or Modify Target
- **Option A: Modify MATEKF411**:
  - Edit `target.h` and `target.c` to match TCMMF411’s settings from Betaflight 4.3.0.
- **Option B: Create TCMMF411**:
  - Copy `MATEKF411/` to a new `TCMMF411/` folder.
  - Update `target.h`:
    ```c
    #define TARGET_BOARD_IDENTIFIER "TCMM"
    #define USBD_PRODUCT_STRING "Tyro79 Pro"
    ```
- **Key Changes** (based on your status and typical TCMMF411):
  - **IMU**:
    ```c
    #define USE_GYRO
    #define USE_ACC
    #define USE_GYRO_SPI_BMI270
    #define USE_ACC_SPI_BMI270
    #define BMI270_CS_PIN PA4  // Adjust based on TCMMF411
    #define BMI270_SPI_BUS BUS_SPI1
    #define IMU_BMI270_ALIGN CW270_DEG  // Verify via Betaflight CLI or source
    ```
  - **OSD**:
    ```c
    #define USE_MAX7456
    #define MAX7456_SPI_BUS BUS_SPI2
    #define MAX7456_CS_PIN PB12  // Adjust if different in TCMMF411
    ```
  - **Barometer** (if absent):
    ```c
    #undef USE_BARO
    #undef USE_BARO_BMP280
    ```
    If present, update I2C pins to match TCMMF411 (e.g., `I2C1_SCL PB8`, `I2C1_SDA PB9`).
  - **UARTs** (for GPS, receiver, telemetry):
    ```c
    #define USE_UART1
    #define UART1_TX_PIN PA9
    #define UART1_RX_PIN PA10
    #define USE_UART2
    #define UART2_TX_PIN PA2
    #define UART2_RX_PIN PA3
    #define USE_SOFTSERIAL1  // If needed
    #define SOFTSERIAL_1_TX_PIN PA0
    ```
  - **LED/Buzzer**:
    - Copy from TCMMF411 (e.g., `LED0 PC13`, `BEEPER PB2`).
