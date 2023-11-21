#include <include.h>
#include <stdarg.h>
void XSD_write_file(char *path, char *data);
void XSMQTT(char *data);
void XLOGDATA(char *data);
void LOGMQTT(char *data)
{
  XSMQTT(data);
}
#define MOUNT_POINT "/sdcard"
void XLOG(char *function, char *location, const char *format, ...) {
    char *Current_Date_Time = (char *)malloc(100);
    Get_current_date_time(Current_Date_Time);
    
    va_list args;
    va_start(args, format);
    
    printf("%s : %s : %s : ", Current_Date_Time, function, location);
    vprintf(format, args);  // Print the log message using variable arguments
    printf("\n");
    int MAX_LOG_SIZE = 100;
    char data[MAX_LOG_SIZE];
    int data_len = snprintf(data, sizeof(data), "%s : %s : %s : ", Current_Date_Time, function, location);
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


