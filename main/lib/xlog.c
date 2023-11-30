#include "include.h"
#include <stdarg.h>
#include "ds3231.h"
void XSD_write_file(char *path, char *data);
void XSMQTT(char *data);
void XLOGDATA(char *data);
void LOGMQTT(char *data)
{
  XSMQTT(data);
}
i2c_dev_t dev;
#define MOUNT_POINT "/sdcard"
void XLOG(char *function, char *location, const char *format, ...) {
    char *Current_Date_Time = (char *)malloc(100);
    Get_current_date_time(Current_Date_Time);
    float temp;
    struct tm rtcinfo;

    ds3231_get_temp_float(&dev, &temp);

    ds3231_get_time(&dev, &rtcinfo);

    va_list args;
    va_start(args, format);
    
    printf("Local Time %04d-%02d-%02d %02d:%02d:%02d, %.2f deg Cel: SNTP time: %s : %s : %s : ",rtcinfo.tm_year, rtcinfo.tm_mon + 1,
           rtcinfo.tm_mday, rtcinfo.tm_hour, rtcinfo.tm_min, rtcinfo.tm_sec, temp, Current_Date_Time, function, location);
    vprintf(format, args);  // Print the log message using variable arguments
    printf("\n");
    int MAX_LOG_SIZE = 100;
    char data[MAX_LOG_SIZE];
    int data_len = snprintf(data, sizeof(data), "Local Time %04d-%02d-%02d %02d:%02d:%02d, %.2f deg Cel: SNTP time: %s : %s : %s : ",rtcinfo.tm_year, rtcinfo.tm_mon + 1,
           rtcinfo.tm_mday, rtcinfo.tm_hour, rtcinfo.tm_min, rtcinfo.tm_sec, temp, Current_Date_Time, function, location);
    int format_len = vsnprintf(data + data_len, sizeof(data) - data_len, format, args);
    data_len += format_len;
    snprintf(data + data_len, sizeof(data) - data_len, "\n");

    
    if (strcmp(location, "MQTT") == 0)
    {
        LOGMQTT(data);
    }
     if (strcmp(location, "XSD") == 0)
    {
        XLOGDATA(data);
    }
va_end(args);
    free(Current_Date_Time);
}


