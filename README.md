# ESP_IOT_v0.1

## Device connected to SPI

SD card Reader [SD Card Reader](https://robu.in/product/sd-card-reading-writing-module-arduino/)

## SPI pins

| Pin Name | ESP32    | Device   |
|----------|----------|----------|
| MOSI     | 15       | MOSI     |
| MISO     | 2        | MISO     |
| CS       | 13       | CS       |
| SCK      | 14       | SCK      |

## Device Connected to I2C

DS3231 RTC Module [RTC Clock](https://robu.in/product/ds3231-rtc-module-precise-real-time-clock-i2c-at24c32/)

HW-290/GY-87 10DOF MPU6050 + HMC5883L + BMP180 [HW-290/GY-87 Sensor](https://robu.in/product/mpu6050hmc5883lbmp180-10dof-3-axis-gyro-3-axis-acceleration-3-axis-magnetic-field-air-pres/)

## I2C Pins

| Pin Name | ESP32 |
|----------|-------|
| SCL      | 19    |
| SDA      | 18    |

# Errors

if not flashing from terminal

sudo chown \<yourname\> /dev/ttyUSB0