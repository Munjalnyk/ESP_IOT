#ifndef _INCLUDES_H 
#define _INCLUDES_H

<<<<<<< Updated upstream
#include <time.h>
#include "esp_sntp.h"
#include "esp_http_server.h"
// this is dev branch....
=======

#include <time.h>
#include "esp_sntp.h"
#include "esp_http_server.h"
#include "driver/i2c.h"
>>>>>>> Stashed changes
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
// #include "mdns.h"
#include "cJSON.h"
#include "esp_spiffs.h"
#include "esp_wifi.h"
#include "freertos/event_groups.h"
#include "esp_event.h"
#include "mqtt_client.h"
#include "esp_err.h"
#include <stdbool.h>
#include "freertos/semphr.h"
#include "esp_netif.h"
#include <stdio.h>
#include <stdlib.h>
#include "rom/gpio.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_continuous.h"
#include "driver/uart.h"
#include "esp32c3/rom/uart.h"
#include "sdkconfig.h"
#include "esp_intr_alloc.h"
#include "esp_log.h"
#include "string.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "freertos/timers.h"
#include "spi_flash_mmap.h"
#include "freertos/queue.h"
#include "esp_http_client.h"
#include "esp_https_ota.h"
#include "driver/spi_master.h"
// #include "esp_websocket_client.h"
#include <inttypes.h>
#include "nvs.h"
#include "nvs.c"
#include "init.h"
#include "init.c"
#include "wifi.h"
#include "wifi.c"
#include "xtime.h"
#include "xtime.c"
#include "xlog.h"
#include "xlog.c"
#include "xspi.h"
#include "xspi.c"
#include "xmqtt.h"
#include "xmqtt.c"
#include "XSD.c"
#include "XSD.h"

// #include "GSM_Drv.h"
// #include "PinConfig.h"
// #include "main.h"
// #include "Perif_Drv.h"
// #include "Common.h"
// #include "RS485_Drv.h"
// #include "memory.h"
// #include "MQTT_Manager.h"
// #include "NTP_Time.h"
// #include "TestCodes.h"
// #include "WiFi_Manager.h"
// #include "Config_System.h"
// #include "FOTA.h"
// #include "HTTP_Serv.h"
// #include "EM_Driver.h"
// #include "Length_Ctr.h"
// #include "app.h"
// #include "esp_temp_sensor.h"
#endif