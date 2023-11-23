# ESP_IOT_v0.1

This repository is for developers who are enthusiastic about learning new things.

Currently, this code is designed for the ESP32 (ESP_IDF v5.0.4) using Visual Studio Code, and the development environment is Linux (Ubuntu 22.04.3).

This repository is a template, and it can be adjusted as per project needs. It has the following functionality:

- It can read and write from NVS.
- It can connect to Wi-Fi in AP, STA, APSTA modes.
- It has a separate log function in which it can log data into serial, MQTT, SD card.
- It can connect to MQTT with a certificate.
- It can do SNTP and get time for INDIA GMT +5:30 if connected to Wi-Fi.

## Functions to be added

- To integrate DS3231 RTC Module for timekeeping when powered off or Wi-Fi is not available, as time is used to log data.
- To integrate HW-290/GY-87 10DOF module consisting of MPU6050 + HMC5883L + BMP180 sensor for project-specific needs.
- To add webserver for logs and other needs, webserver need to be working on websocket for realtime update.
- To add OTA feature for Remote upgrade.
- To add connection for sql database for webpage user authentication.
- To add GSM and GPRS module if wifi is not available and GPRS to get location as project need.

## Devices connected to SPI

SD Card Reader [SD Card Reader](https://robu.in/product/sd-card-reading-writing-module-arduino/)

## SPI pins

| Pin Name | ESP32 | Device |
|----------|-------|--------|
| MOSI     | 15    | MOSI   |
| MISO     | 2     | MISO   |
| CS       | 13    | CS     |
| SCK      | 14    | SCK    |

## Devices Connected to I2C

DS3231 RTC Module [RTC Clock](https://robu.in/product/ds3231-rtc-module-precise-real-time-clock-i2c-at24c32/)

HW-290/GY-87 10DOF MPU6050 + HMC5883L + BMP180 [HW-290/GY-87 Sensor](https://robu.in/product/mpu6050hmc5883lbmp180-10dof-3-axis-gyro-3-axis-acceleration-3-axis-magnetic-field-air-pres/)

## I2C Pins

| Pin Name | ESP32 |
|----------|-------|
| SCL      | 19    |
| SDA      | 18    |

## Errors

In Linux, if not able to flash ESP32 from the terminal, it is due to inadequate permissions available to read and write to the USB port. To rectify it, use the following command:

```bash
sudo chown yourname /dev/ttyUSB0
